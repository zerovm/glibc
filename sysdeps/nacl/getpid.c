
#include <unistd.h>

#include <nacl_syscalls.h>


pid_t __getpid ()
{
  return NACL_SYSCALL (getpid) ();
}
libc_hidden_def (__getpid)
weak_alias (__getpid, getpid)
libc_hidden_def (getpid)
