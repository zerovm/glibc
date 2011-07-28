
#include <errno.h>
#include <sys/stat.h>

#include <nacl_stat.h>
#include <irt_syscalls.h>

int __xstat (int version, const char *path, struct stat *buf)
{
  if (buf == NULL || path == NULL)
    {
      errno = EFAULT;
      return -1;
    }
  struct nacl_abi_stat st;
  int result = __nacl_irt_stat (path, &st);
  if (result != 0)
    {
      errno = result;
      return -1;
    }
  else
    {
      __nacl_abi_stat_to_stat (&st, buf);
      return 0;
    }
}
libc_hidden_def (__xstat)
#ifdef SHARED
extern __typeof (__xstat64) __xstat64 __attribute__ ((alias ("__GI___xstat")));
#endif
