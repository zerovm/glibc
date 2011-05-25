#include <atomic.h>
#include <nacl_syscalls.h>
#include <nptl/pthreadP.h>
#include <unistd.h>

void __exit_thread (int val)
{
  /* We are about to die: make our pd "almost free" and wake up waiter. */
  struct pthread* pd = THREAD_SELF;
  pd->tid = -2;
  __nacl_futex_wake (&pd->tid, INT_MAX, __FUTEX_BITSET_MATCH_ANY);
  NACL_SYSCALL (thread_exit) (&pd->tid);
  /* Not reached */
  __asm__ ("hlt");
}