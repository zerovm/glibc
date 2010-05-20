
#include <errno.h>

int __unimplemented_syscall(const char *name)
{
  errno = ENOSYS;
  return -1;
}
