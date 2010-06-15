
#include <stdlib.h>
#include <unistd.h>

#include <nacl_syscalls.h>


void _exit (int status)
{
  NACL_SYSCALL (exit) (status);
  /* In case the syscall returns: */
  while (1)
    __asm__("hlt");
}
libc_hidden_def (_exit)
weak_alias (_exit, _Exit)
