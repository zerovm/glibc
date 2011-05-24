#include <nacl_syscalls.h>
#include <unistd.h>

void __exit_thread (int val)
{
  NACL_SYSCALL (thread_exit) (NULL);
  /* Not reached */
  __asm__ ("hlt");
}