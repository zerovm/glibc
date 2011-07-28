
#include <irt_syscalls.h>

#ifdef __x86_64__
void* __attribute__ ((const)) __nacl_read_tp ()
{
  return __nacl_irt_tls_get ();
}
#endif
