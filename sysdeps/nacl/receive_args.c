
#include "receive_args.h"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <nacl_rpc.h>


/* We expect to receive an IMC message with the following format:

   Fixed-size header:

     "ARGS" (4 bytes) - message type tag
     argc (4 bytes, uint32_t) - number of strings in argv array
     envc (4 bytes, uint32_t) - number of strings in environment array

   This is followed by the argv and env strings, each with a null terminator:

     <argv string 0> '\0'
     <argv string 1> '\0'
     ...
     <argv string (argc-1)> '\0'

     <env string 0> '\0'
     <env string 1> '\0'
     ...
     <env string (envc-1)> '\0'

   The strings are not padded; they are not necessarily aligned.
*/

struct args_message
{
  char tag[4];
  uint32_t argc;
  uint32_t envc;
  char string_data[];
};

struct process_args
{
  int received_size;
  struct args_message message;
};

#define MESSAGE_SIZE_MAX 0x10000


static void fail (const char *message)
{
  __write (2, message, strlen (message));
  _exit (127);
}

/* The NaCl plugin blocks waiting for us to accept an SRPC connection.
   If we reject the connection, the "onload" Javascript hook is not run.
   See http://code.google.com/p/nativeclient/issues/detail?id=1501
   Since it is currently not practical to link libsrpc into the dynamic
   linker, we have to hard-code the response.
   TODO(mseaborn): Fix the plugin's process startup interface so that we
   do not have to send it the SRPC connection acceptance message below.  */
static const uint8_t srpc_reply_message[] =
{
  /* struct LengthHeader[2]: */
  11 * 4, 0, 0, 0, /* total byte_count */
  0, 0, 0, 0,      /* total desc_count */
  11 * 4, 0, 0, 0, /* this fragment's byte_count */
  0, 0, 0, 0,      /* this fragment's desc_count */

  /* struct NACL_SRPC_RPC_SERIALIZED_FIELDS: */
  /* protocol_version (fixed, kNaClSrpcProtocolVersion) */
  2, 0, 0xda, 0xc0,
  /* request_id */
  0, 0, 0, 0,
  /* is_request */
  0, 0, 0, 0,
  /* rpc_number (method number): 0 for service_discovery */
  0, 0, 0, 0,
  /* result (return code): 256 (NACL_SRPC_RESULT_OK) */
  0, 1, 0, 0,
  /* value_len (for a reply, the number of result values): 1 */
  1, 0, 0, 0,
  /* template_len (not meaningful for a reply): 0 */
  0, 0, 0, 0,

  /* struct NACL_SRPC_ARG_SERIALIZED_FIELDS: */
  /* argument 1 type: NACL_SRPC_ARG_TYPE_CHAR_ARRAY */
  'C', 0, 0, 0,
  /* pad to 8 byte alignment */
  0, 0, 0, 0,
  /* argument 1 length: 0 (empty method list) */
  0, 0, 0, 0,
  /* pad up to union size */
  0, 0, 0, 0,
};

static int keep_plugin_happy (int socket_fd)
{
  /* Receive service_discovery SRPC request.  This should always be
     the first message.  */
  char buf[20];
  struct NaClImcMsgIoVec recv_iov = { buf, sizeof (buf) };
  struct NaClImcMsgHdr recv_message = { &recv_iov, 1, NULL, 0, 0 };
  int received = imc_recvmsg (socket_fd, &recv_message, 0);
  if (received < 0)
    return -1;

  /* Send reply, listing no SRPC methods.  */
  struct NaClImcMsgIoVec send_iov = {
    srpc_reply_message,
    sizeof (srpc_reply_message)
  };
  struct NaClImcMsgHdr send_message = { &send_iov, 1, NULL, 0, 0 };
  int sent = imc_sendmsg (socket_fd, &send_message, 0);
  if (sent < 0)
    return -1;

  if (__close (socket_fd) < 0)
    return -1;

  return 0;
}

static int from_hex (unsigned char value)
{
  switch (value)
    {
      case '0': return 0;
      case '1': return 1;
      case '2': return 2;
      case '3': return 3;
      case '4': return 4;
      case '5': return 5;
      case '6': return 6;
      case '7': return 7;
      case '8': return 8;
      case '9': return 9;
      case 'a': case 'A': return 10;
      case 'b': case 'B': return 11;
      case 'c': case 'C': return 12;
      case 'd': case 'D': return 13;
      case 'e': case 'E': return 14;
      case 'f': case 'F': return 15;
      default: return 0;
    }
}

static void decode_hex (unsigned char *dest, int *dest_size,
                        unsigned char *src, int src_size)
{
  int i;
  for (i = 0; i < src_size / 2; i++)
    dest[i] = from_hex (src[i*2]) * 0x10 + from_hex (src[i*2 + 1]);
  *dest_size = src_size / 2;
}

struct process_args *argmsg_fetch ()
{
  /* The NaCl browser plugin does not give us a good way to detect
     that we are running under it.  We detect that we are running
     under the plugin by the presence of the file descriptor
     NACL_PLUGIN_BOUND_SOCK.
     See http://code.google.com/p/nativeclient/issues/detail?id=889

     Furthermore, the NaCl plugin blocks waiting for us to accept or
     reject an SRPC connection.  We cannot usefully accept the
     connection at this point, but we need to unblock the plugin so
     that we can send it file open requests.  So we reject the
     connection by closing the socket.
     See http://code.google.com/p/nativeclient/issues/detail?id=673

     TODO(mseaborn): Fix these problems with the plugin's process
     startup interface.  */
  int socket_fd = imc_accept (NACL_PLUGIN_BOUND_SOCK);
  if (socket_fd == -1 && errno == EBADF)
    {
      /* We are not running under the NaCl browser plugin or in a
         similar environment.  */
      return NULL;
    }
  keep_plugin_happy (socket_fd);
  struct process_args *args =
    mmap (NULL, MESSAGE_SIZE_MAX, PROT_READ | PROT_WRITE,
          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (args == MAP_FAILED)
    fail ("Failed to allocate memory to receive startup message\n");
  struct NaClImcMsgIoVec iov;
  struct NaClImcMsgHdr message;
  iov.base = (void *) &args->message;
  iov.length = MESSAGE_SIZE_MAX - offsetof (struct process_args, message);
  message.iov = &iov;
  message.iov_length = 1;
  message.descv = NULL;
  message.desc_length = 0;
  message.flags = 0;
  args->received_size = imc_recvmsg (NACL_PLUGIN_ASYNC_TO_CHILD_FD,
                                     &message, 0);

  /* As a workaround for a limitation in the NaCl plugin, allow the
     message to be hex-encoded.  This is because __sendAsyncMessage*()
     does not support null bytes in messages.
     See http://code.google.com/p/nativeclient/issues/detail?id=1535
     TODO(mseaborn): Fix this limitation.  */
  if (args->received_size >= 4 &&
      memcmp (args->message.tag, "HEXD", 4) == 0)
    decode_hex ((unsigned char *) &args->message, &args->received_size,
                (unsigned char *) &args->message + 4, args->received_size - 4);

  if (args->received_size < 0)
    fail ("Error receiving startup message\n");
  if (args->received_size < 4 ||
      memcmp (args->message.tag, "ARGS", 4) != 0)
    fail ("Startup message does not have the expected tag\n");
  if (args->received_size < offsetof (struct args_message, string_data))
    fail ("Startup message too small\n");
  /* Check for an attempt to allocate too much space on the stack.  */
  if (args->message.argc > MESSAGE_SIZE_MAX ||
      args->message.envc > MESSAGE_SIZE_MAX ||
      args->message.argc + args->message.envc > MESSAGE_SIZE_MAX)
    fail ("argv/env too large\n");
  return args;
}

static size_t arrays_size (struct process_args *args)
{
  return (sizeof(argc_type) +
          sizeof(char *) * (args->message.argc + 1 +
                            args->message.envc + 1 +
                            2 /* for empty auxv */));
}

static size_t strings_size (struct process_args *args)
{
  return args->received_size - offsetof (struct args_message, string_data);
}

size_t argmsg_get_size_on_stack (struct process_args *args)
{
  return arrays_size (args) + strings_size (args);
}

void argmsg_move_to_stack (struct process_args *args,
                           void *buf, size_t buf_size)
{
  char *buf_end = (char *) buf + buf_size;
  char *strings = (char *) buf + arrays_size (args);
  memcpy (strings, args->message.string_data, strings_size (args));

  *(argc_type *) buf = args->message.argc;
  char **dest = (char **) ((char *) buf + sizeof(argc_type));
  char *next_str = strings;
  int i;
  /* Set up argv array */
  for (i = 0; i < args->message.argc; i++)
    {
      *dest++ = next_str;
      next_str = (char *) memchr (next_str, 0, buf_end - next_str);
      if (next_str == NULL)
        fail ("Missing null terminator in argv list\n");
      next_str++;
    }
  *dest++ = NULL;
  /* Set up environment array */
  for (i = 0; i < args->message.envc; i++)
    {
      *dest++ = next_str;
      next_str = (char *) memchr (next_str, 0, buf_end - next_str);
      if (next_str == NULL)
        fail ("Missing null terminator in env list\n");
      next_str++;
    }
  *dest++ = NULL;
  /* Set up an empty auxv */
  *dest++ = NULL;
  *dest++ = NULL;
  if ((char *) dest != strings)
    fail ("Internal error: mismatch in array size\n");
  if (next_str != buf_end)
    fail ("Excess data in message body\n");

  if (munmap (args, MESSAGE_SIZE_MAX) != 0)
    fail ("Failed to munmap() startup message\n");
}

void jump_to_elf_start (void *buf, uintptr_t entry_func, uintptr_t atexit_func)
{
  /* The ELF entry point ABI is such that assembly code is required to
     call the entry point.
     See http://code.google.com/p/nativeclient/issues/detail?id=1131
     TODO(mseaborn): Switch to using the normal function call ABI.  */
#if defined(__i386__)
  __asm__ ("mov %0, %%esp\n"
           "nacljmp %1\n"
           : : "m" (buf), "r" (entry_func),
               "d" (atexit_func) /* %edx */);
#elif defined(__x86_64__)
  __asm__ ("naclrestsp %0, %%r15\n"
           "nacljmp %1, %%r15\n"
           : : "m" (buf), "r" (entry_func),
               "d" (atexit_func) /* %rdx */);
#else
# error Unsupported architecture
#endif
}
