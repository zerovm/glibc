
#include <dirent.h>
#include <stddef.h>
#include <string.h>

#include <irt_syscalls.h>

#define __READDIR __readdir
#define __GETDENTS __getdents
#define DIRENT_TYPE struct dirent
#define __READDIR_ALIAS

#include <sysdeps/unix/readdir.c>
