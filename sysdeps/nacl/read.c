
#include <errno.h>
#include <unistd.h>
#include <stddef.h>

#include <irt_syscalls.h>


ssize_t __libc_read (int fd, void *buf, size_t size)
{
  size_t nread;
  int result = __nacl_irt_read (fd, buf, size, &nread);
  if (result != 0) {
    errno = -result;
    return -1;
  }
  return nread;
}
libc_hidden_def (__libc_read)
weak_alias (__libc_read, __read)
libc_hidden_weak (__read)
weak_alias (__libc_read, read)
strong_alias (__libc_read, __read_nocancel)
