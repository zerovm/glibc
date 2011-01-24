
#include <errno.h>

#include <nacl_syscalls.h>


int imc_accept (int fd)
{
  int retval = NACL_SYSCALL (imc_accept) (fd);
  if (retval < 0) {
    errno = -retval;
    return -1;
  }
  return retval;
}
