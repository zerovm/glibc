#include <errno.h>
#include <stdio.h>
#include <ucontext.h>

int getcontext (ucontext_t *ucp)
{
  errno = ENOSYS;
  perror ("getcontext:");
  abort ();
}