
/* The purpose of this file is to be #included by generic readdir
   implementations.  */

static const int d_name_shift = offsetof (DIRENT_TYPE, d_name) -
    offsetof (struct nacl_abi_dirent, nacl_abi_d_name);

/* Calls NACL_SYSCALL(getdents) and converts resulting buffer to glibc abi.
   This wrapper is required since glibc abi for DIRENT_TYPE differs from
   struct nacl_abi_dirent. */
__ssize_t internal_function __GETDENTS (int fd, char *buf, size_t buf_size)
{
  /* NACL_SYSCALL (getdents) fills buffer with overlapped structures
     nacl_abi_dirent. Right after d_reclen bytes of one structure end the next
     structure begins, and so on. For example if nacl_abi_dirent contains 14
     bytes long string in d_name field then it will occupy 10+14 bytes in the
     buffer. This wrapper fills buf so that every DIRENT_TYPE occupies in it
     one byte more than corresponding nacl_abi_dirent in buffer filled by nacl
     syscall. To avoid overwhelming of buf it is necessary to make nacl_buf
     smaller. It is ok to make nacl_buf_size equal buf_size * 0.9 because
     minimal size of nacl_abi_dirent is 12 bytes. */
  int nacl_buf_size = buf_size - buf_size / 10 - 1;
  char nacl_buf[nacl_buf_size];
  int nbytes = NACL_SYSCALL (getdents) (fd, nacl_buf, nacl_buf_size);
  struct nacl_abi_dirent *nacl_dp;
  DIRENT_TYPE *dp;
  int nacl_offset = 0;
  int offset = 0;
  int d_name_len;
  if (nbytes <= 0)
    {
      return nbytes;
    }
  while (nacl_offset < nbytes)
    {
      nacl_dp = (struct nacl_abi_dirent *) (nacl_buf + nacl_offset);
      dp = (DIRENT_TYPE *) (buf + offset);
      if ((offset + nacl_dp->nacl_abi_d_reclen + d_name_shift) >= buf_size)
        {
          errno = EINVAL;
          return -1;
        }
      dp->d_ino = nacl_dp->nacl_abi_d_ino;
      dp->d_off = nacl_dp->nacl_abi_d_off;
      dp->d_reclen = nacl_dp->nacl_abi_d_reclen + d_name_shift;
      dp->d_type = 0;
      d_name_len =  nacl_dp->nacl_abi_d_reclen -
          offsetof (struct nacl_abi_dirent, nacl_abi_d_name);
      memcpy (dp->d_name, nacl_dp->nacl_abi_d_name, d_name_len);
      offset += dp->d_reclen;
      nacl_offset += nacl_dp->nacl_abi_d_reclen;
    }
  return offset;
}
