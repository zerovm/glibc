#include <errno.h>
#include <stdio.h>
#include <ucontext.h>

void makecontext (ucontext_t *ucp, (void *func)(), int argc, ...)
{
  errno = ENOSYS;
  perror ("makecontext");
  abort ();
}
