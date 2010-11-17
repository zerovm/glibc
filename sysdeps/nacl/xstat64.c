
#include <errno.h>
#include <sys/stat.h>

#include <nacl_stat.h>
#include <nacl_syscalls.h>

/* stat64 is always supported on Native Client */
int __have_no_stat64 = 0;

int __xstat64 (int version, const char *path, struct stat64 *buf)
{
  if (buf == NULL || path == NULL)
    {
      errno = EFAULT;
      return -1;
    }
  struct nacl_abi_stat st;
  int result = NACL_SYSCALL (stat) (path, &st);
  if (result < 0)
    {
      errno = -result;
      return -1;
    }
  else
    {
      __nacl_abi_stat_to_stat64 (&st, buf);
      return 0;
    }
}
libc_hidden_def (__xstat64)
