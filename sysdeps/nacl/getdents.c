
#include <dirent.h>
#include <stddef.h>
#include <string.h>

#include <irt_syscalls.h>

#define __GETDENTS __getdents
#define DIRENT_TYPE struct dirent

__ssize_t internal_function __getdents (int fd, char *buf, size_t buf_size)
{
  size_t nbytes;
  if( 0 == __nacl_irt_getdents (fd, buf, buf_size, &nbytes) )
      return nbytes;
  else
      return -1;
}
