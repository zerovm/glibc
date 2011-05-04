#include <errno.h>
#include <pthread.h>

#include <nacl_syscalls.h>


int
pthread_setschedprio (threadid, prio)
     pthread_t threadid;
     int prio;
{
  if (threadid != pthread_self ())
    /* We can only support changing our own priority.  */
    return EPERM;

  return NACL_SYSCALL (thread_nice) (prio);
}
