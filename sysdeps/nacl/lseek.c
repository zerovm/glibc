
#include <errno.h>
#include <unistd.h>

#include <irt_syscalls.h>


off_t __lseek (int fd, off_t offset, int whence)
{
  nacl_abi_off_t nacl_offset = offset;
  int result = __nacl_irt_seek (fd, nacl_offset, whence, &nacl_offset);
  if (result != 0) {
    errno = result;
    return -1;
  }
  return nacl_offset;
}
libc_hidden_def (__lseek)
weak_alias (__lseek, lseek)
