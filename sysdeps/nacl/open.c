
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>

#include <nacl_syscalls.h>


int __open (const char *filename, int flags, ...)
{
  int mode = 0;
  if(flags & O_CREAT) {
    va_list arg;
    va_start(arg, flags);
    mode = va_arg(arg, int);
    va_end(arg);
  }

  int result = NACL_SYSCALL (open) (filename, flags, mode);
  if (result < 0) {
    errno = -result;
    return -1;
  }
  return result;
}
libc_hidden_def (__open)
weak_alias (__open, open)
strong_alias (__open, __libc_open)
strong_alias (__open, __open_nocancel)
