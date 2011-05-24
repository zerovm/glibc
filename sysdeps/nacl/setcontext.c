#include <errno.h>
#include <stdio.h>
#include <ucontext.h>

int setcontext (const ucontext_t *ucp)
{
  errno = ENOSYS;
  perror ("setcontext:");
  abort ();
}