
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>

#include <kernel_stat.h>
#include <nacl_stat.h>
#include <nacl_syscalls.h>


int __fxstat64 (int vers, int fd, struct stat64 *buf)
{
  struct nacl_abi_stat nacl_buf;
  int result = NACL_SYSCALL (fstat) (fd, &nacl_buf);
  if (result < 0) {
    errno = -result;
    return -1;
  }
  buf->st_dev = nacl_buf.nacl_abi_st_dev;
#ifdef _HAVE_STAT64___PAD1
  buf->__pad1 = 0;
#endif
#ifdef _HAVE_STAT64___ST_INO
  buf->__st_ino = nacl_buf.nacl_abi_st_ino;
#endif
  buf->st_mode = nacl_buf.nacl_abi_st_mode;
  buf->st_nlink = nacl_buf.nacl_abi_st_nlink;
  buf->st_uid = nacl_buf.nacl_abi_st_uid;
  buf->st_gid = nacl_buf.nacl_abi_st_gid;
  buf->st_rdev = nacl_buf.nacl_abi_st_rdev;
#ifdef _HAVE_STAT64___PAD2
  buf->__pad2 = 0;
#endif
  buf->st_size = nacl_buf.nacl_abi_st_size;
  buf->st_blksize = nacl_buf.nacl_abi_st_blksize;
  buf->st_blocks = nacl_buf.nacl_abi_st_blocks;
  buf->st_atim.tv_sec = nacl_buf.nacl_abi_st_atime;
  buf->st_atim.tv_nsec = 0;
  buf->st_mtim.tv_sec = nacl_buf.nacl_abi_st_mtime;
  buf->st_mtim.tv_nsec = 0;
  buf->st_ctim.tv_sec = nacl_buf.nacl_abi_st_ctime;
  buf->st_ctim.tv_nsec = 0;
  buf->st_ino = nacl_buf.nacl_abi_st_ino;
  return result;
}
hidden_def (__fxstat64)
