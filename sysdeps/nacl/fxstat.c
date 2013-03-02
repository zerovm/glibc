
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>

#include <kernel_stat.h>
#include <nacl_stat.h>
#include <irt_syscalls.h>

int __fxstat (int vers, int fd, struct stat *buf)
{
    return __nacl_irt_fstat (fd, buf);
}
hidden_def(__fxstat)
#ifdef SHARED
extern __typeof (__fxstat64) __fxstat64 __attribute__ ((alias ("__GI___fxstat")));
#endif
