#include <errno.h>
#include <stdio.h>
#include <ucontext.h>

int swapcontext (ucontext_t *oucp, const ucontext_t *ucp)
{
  errno = ENOSYS;
  perror ("swapcontext:");
  abort ();
}