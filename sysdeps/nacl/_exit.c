
#include <stdlib.h>
#include <unistd.h>

#include <nacl_syscalls.h>


void _exit (int status)
{
  int (*nacl_exit)(int status) = NACL_SYSCALL_ADDR(NACL_sys_exit);
  nacl_exit(status);
  /* In case the syscall returns: */
  __asm__("hlt");
}
libc_hidden_def (_exit)
weak_alias (_exit, _Exit)
