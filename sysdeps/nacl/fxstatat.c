#include <io/fxstatat.c>

#ifdef SHARED
extern __typeof (__fxstatat64) __fxstatat64 __attribute__ ((alias ("__GI___fxstatat")));
#endif
