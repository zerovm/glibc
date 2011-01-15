
#include <errno.h>

#include <nacl_syscalls.h>


ssize_t imc_recvmsg (int fd, struct NaClImcMsgHdr *msg, int flags)
{
  int retval = NACL_SYSCALL (imc_recvmsg) (fd, msg, flags);
  if (retval < 0) {
    errno = -retval;
    return -1;
  }
  return retval;
}
