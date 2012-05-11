#include <errno.h>
#include <stddef.h>
#include <unistd.h>

#include <irt_syscalls.h>

clock_t __clock (void)
{
  clock_t ticks;
  int result = __nacl_irt_clock (&ticks);
  if (result != 0) {
    errno = result;
    return -1;
  }
  return ticks;
}
weak_alias (__clock, clock)
