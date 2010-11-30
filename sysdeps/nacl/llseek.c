
#include <errno.h>

#include <nacl_syscalls.h>


loff_t __llseek (int fd, loff_t offset, int whence)
{
  /* There is no appropriate 64 bit seek syscall in Native Client at the moment.
     So this llseek method simply returns ENOSYS in case of 64 bit offset and
     calls NACL_SYSCALL (lseek) otherwise. */
  if (offset >> 32)
    {
      errno = ENOSYS;
      return -1;
    }
  loff_t res = NACL_SYSCALL (lseek) (fd, offset, whence);
  if (res < 0)
    {
      errno = -res;
      return -1;
    }
  return res;
}
weak_alias (__llseek, llseek)
strong_alias (__llseek, __libc_lseek64)
strong_alias (__llseek, __lseek64)
weak_alias (__llseek, lseek64)
