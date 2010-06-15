
#include <errno.h>
#include <sys/mman.h>

#include <nacl_syscalls.h>


int __munmap (void *start, size_t length)
{
  int result = NACL_SYSCALL (munmap) (start, length);
  if (result < 0) {
    errno = -result;
    return -1;
  }
  return result;
}
weak_alias (__munmap, munmap)
