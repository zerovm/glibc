#include <errno.h>
#include <stdio.h>

int
__pthread_kill (threadid, signo)
     pthread_t threadid;
     int signo;
{
  errno = ENOSYS;
  perror ("pthread_kill");
  abort ();

  /* Cannot reach this point.  */
  return -1;
}
strong_alias (__pthread_kill, pthread_kill)
stub_warning (pthread_kill)
