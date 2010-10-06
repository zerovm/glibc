
#include <errno.h>
#include <time.h>

#include <nacl_syscalls.h>


/* Undo the #define in include/sys/time.h */
#undef __gettimeofday

int __gettimeofday (struct timeval *tv, struct timezone *tz)
{
  int result = NACL_SYSCALL (gettimeofday) (tv, tz);
  if ((unsigned int) result > 0xfffff000u) {
    errno = -result;
    return -1;
  }
  return result;
}
INTDEF (__gettimeofday)
weak_alias (__gettimeofday, gettimeofday)
