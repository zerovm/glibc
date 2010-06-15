
#include <errno.h>
#include <unistd.h>

#include <nacl_syscalls.h>


int __close (int fd)
{
  int result = NACL_SYSCALL (close) (fd);
  if (result < 0) {
    errno = -result;
    return -1;
  }
  return result;
}
libc_hidden_def (__close)
weak_alias (__close, close)
strong_alias (__close, __libc_close)
strong_alias (__close, __close_nocancel)
