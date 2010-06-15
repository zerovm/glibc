
#include <nacl_syscalls.h>
#include <nacl_tls.h>


/* TODO(mseaborn): Work out what the maximum size of the TLS area
   should be, or change NaCl to use all the address space from the TLS
   pointer up. */
int __nacl_tls_init (void *tls_area)
{
  return NACL_SYSCALL (tls_init) (tls_area, 0x1000);
}
libc_hidden_def (__nacl_dyncode_copy)
