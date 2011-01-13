
#include <errno.h>

#include <nacl_syscalls.h>


loff_t __llseek (int fd, loff_t offset, int whence)
{
  int result = NACL_SYSCALL (lseek) (fd, &offset, whence);
  if (result < 0)
    {
      errno = -result;
      return -1;
    }
  return offset;
}
weak_alias (__llseek, llseek)
strong_alias (__llseek, __libc_lseek64)
strong_alias (__llseek, __lseek64)
weak_alias (__llseek, lseek64)
