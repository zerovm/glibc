#ifndef _NACL_SYSDEP_H
#define _NACL_SYSDEP_H 1

#if !defined(__ASSEMBLER__)
#include <futex_emulation.h>
#include <irt_syscalls.h>
#include <lowlevellock.h>

/* Implementation of all syscalls for use in platform- and OS- independent code
   as inline functions.  Each function translates the syscall arguments into IRT
   arguments and allows to intercept each call in user code.
   TODO(khim): implement the interception logic.  */

#define INTERNAL_SYSCALL(name, err, nr, args...) \
  INTERNAL_SYSCALL_ ## name ## _ ## nr (&err , ## args)

#define INTERNAL_SYSCALL_NCS(name, err, nr, args...) \
  INTERNAL_SYSCALL_NCS_ ## nr (name, &err , ## args)

__extern_always_inline int
INTERNAL_SYSCALL_NCS_3 (int syscall_nr, int *err, int id1, int id2, int id3)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_access_2 (int *err, const char *pathname, int mode)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_chmod_2 (int *err, const char *path, mode_t mode)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_chown_3 (int *err, const char *path,
			  __kernel_uid_t owner, __kernel_gid_t group)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_chown32_3 (int *err, const char *path,
			    uid_t owner, gid_t group)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_clock_getres_2 (int *err, clockid_t clk_id,
				 struct timespec *res)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_clock_gettime_2 (int *err, clockid_t clk_id,
				  struct timespec *tp)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_clock_nanosleep_4 (int *err, clockid_t clock_id, int flags,
				    const struct timespec *request,
				    struct timespec *remain)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_clock_settime_2 (int *err, clockid_t clk_id,
				  const struct timespec *tp)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_close_1 (int *err, unsigned int fd)
{
  *err = __nacl_irt_close(fd);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_epoll_pwait_6 (int *err, int epfd, struct epoll_event *events,
				int maxevents, int timeout,
				const sigset_t *sigmask, size_t sigsetsize)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_execve_3 (int *err, const char *filename, char *const argv[],
			   char *const envp[])
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_eventfd2_2 (int *err, unsigned int initval, int flags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_eventfd_1 (int *err, unsigned int initval)
{
  return INTERNAL_SYSCALL_eventfd2_2(err, initval, 0);
}

__extern_always_inline int
INTERNAL_SYSCALL_faccessat_3 (int *err, int dirfd, const char *pathname,
			      int mode)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_fchmod_2 (int *err, int fd, mode_t mode)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_fchmodat_3 (int *err, int dirfd, const char *pathname,
			     mode_t mode)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_fchown_3 (int *err, int fd,
			  __kernel_uid_t owner, __kernel_gid_t group)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_fchown32_3 (int *err, int fd,
			    uid_t owner, gid_t group)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_fchownat_5 (int *err, int dirfd, const char *pathname,
			     uid_t owner, gid_t group, int flags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_fcntl_3 (int *err, int fd, int cmd, void *arg)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_fcntl64_3 (int *err, int fd, int cmd,
			    union __attribute__ ((__transparent_union__))
			    {
			      unsigned long long ullarg;
			      long long llarg;
			      unsigned long ularg;
			      long larg;
			      unsigned int uiarg;
			      int iarg;
			      void *parg;
			    } arg)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__errordecl (__futex_emulation_unknown_operation,
	     "unknown futex operation or argument");

struct __attribute__ ((__packed__)) decoded_futex_operation
  {
    unsigned int operation	:7;
    unsigned int private	:1;
    unsigned int realtime	:1;
    unsigned int padding	:23;
};

union __attribute__ ((__transparent_union__)) encoded_futex_operation
  {
    unsigned int encoded_futex_operation;
    int const_futex_operation;
    struct decoded_futex_operation decoded_futex_operation;
  };

__extern_always_inline void
INTERNAL_SYSCALL_futex_2 (int *err,
			  union __attribute__ ((__transparent_union__))
			  {
			    volatile int *addr;
			    volatile unsigned int *uaddr;
			  } addr,
			  union encoded_futex_operation futex_operation)
{
  switch (futex_operation.decoded_futex_operation.operation)
    {
      case FUTEX_UNLOCK_PI:
	*err = (38 /* ENOSYS */);
	return;
      default:
	if (__builtin_constant_p (
	    futex_operation.decoded_futex_operation.operation))
	  __futex_emulation_unknown_operation ();
	*err = 0;
    }
}

__extern_always_inline unsigned int
INTERNAL_SYSCALL_futex_3 (int *err,
			  union __attribute__ ((__transparent_union__))
			  {
			    volatile int *addr;
			    volatile unsigned int *uaddr;
			  } addr,
			  union encoded_futex_operation futex_operation,
			  int val)
{
  int count;
  switch (futex_operation.decoded_futex_operation.operation)
    {
      case FUTEX_WAKE:
	*err = __nacl_futex_wake (addr.addr, val, __FUTEX_BITSET_MATCH_ANY,
				    &count);
	return count;
      default:
	if (__builtin_constant_p (
	    futex_operation.decoded_futex_operation.operation))
	  __futex_emulation_unknown_operation ();
	*err = 0;
    }
    return 0;
}

__extern_always_inline unsigned int
INTERNAL_SYSCALL_futex_4 (int *err,
			  union __attribute__ ((__transparent_union__))
			  {
			    volatile int *addr;
			    volatile unsigned int *uaddr;
			  } addr,
			  union encoded_futex_operation futex_operation,
			  int val1,
			  /* We need to accept 0 here.  May be better to
			     replace 0 with NULL in NPTL code?  */
			  union __attribute__ ((__transparent_union__))
			    {
			      const struct timespec *timeout;
			      int null_timeout;
			    } val2)
{
  switch (futex_operation.decoded_futex_operation.operation)
    {
      case FUTEX_WAIT:
	*err = __nacl_futex_wait (addr.addr, val1, __FUTEX_BITSET_MATCH_ANY,
				    val2.timeout);
	return 0;
      /* TODO(khim): hook up PI-locks to some IRT-handleable functions.  */
      case FUTEX_LOCK_PI:
      case FUTEX_UNLOCK_PI:
      case FUTEX_TRYLOCK_PI:
	*err = (38 /* ENOSYS */);
	return 0;
      default:
	if (__builtin_constant_p (
	    futex_operation.decoded_futex_operation.operation))
	  __futex_emulation_unknown_operation ();
	*err = 0;
    }
    return 0;
}

__extern_always_inline unsigned int
INTERNAL_SYSCALL_futex_6 (int *err,
			  union __attribute__ ((__transparent_union__))
			  {
			    volatile int *addr;
			    volatile unsigned int *uaddr;
			  } addr1,
			  union encoded_futex_operation futex_operation,
			  int val1,
			  int val2,
			  union __attribute__ ((__transparent_union__))
			  {
			    volatile int *addr;
			    volatile unsigned int *uaddr;
			  } addr2,
			  int val3)
{
  int count;
  switch (futex_operation.decoded_futex_operation.operation)
    {
      case FUTEX_WAKE_OP:
	*err = __nacl_futex_wake_op (addr1.addr, addr2.addr,
				       val1, val2, val3, &count);
	return count;
      case FUTEX_CMP_REQUEUE:
	*err = __nacl_futex_cmp_requeue (addr1.addr, addr2.addr,
					   val1, val2, val3, &count);
	return count;
      default:
	if (__builtin_constant_p (
	    futex_operation.decoded_futex_operation.operation))
	  __futex_emulation_unknown_operation ();
	*err = 0;
    }
    return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_futimesat_3 (int *err, int dirfd, const char *pathname,
			      const struct timeval times[2])
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_getcpu_3 (int *err, unsigned *cpu, unsigned *node,
			   struct getcpu_cache *tcache)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

/* NOTE! The user-level library version returns a character pointer.

   The system call just returns the length of the buffer filled (which includes
   the ending '\0' character), or zero in case of error.  */
__extern_always_inline int
INTERNAL_SYSCALL_getcwd_2 (int *err, char *buf, size_t size)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline __kernel_gid_t
INTERNAL_SYSCALL_getegid_0 (int *err)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline gid_t
INTERNAL_SYSCALL_getegid32_0 (int *err)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline __kernel_uid_t
INTERNAL_SYSCALL_geteuid_0 (int *err)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline uid_t
INTERNAL_SYSCALL_geteuid32_0 (int *err)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline __kernel_gid_t
INTERNAL_SYSCALL_getgid_0 (int *err)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline gid_t
INTERNAL_SYSCALL_getgid32_0 (int *err)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_getgroups_2 (int *err, int size, __kernel_gid_t *list)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_getgroups32_2 (int *err, int size, gid_t *list)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_getmsg_4 (int *err, int fildes, const struct strbuf *ctlptr,
			   const struct strbuf *dataptr, int *pflags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_getpmsg_5 (int *err, int fildes, const struct strbuf *ctlptr,
			   const struct strbuf *dataptr, int *pband, int *pflags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

/* The return value of getpriority syscall is biased by this value
   to avoid returning negative values.  */
__extern_always_inline int
INTERNAL_SYSCALL_getpriority_2 (int *err, int which, int who)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_getresgid_3 (int *err, __kernel_gid_t *rgid,
			      __kernel_gid_t *egid, __kernel_gid_t *sgid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_getresgid32_3 (int *err, gid_t *rgid, gid_t *egid, gid_t *sgid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_getresuid_3 (int *err, __kernel_uid_t *ruid,
			      __kernel_uid_t *euid, __kernel_uid_t *suid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_getresuid32_3 (int *err, uid_t *ruid, uid_t *euid, uid_t *ugid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline __kernel_uid_t
INTERNAL_SYSCALL_getuid_0 (int *err)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline uid_t
INTERNAL_SYSCALL_getuid32_0 (int *err)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline uid_t
INTERNAL_SYSCALL_open_3 (int *err, const char *pathname, int flags, mode_t mode)
{
  int newfd;
  *err = __nacl_irt_open (pathname, flags, mode, &newfd);
  return newfd;
}

__extern_always_inline uid_t
INTERNAL_SYSCALL_openat_4 (int *err, int dirfd, const char *pathname,
			   int flags, mode_t mode)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_ioctl_3 (int *err, int d, int request,
			  union __attribute__ ((__transparent_union__))
			  {
			    unsigned long long ullarg;
			    long long llarg;
			    unsigned long ularg;
			    long larg;
			    unsigned int uiarg;
			    int iarg;
			    void *parg;
			  } arg)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_ipc_6 (int *err, unsigned int call, int first, int second,
			int third, void *ptr, long fifth)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_ipc_5 (int *err, unsigned int call, int first, int second,
			int third, void *ptr)
{
  return INTERNAL_SYSCALL_ipc_6 (err, call, first, second, third, ptr, 0);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_kill_2 (int *err, pid_t pid, int sig)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_laccess_2 (int *err, const char *pathname, int mode)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_lchown_3 (int *err, const char *path,
			  __kernel_uid_t owner, __kernel_gid_t group)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_lchown32_3 (int *err, const char *path,
			    uid_t owner, gid_t group)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_link_2 (int *err, const char *oldpath, const char *newpath)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_linkat_5 (int *err, int olddfd, const char *oldname,
			   int newdfd, const char *newname, int flags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_madvise_3 (int *err, void *addr, size_t length, int advice)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_mkdir_2 (int *err, const char *pathname, mode_t mode)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_mkdirat_3 (int *err, int dirfd, const char *pathname,
			    mode_t mode)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_mknod_3 (int *err, const char *pathname,
			  mode_t mode, dev_t dev)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_mknodat_4 (int *err, int dirfd, const char *pathname,
			    mode_t mode, dev_t dev)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

struct mq_attr;
__extern_always_inline int/*mqd_t*/
INTERNAL_SYSCALL_mq_open_4 (int *err, const char *name, int oflag, mode_t mode,
			    struct mq_attr *attr)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_mq_notify_2 (int *err, int/*mqd_t*/ mqdes,
			      const struct sigevent *notification)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_mq_unlink_1 (int *err, const char *name)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_munmap_2 (int *err, void *addr, size_t length)
{
  *err = __nacl_irt_munmap(addr, length);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_poll_3 (int *err, struct pollfd *fds, nfds_t nfds, int timeout)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_ppoll_5 (int *err, struct pollfd *fds, nfds_t nfds,
			  const struct timespec *timeout,
			  const sigset_t *sigmask, size_t sigsetsize)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline long
INTERNAL_SYSCALL_ptrace_4 (int *err, enum __ptrace_request request, pid_t pid,
			   void *addr, void *data)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_putmsg_4 (int *err, int fildes, const struct strbuf *ctlptr,
			   const struct strbuf *dataptr, int flags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_putpmsg_5 (int *err, int fildes, const struct strbuf *ctlptr,
			   const struct strbuf *dataptr, int band, int flags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_readahead_4 (int *err, int fd,
			      __LONG_LONG_PAIR(long offset_high,
					       long offset_low), size_t count)
{
#if 0
  off64_t offset = ((off64_t)offset_high) << 32 | offset_low;
#endif
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline ssize_t
INTERNAL_SYSCALL_readv_3 (int *err, int fd, const
			  struct iovec *iov, int iovcnt)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline ssize_t
INTERNAL_SYSCALL_readlink_3 (int *err, const char *path,
			     char *buf, size_t bufsiz)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline ssize_t
INTERNAL_SYSCALL_readlinkat_4 (int *err, int dirfd, const char *pathname,
			       char *buf, size_t bufsiz)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_reboot_4 (int *err, int magic1, int magic2, int cmd, void *arg)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_reboot_3 (int *err, int magic1, int magic2, int howto)
{
  return INTERNAL_SYSCALL_reboot_4 (err, magic1, magic2, howto, NULL);
}

__extern_always_inline int
INTERNAL_SYSCALL_rename_2 (int *err, const char *oldpath, const char *newpath)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_renameat_4 (int *err, int olddfd, const char *oldname,
			     int newdfd, const char *newname)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_rmdir_1 (int *err, const char *pathname)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_rt_sigpending_2(int *err, sigset_t *set, size_t sigsetsize)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_rt_sigprocmask_4 (int *err, int how, const sigset_t *set,
				   sigset_t *oldset, size_t sigsetsize)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_rt_sigqueueinfo_3 (int *err, pid_t pid, int sig,
				    siginfo_t *info)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_rt_sigsuspend_2 (int *err, const sigset_t *mask,
				  size_t sigsetsize)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_rt_sigtimedwait_4 (int *err, const sigset_t *set,
				    siginfo_t *info,
				    const struct timespec *timeout,
				    size_t sigsetsize)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_sched_getaffinity_3 (int *err, pid_t pid, size_t cpusetsize,
				      cpu_set_t *mask)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_sched_getparam_2 (int *err, pid_t pid,
				   struct sched_param *param)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_sched_get_priority_min_1 (int *err, int policy)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_sched_get_priority_max_1 (int *err, int policy)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_sched_getscheduler_1 (int *err, pid_t pid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_sched_setaffinity_3 (int *err, pid_t pid, size_t cpusetsize,
				      const cpu_set_t *mask)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_sched_setparam_2 (int *err, pid_t pid,
				   struct sched_param *param)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_sched_setscheduler_3 (int *err, pid_t pid, int policy,
				       const struct sched_param *param)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setfsgid_1 (int *err, __kernel_gid_t gid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setfsgid32_1 (int *err, gid_t gid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setfsuid_1 (int *err, __kernel_uid_t uid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setfsuid32_1 (int *err, uid_t uid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setgid_1 (int *err, __kernel_gid_t gid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setgid32_1 (int *err, gid_t gid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setgroups_2 (int *err, int size, const __kernel_gid_t *list)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setgroups32_2 (int *err, int size, const gid_t *list)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setpriority_3 (int *err, int which, int who, int prio)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setregid_2 (int *err, __kernel_gid_t rgid, __kernel_gid_t egid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setregid32_2 (int *err, gid_t rgid, gid_t egid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setresgid_3 (int *err, __kernel_gid_t rgid,
			      __kernel_gid_t egid, __kernel_gid_t sgid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setresgid32_3 (int *err, gid_t rgid, gid_t egid, gid_t sgid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setresuid_3 (int *err, __kernel_uid_t ruid,
			      __kernel_uid_t euid, __kernel_uid_t suid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setresuid32_3 (int *err, uid_t ruid, uid_t euid, uid_t suid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setreuid_2 (int *err, __kernel_uid_t ruid, __kernel_uid_t euid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setreuid32_2 (int *err, uid_t ruid, uid_t euid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_set_robust_list_2 (int *err, struct robust_list_head *head,
				    size_t len)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setuid_1 (int *err, __kernel_uid_t uid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_setuid32_1 (int *err, uid_t uid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_signalfd4_4 (int *err, int fd, const sigset_t *mask,
			      size_t sigsetsize, int flags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_signalfd_3 (int *err, int fd, const sigset_t *mask,
			     size_t sigsetsize)
{
  return INTERNAL_SYSCALL_signalfd4_4 (err, fd, mask, sigsetsize, 0);
}

__extern_always_inline int
INTERNAL_SYSCALL_symlink_2 (int *err, const char *oldpath, const char *newpath)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_symlinkat_3 (int *err, const char *oldpath,
			      int newdirfd, const char *newpath)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_sync_file_range_6 (int *err, int fd,
				    __LONG_LONG_PAIR(long offset_high,
						     long offset_low),
				    __LONG_LONG_PAIR(long nbytes_high,
						     long nbytes_low),
				    unsigned int flags)
{
#if 0
  off64_t offset = ((off64_t)offset_high) << 32 | offset_low;
  off64_t nbytes = ((off64_t)nbytes_high) << 32 | nbytes_low;
#endif
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL__sysctl_1 (int *err, struct __sysctl_args *args)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_tgkill_3 (int *err, gid_t tgid, pid_t tid, int sig)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_timer_create_3 (int *err, clockid_t clockid,
				 struct sigevent *evp,
				 __kernel_timer_t *timerid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_timer_delete_1 (int *err, __kernel_timer_t timerid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_timer_getoverrun_1 (int *err, __kernel_timer_t timerid)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_timer_gettime_2 (int *err, __kernel_timer_t timerid,
				  struct itimerspec *curr_value)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_timer_settime_4 (int *err, __kernel_timer_t timerid, int flags,
				  const struct itimerspec *new_value,
				  struct itimerspec * old_value)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline clock_t
INTERNAL_SYSCALL_times_1 (int *err, struct tms *buf)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_tkill_2 (int *err, pid_t tid, int sig)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_unlink_1 (int *err, const char *pathname)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_unlinkat_3 (int *err, int dirfd, const char *pathname,
			     int flags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_ustat_2 (int *err, dev_t dev, struct ustat *ubuf)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_utime_2 (int *err, const char *filename,
		          const struct utimbuf *times)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_utimes_2 (int *err, const char *filename,
			   const struct timeval times[2])
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline int
INTERNAL_SYSCALL_utimensat_4 (int *err, int dirfd, const char *pathname,
			      const struct timespec times[2], int flags)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline pid_t
INTERNAL_SYSCALL_wait3_3 (int *err, int *status, int options,
			  struct rusage *rusage)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline pid_t
INTERNAL_SYSCALL_wait4_4 (int *err, pid_t pid, int *status, int options,
			  struct rusage *rusage)
{
  *err = (38 /* ENOSYS */);
  return 0;
}

__extern_always_inline ssize_t
INTERNAL_SYSCALL_write_3 (int *err, int fd, const void *buf, size_t count)
{
  size_t nwrote;
  *err = __nacl_irt_write (fd, buf, count, &nwrote);
  return nwrote;
}

__extern_always_inline ssize_t
INTERNAL_SYSCALL_writev_3 (int *err, int fd, const
			   struct iovec *iov, int iovcnt)
{
  *err = (38 /* ENOSYS */);
  return 0;
}
#endif

#include_next <sysdep.h>

#if !defined(__ASSEMBLER__) && \
     defined(THREAD_SELF) && defined(CLOCK_IDFIELD_SIZE)
#include <tls.h>

/* We don't support redefinition of this function. We always use 30 bits of the
   address of pthread structure as TID.  */
__extern_always_inline pid_t
INTERNAL_SYSCALL_gettid_0 (int *err)
{
  *err = 0;
  /* Native Client does not have a notion of a thread ID, so we make
     one up.  This must be small enough to leave space for number
     identifying the clock.  Use CLOCK_IDFIELD_SIZE to guarantee
     that.  */
  return ((unsigned int) THREAD_SELF) >> CLOCK_IDFIELD_SIZE;
}

/* We can not return error code - this function is not supposed to fail.
   It should return pid (which is equal to tid for the main process).
   It's only ever called from main process so we just return TID.  */
__extern_always_inline int
INTERNAL_SYSCALL_set_tid_address_1 (int *err, volatile int *tidptr)
{
  return INTERNAL_SYSCALL_gettid_0 (err);
}
#endif

#undef INTERNAL_SYSCALL_DECL
#define INTERNAL_SYSCALL_DECL(err) int err

#undef INTERNAL_SYSCALL_ERRNO
#define INTERNAL_SYSCALL_ERRNO(val, err) (err)

#undef INTERNAL_SYSCALL_ERROR_P
#define INTERNAL_SYSCALL_ERROR_P(val, err) (err)

/* Define a macro which expands inline into the wrapper code for a IRT
   call.  */
# undef INLINE_SYSCALL
# define INLINE_SYSCALL(name, nr, args...) \
  ({									      \
    INTERNAL_SYSCALL_DECL (err);					      \
    unsigned long int resultvar = INTERNAL_SYSCALL (name, err, nr , ## args); \
    if (__builtin_expect (err, 0))					      \
      {									      \
	__set_errno (err);						      \
	resultvar = (unsigned long int) -1;				      \
      }									      \
    (long int) resultvar; })

#undef INTERNAL_VSYSCALL
#define INTERNAL_VSYSCALL INTERNAL_SYSCALL

/* Nacl uses AT_SYSINFO for IRT. Thus sysinfo can never be usable
   and it's safe to disable this machinery.  */
#undef NEED_DL_SYSINFO

#undef __NR_fallocate
#undef __NR_pselect6
#undef __NR_rt_sigaction

#ifndef __x86_64__
#undef	PSEUDO
#define	PSEUDO(name, syscall_name, args)	\
  .text;					\
  ENTRY (name)					\
    mov $-38, %eax;				\
  L(pseudo_end):				\
    NACLRET

#undef	PSEUDO_END
#define	PSEUDO_END(name)			\
    SYSCALL_ERROR_HANDLER			\
  END (name)

#undef  PSEUDO_NOERRNO
#define PSEUDO_NOERRNO(name, syscall_name, args)\
  .text;					\
  ENTRY (name)					\
    mov $-38, %eax

#undef  PSEUDO_END_NOERRNO
#define PSEUDO_END_NOERRNO(name)		\
  END (name)

#undef ret_NOERRNO
#define ret_NOERRNO NACLRET

/* The function has to return the error code.  */
#undef  PSEUDO_ERRVAL
#define PSEUDO_ERRVAL(name, syscall_name, args)	\
  .text;					\
  ENTRY (name)					\
    mov $38, %eax

#undef  PSEUDO_END_ERRVAL
#define PSEUDO_END_ERRVAL(name)			\
  END (name)

#undef SYSCALL_ERROR_HANDLER_TLS_STORE
#define SYSCALL_ERROR_HANDLER_TLS_STORE(src, destoff)	\
  movl %gs:0, %eax;					\
  movl src, (%eax,destoff)

#endif
#endif
