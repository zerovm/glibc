
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>

#include <kernel_stat.h>
#include <nacl_stat.h>
#include <nacl_syscalls.h>


void __nacl_abi_stat_to_stat64 (struct nacl_abi_stat *nacl_st,
                                struct stat64 *st) {
  st->st_dev = nacl_st->nacl_abi_st_dev;
#ifdef _HAVE_STAT64___PAD1
  st->__pad1 = 0;
#endif
#ifdef _HAVE_STAT64___ST_INO
  st->__st_ino = nacl_st->nacl_abi_st_ino;
#endif
  st->st_mode = nacl_st->nacl_abi_st_mode;
  st->st_nlink = nacl_st->nacl_abi_st_nlink;
  st->st_uid = nacl_st->nacl_abi_st_uid;
  st->st_gid = nacl_st->nacl_abi_st_gid;
  st->st_rdev = nacl_st->nacl_abi_st_rdev;
#ifdef _HAVE_STAT64___PAD2
  st->__pad2 = 0;
#endif
  st->st_size = nacl_st->nacl_abi_st_size;
  st->st_blksize = nacl_st->nacl_abi_st_blksize;
  st->st_blocks = nacl_st->nacl_abi_st_blocks;
  st->st_atim.tv_sec = nacl_st->nacl_abi_st_atime;
  st->st_atim.tv_nsec = 0;
  st->st_mtim.tv_sec = nacl_st->nacl_abi_st_mtime;
  st->st_mtim.tv_nsec = 0;
  st->st_ctim.tv_sec = nacl_st->nacl_abi_st_ctime;
  st->st_ctim.tv_nsec = 0;
  st->st_ino = nacl_st->nacl_abi_st_ino;
}

int __fxstat64 (int vers, int fd, struct stat64 *buf)
{
  if (buf == NULL) {
    errno = EFAULT;
    return -1;
  }
  struct nacl_abi_stat nacl_buf;
  int result = NACL_SYSCALL (fstat) (fd, &nacl_buf);
  if (result < 0) {
    errno = -result;
    return -1;
  }
  __nacl_abi_stat_to_stat64 (&nacl_buf, buf);
  return result;
}
hidden_def (__fxstat64)
