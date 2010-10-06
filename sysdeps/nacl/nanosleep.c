
#include <errno.h>
#include <time.h>

#include <nacl_syscalls.h>


int __nanosleep (const struct timespec *req, struct timespec *rem)
{
  int result = NACL_SYSCALL (nanosleep) (req, rem);
  if ((unsigned int) result > 0xfffff000u) {
    errno = -result;
    return -1;
  }
  return result;
}
libc_hidden_def (__nanosleep)
weak_alias (__nanosleep, nanosleep)
