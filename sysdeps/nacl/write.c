
#include <errno.h>
#include <unistd.h>
#include <sysdep.h>

#include <irt_syscalls.h>


ssize_t __write(int desc, void const *buf, size_t count)
{
  size_t nwrite;
  int result = __nacl_irt_write (desc, buf, count, &nwrite);
  if (result != 0) 
      return -1;
  else
      return nwrite;
}
libc_hidden_def (__write)
weak_alias (__write, write)
strong_alias (__write, __libc_write)
libc_hidden_def (__libc_write)
strong_alias (__write, __write_nocancel)
