
#include <sysdep.h>
#include <unistd.h>


/* Return the system page size. */
int __getpagesize ()
{
  /* NaCl does not have a syscall for getting the page size.  The page
     size is fixed at 64k for compatiblity with Windows. */
  return 0x10000;
}
libc_hidden_def (__getpagesize)
weak_alias (__getpagesize, getpagesize)
