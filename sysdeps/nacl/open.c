#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <sysdep.h>

int __open (const char *filename, int flags, ...)
{
  int newfd;
  int mode = 0;

  if(flags & O_CREAT) {
    va_list arg;
    va_start(arg, flags);
    mode = va_arg(arg, int);
    va_end(arg);
  }

  return INLINE_SYSCALL (open, 3, filename, flags, mode);
}
libc_hidden_def (__open)
weak_alias (__open, open)
strong_alias (__open, __libc_open)
strong_alias (__open, __open_nocancel)
