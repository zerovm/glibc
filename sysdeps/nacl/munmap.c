
#include <errno.h>
#include <sys/mman.h>

#include <irt_syscalls.h>


int __munmap (void *start, size_t length)
{
  int result = __nacl_irt_munmap (start, length);
  if (result != 0) {
    errno = result;
    return -1;
  }
  return 0;
}
weak_alias (__munmap, munmap)
