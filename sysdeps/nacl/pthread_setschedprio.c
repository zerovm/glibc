#include <errno.h>
#include <pthread.h>

#include <irt_syscalls.h>


int
pthread_setschedprio (threadid, prio)
     pthread_t threadid;
     int prio;
{
  if (threadid != pthread_self ())
    /* We can only support changing our own priority.  */
    return EPERM;

  return -__nacl_irt_thread_nice (prio);
}
