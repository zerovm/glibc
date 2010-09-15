
#include <nacl_syscalls.h>
#include <nacl_tls.h>
#include "pthreadP.h"

int __nacl_tls_init (void *tls_area)
{
  return NACL_SYSCALL (tls_init) (tls_area, sizeof(struct pthread));
}
libc_hidden_def (__nacl_dyncode_copy)
