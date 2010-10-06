
#include <errno.h>
#include <unistd.h>

#include <nacl_syscalls.h>


off_t __lseek (int fd, off_t offset, int whence)
{
  off_t result = NACL_SYSCALL (lseek) (fd, offset, whence);
  if ((unsigned int) result > 0xfffff000u) {
    errno = -result;
    return -1;
  }
  return result;
}
libc_hidden_def (__lseek)
weak_alias (__lseek, lseek)
