#include <atomic.h>
#include <irt_syscalls.h>
#include <nptl/pthreadP.h>
#include <unistd.h>

void __exit_thread (int val)
{
  /* We are about to die: make our pd "almost free" and wake up waiter. */
  struct pthread* pd = THREAD_SELF;
  int count;
  pd->tid = -2;
  __nacl_futex_wake (&pd->tid, INT_MAX, __FUTEX_BITSET_MATCH_ANY, &count);
  __nacl_irt_thread_exit (&pd->tid);
  /* Not reached */
  __asm__ ("hlt");
}