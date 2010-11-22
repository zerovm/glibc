
#include <dirent.h>
#include <stddef.h>
#include <string.h>

#include <nacl_dirent.h>
#include <nacl_syscalls.h>

#define __READDIR __readdir
#define __GETDENTS nacl_getdents_wrapper
#define DIRENT_TYPE struct dirent
#define __READDIR_ALIAS

#include <sysdeps/nacl/nacl_getdents_wrapper.c>
#include <sysdeps/unix/readdir.c>
