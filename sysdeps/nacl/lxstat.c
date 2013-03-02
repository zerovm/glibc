#include <errno.h>
#include <sys/stat.h>

int __lxstat (int vers, const char *name, struct stat *buf)
{
    return __xstat (vers, name, buf);
}
hidden_def (__lxstat)
weak_alias (__lxstat, _lxstat);
#ifdef SHARED
extern __typeof (__lxstat64) __lxstat64 __attribute__ ((alias ("__GI___lxstat")));
#endif
