
#include <errno.h>
#include <sched.h>

#include <nacl_syscalls.h>


int __sched_yield (void)
{
  int result = NACL_SYSCALL (sched_yield) ();
  if ((unsigned int) result > 0xfffff000u) {
    errno = -result;
    return -1;
  }
  return result;
}
libc_hidden_def (__sched_yield)
weak_alias (__sched_yield, sched_yield)
