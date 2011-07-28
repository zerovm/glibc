
#include <errno.h>

#include <nacl_dyncode.h>
#include <irt_syscalls.h>


int __nacl_dyncode_create (void *dest, const void *src, size_t size)
{
  int retval = __nacl_irt_dyncode_create (dest, src, size);
  if (retval > 0) {
    errno = retval;
    return -1;
  }
  return -retval;
}
libc_hidden_def (__nacl_dyncode_create)
