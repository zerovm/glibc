
#include "receive_args.h"

#include <assert.h>
#include <errno.h>
#include <elf.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ldsodefs.h>

#include <nacl_rpc.h>
#include <nacl_syscalls.h>


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

#define MESSAGE_SIZE_MAX 0x10000


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
    (void *) srpc_reply_message,
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

uint32_t *argmsg_fetch (uint32_t *info)
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
  /* Find the descriptor for communicating with plugin.  The number differs
     with IRT because NaCl runtime creates more descriptors in order to load
     the IRT blob: three mutexes and one condvar.  We could distinguish IRT
     case from non-IRT case here by looking into the aux vector, but it does
     not make sense to write the proper code here until the hack with
     in-browser DSO loading sequence (described just above) goes away.
     TODO(pasko): eliminate testing socket numbers as part of dynamic linker
     startup.  */
  if (socket_fd == -1 && errno == EINVAL)
    {
      socket_fd = imc_accept (NACL_PLUGIN_BOUND_SOCK + 4);
    }
  if (socket_fd == -1 && errno == EBADF)
    {
      /* We are not running under the NaCl browser plugin or in a
         similar environment.  */
      return info;
    }
  keep_plugin_happy (socket_fd);

  union
  {
    unsigned char buffer[MESSAGE_SIZE_MAX];
    struct args_message message;
  } msgbuf;

  struct NaClImcMsgIoVec iov = { .base = &msgbuf, .length = sizeof(msgbuf) };
  struct NaClImcMsgHdr message = { .iov = &iov, .iov_length = 1 };

  int received_size = imc_recvmsg (NACL_PLUGIN_ASYNC_TO_CHILD_FD,
                                   &message, 0);

  /* As a workaround for a limitation in the NaCl plugin, allow the
     message to be hex-encoded.  This is because __sendAsyncMessage*()
     does not support null bytes in messages.
     See http://code.google.com/p/nativeclient/issues/detail?id=1535
     TODO(mseaborn): Fix this limitation.  */
  if (received_size >= 4 &&
      memcmp (msgbuf.message.tag, "HEXD", 4) == 0)
    decode_hex (msgbuf.buffer, &received_size,
                &msgbuf.buffer[4], received_size - 4);

  if (received_size < 0)
    _dl_fatal_printf ("Error receiving startup message (%u)\n", errno);
  if (received_size < 4 ||
      memcmp (msgbuf.message.tag, "ARGS", 4) != 0)
    _dl_fatal_printf ("Startup message (%u bytes) lacks the expected tag\n",
                      received_size);
  if (received_size < offsetof (struct args_message, string_data))
    _dl_fatal_printf ("Startup message too small (%u bytes)\n", received_size);

  /* Count the original auxv size so we know how big to make the new buffer.  */
  size_t nauxv = 0;
  Elf32_auxv_t *const auxv = (void *) &info[3 + info[2] + 1 + info[1] + 1];
  Elf32_auxv_t *av = auxv;
  do
    ++nauxv;
  while (av++->a_type != AT_NULL);

  /* Allocate a new information block in the heap.
     We will pass this pointer to the user program rather than
     the original argument (which was a pointer onto our stack).  */
  size_t infosize = ((3 + msgbuf.message.argc + 1 +
                      msgbuf.message.envc + 1) * sizeof(uint32_t) +
                     nauxv * sizeof(Elf32_auxv_t) +
                     (received_size -
                      offsetof (struct args_message, string_data)));
  uint32_t *newinfo = __mmap (NULL, infosize, PROT_READ|PROT_WRITE,
                              MAP_PRIVATE|MAP_ANON, -1, 0);
  if (newinfo == NULL)
    _dl_fatal_printf ("Cannot allocate %u bytes for startup information\n",
                      infosize);

  /* Fill in the new information block.
     We replace the arguments and environment with what we got from IPC,
     and copy the rest from the original block.  */
  newinfo[0] = info[0];
  newinfo[1] = msgbuf.message.envc;
  newinfo[2] = msgbuf.message.argc;

  /* Copy the auxiliary vector, which sits after the string vectors.  */
  char **argv = (char **) &newinfo[3];
  char **envp = &argv[msgbuf.message.argc + 1];
  char *copy = __mempcpy (&envp[msgbuf.message.envc + 1],
                          auxv, nauxv * sizeof(auxv[0]));

  /* Now copy the strings into place and fill in the string vectors.  */
  const char *p = msgbuf.message.string_data;
  const char *endp = (char *) &msgbuf.buffer[received_size];

  /* First, the argument vector.  */
  for (int i = 0; i < msgbuf.message.argc; ++i)
    {
      const char *elt_end = memchr (p, '\0', endp - p);
      if (elt_end == NULL)
        _dl_fatal_printf ("Unterminated argument string in startup message\n");
      ++elt_end;
      argv[i] = copy;
      copy = __mempcpy (copy, p, elt_end - p);
      p = elt_end;
    }
  argv[msgbuf.message.argc] = NULL;

  /* Finally, the environment vector.  */
  for (int i = 0; i < msgbuf.message.envc; ++i)
    {
      const char *elt_end = memchr (p, '\0', endp - p);
      if (elt_end == NULL)
        _dl_fatal_printf ("\
Unterminated environment string in startup message\n");
      ++elt_end;
      envp[i] = copy;
      copy = __mempcpy (copy, p, elt_end - p);
      p = elt_end;
    }
  envp[msgbuf.message.envc] = NULL;

  if (p != endp)
    _dl_fatal_printf ("Excess data (%u bytes) in startup message body\n",
                      endp - p);

  return newinfo;
}
