
#include <nacl_syscalls.h>

#ifdef __x86_64__
void *__nacl_read_tp ()
{
  return NACL_SYSCALL (tls_get) ();
}
#endif
