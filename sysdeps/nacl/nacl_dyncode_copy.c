
#include <errno.h>

#include <nacl_dyncode.h>
#include <nacl_syscalls.h>


int __nacl_dyncode_copy (void *dest, const void *src, size_t size)
{
  int retval = NACL_SYSCALL (dyncode_copy) (dest, src, size);
  if (retval < 0) {
    errno = -retval;
    return -1;
  }
  return retval;
}
libc_hidden_def (__nacl_dyncode_copy)
