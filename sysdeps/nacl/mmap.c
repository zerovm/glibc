
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>

#include <irt_syscalls.h>


__ptr_t __mmap (__ptr_t addr, size_t len, int prot, int flags,
		int fd, off_t offset)
{
  int result = __nacl_irt_mmap (&addr, len, prot, flags, fd, offset);
  if (result != 0) {
    return MAP_FAILED;
  }
  return addr;
}
weak_alias (__mmap, mmap)
