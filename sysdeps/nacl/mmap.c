
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>

#include <nacl_syscalls.h>


__ptr_t __mmap (__ptr_t addr, size_t len, int prot, int flags,
		int fd, off_t offset)
{
  void *(*nacl_mmap)(__ptr_t addr, size_t len, int prot, int flags,
		     int fd, off_t offset) = NACL_SYSCALL_ADDR(NACL_sys_mmap);
  void *result = nacl_mmap(addr, len, prot, flags, fd, offset);
  if ((unsigned int) result > 0xfffff000u) {
    errno = -(int) result;
    return -1;
  }
  return result;
}
weak_alias (__mmap, mmap)
