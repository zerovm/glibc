
#include <errno.h>
#include <sys/stat.h>

#include <nacl_stat.h>
#include <nacl_syscalls.h>

int __xstat (int version, const char *path, struct stat *buf)
{
  if (buf == NULL || path == NULL)
    {
      errno = EFAULT;
      return -1;
    }
  struct nacl_abi_stat st;
  int result = NACL_SYSCALL (stat) (path, &st);
  if (result < 0)
    {
      errno = -result;
      return -1;
    }
  else
    {
      buf->st_dev = st.nacl_abi_st_dev;
#ifdef _HAVE_STAT___PAD1
      buf->__pad1 = 0;
#endif
      buf->st_ino = st.nacl_abi_st_ino;
      buf->st_mode = st.nacl_abi_st_mode;
      buf->st_nlink = st.nacl_abi_st_nlink;
      buf->st_uid = st.nacl_abi_st_uid;
      buf->st_gid = st.nacl_abi_st_gid;
      buf->st_rdev = st.nacl_abi_st_rdev;
#ifdef _HAVE_STAT___PAD2
      buf->__pad2 = 0;
#endif
      buf->st_size = st.nacl_abi_st_size;
      buf->st_blksize = st.nacl_abi_st_blksize;
      buf->st_blocks = st.nacl_abi_st_blocks;
      buf->st_atime = st.nacl_abi_st_atime;
      buf->st_mtime = st.nacl_abi_st_mtime;
      buf->st_ctime = st.nacl_abi_st_ctime;
      return 0;
    }
}
libc_hidden_def (__xstat)
