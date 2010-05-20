
#include <errno.h>
#include <sys/vm86.h>

int vm86 (unsigned long int __subfunction,
          struct vm86plus_struct *__info)
{
  errno = ENOSYS;
  return -1;
}
