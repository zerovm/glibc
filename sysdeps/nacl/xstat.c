
#include <errno.h>
#include <sys/stat.h>

#include <nacl_stat.h>
#include <irt_syscalls.h>

int __xstat (int version, const char *path, struct stat *buf)
{
  return __nacl_irt_stat (path, buf);
}
libc_hidden_def (__xstat)
#ifdef SHARED
extern __typeof (__xstat64) __xstat64 __attribute__ ((alias ("__GI___xstat")));
#endif
