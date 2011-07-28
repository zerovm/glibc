
#include <stdlib.h>
#include <unistd.h>

#include <irt_syscalls.h>


void _exit (int status)
{
  __nacl_irt_exit (status);
  /* In case the syscall returns: */
  while (1)
    __asm__("hlt");
}
libc_hidden_def (_exit)
weak_alias (_exit, _Exit)
