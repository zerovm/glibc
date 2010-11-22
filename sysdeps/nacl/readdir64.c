
#include <dirent.h>
#include <stddef.h>
#include <string.h>

#include <nacl_dirent.h>
#include <nacl_syscalls.h>

#define __READDIR __readdir64
#define __GETDENTS nacl_getdents_wrapper64
#define DIRENT_TYPE struct dirent64

#include <sysdeps/nacl/nacl_getdents_wrapper.c>
#include <sysdeps/unix/readdir.c>

weak_alias (__readdir64, readdir64)

/* Export compatibility symbol since it is in use by other compatibility code
   (such as glob). There is no NaCl binaries linked against glibc version that
   old. We may crash here. TODO(pasko): eliminate other pre-GLIBC2.9
   compatibility code.  */
void __old_readdir64() {
  abort ();
}

compat_symbol (libc, __old_readdir64, readdir64, GLIBC_2_1);
