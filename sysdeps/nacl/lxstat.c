#include <errno.h>
#include <sys/stat.h>

int __lxstat (int vers, const char *name, struct stat *buf)
{
  errno = ENOSYS;
  return -1;
}
hidden_def (__lxstat)
weak_alias (__lxstat, _lxstat);
#ifdef SHARED
extern __typeof (__lxstat64) __lxstat64 __attribute__ ((alias ("__GI___lxstat")));
#endif