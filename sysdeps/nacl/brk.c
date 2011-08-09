
#include <errno.h>
#include <unistd.h>
#include <sysdep.h>

#include <irt_syscalls.h>


void *__curbrk = 0;
weak_alias (__curbrk, ___brk_addr)

int __brk (void *addr)
{
  int rv = __nacl_irt_sysbrk (&addr);
  if (rv != 0) {
    errno = rv;
    return -1;
  }
  if (__curbrk > addr) {
    errno = ENOMEM;
    return -1;
  }
  __curbrk = addr;
  return 0;
}
weak_alias (__brk, brk)
