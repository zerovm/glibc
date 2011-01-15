
#include <errno.h>

#include <nacl_syscalls.h>


ssize_t imc_sendmsg (int fd, const struct NaClImcMsgHdr *msg, int flags)
{
  int retval = NACL_SYSCALL (imc_sendmsg) (fd, msg, flags);
  if (retval < 0) {
    errno = -retval;
    return -1;
  }
  return retval;
}
