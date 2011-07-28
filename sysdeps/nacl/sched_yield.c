
#include <errno.h>
#include <sched.h>

#include <irt_syscalls.h>


int __sched_yield (void)
{
  int result = __nacl_irt_sched_yield ();
  if (result != 0) {
    errno = -result;
    return -1;
  }
  return -result;
}
libc_hidden_def (__sched_yield)
weak_alias (__sched_yield, sched_yield)
