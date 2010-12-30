#ifndef _NACL_SYSDEP_H
#define _NACL_SYSDEP_H 1

#if !defined(__ASSEMBLER__)
#include <futex_emulation.h>
#include <lowlevellock.h>

/* TODO(mseaborn): Don't use these in the NaCl build, or at least
   don't hard code the value of ENOSYS. */

#define INTERNAL_SYSCALL(name, err, nr, args...) \
  INTERNAL_SYSCALL_ ## name(err, nr, args)

#define INTERNAL_SYSCALL_access(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_chmod(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_chown(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_chown32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_clock_getres(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_clock_gettime(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_clock_nanosleep(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_clock_settime(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_clone(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_close(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_epoll_pwait(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_eventfd(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_execve(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_faccessat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fallocate(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fadvise64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fchmodat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fchown(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fchown32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fchownat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fcntl(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fcntl64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fstat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fstat64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fstatat64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_fstatfs64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_ftruncate64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_futimesat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getcpu(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getcwd(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getdents(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getdents64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getegid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getegid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_geteuid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_geteuid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getgid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getgid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getgroups(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getgroups32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getpmsg(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getpriority(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getresgid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getresgid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getresuid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getresuid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getrlimit(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_gettid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getuid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_getuid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_ioctl(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_ipc(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_kill(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_lchown(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_lchown32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_link(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_linkat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL__llseek(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_lstat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_lstat64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_madvise(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_mkdir(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_mkdirat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_mknod(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_mknodat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_mq_notify(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_mq_open(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_mq_unlink(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_munmap(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_newfstatat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_open(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_openat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_poll(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_ppoll(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_pread(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_ptrace(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_putpmsg(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_pwrite(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_pselect6(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_readahead(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_readlink(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_readlinkat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_readv(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_reboot(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_rename(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_renameat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_rmdir(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_rt_sigaction(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_rt_sigpending(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_rt_sigprocmask(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_rt_sigqueueinfo(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_rt_sigsuspend(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_rt_sigtimedwait(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_sched_get_priority_max(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_sched_get_priority_min(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_sched_getaffinity(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_sched_getparam(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_sched_getscheduler(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_sched_setaffinity(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_sched_setscheduler(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_set_robust_list(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_set_tid_address(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setfsgid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setfsgid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setfsuid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setfsuid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setgid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setgid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setgroups(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setgroups32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setregid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setregid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setresgid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setresgid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setresuid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setresuid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setreuid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setreuid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setrlimit(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setuid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_setuid32(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_signalfd(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_stat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_stat64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_statfs64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_symlink(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_symlinkat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_sync_file_range(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL__sysctl(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_tgkill(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_time(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_timer_create(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_timer_delete(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_timer_getoverrun(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_timer_gettime(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_timer_settime(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_times(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_tkill(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_truncate64(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_ugetrlimit(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_unlink(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_unlinkat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_ustat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_utime(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_utimensat(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_utimes(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_wait4(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_waitid(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_write(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_writev(err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_NCS(name, err, nr, args...) (-38 /* ENOSYS */)
#define INTERNAL_SYSCALL_futex(err, nr, args...) \
  INTERNAL_SYSCALL_futex ## nr(args)

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

__extern_always_inline unsigned int
INTERNAL_SYSCALL_futex2 (union __attribute__ ((__transparent_union__))
			 {
			   volatile int *addr;
			   volatile unsigned int *uaddr;
			 } addr,
			 union encoded_futex_operation futex_operation)
{
  switch (futex_operation.decoded_futex_operation.operation)
    {
      case FUTEX_UNLOCK_PI:
	return (-38 /* ENOSYS */);
      default:
	if (__builtin_constant_p (
	    futex_operation.decoded_futex_operation.operation))
	  __futex_emulation_unknown_operation ();
    }
    return 0;
}

__extern_always_inline unsigned int
INTERNAL_SYSCALL_futex3 (union __attribute__ ((__transparent_union__))
			 {
			   volatile int *addr;
			   volatile unsigned int *uaddr;
			 } addr,
			 union encoded_futex_operation futex_operation,
			 int val)
{
  switch (futex_operation.decoded_futex_operation.operation)
    {
      case FUTEX_WAKE:
	return __nacl_futex_wake (addr.addr, val, __FUTEX_BITSET_MATCH_ANY);
      default:
	if (__builtin_constant_p (
	    futex_operation.decoded_futex_operation.operation))
	  __futex_emulation_unknown_operation ();
    }
    return 0;
}

__extern_always_inline unsigned int
INTERNAL_SYSCALL_futex4 (union __attribute__ ((__transparent_union__))
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
	return __nacl_futex_wait (addr.addr, val1, __FUTEX_BITSET_MATCH_ANY,
				  val2.timeout);
      /* TODO(khim): handle PI-locks in some less hacky way.  */
      case FUTEX_LOCK_PI:
      case FUTEX_UNLOCK_PI:
      case FUTEX_TRYLOCK_PI:
	return (-38 /* ENOSYS */);
      default:
	if (__builtin_constant_p (
	    futex_operation.decoded_futex_operation.operation))
	  __futex_emulation_unknown_operation ();
    }
    return 0;
}
__extern_always_inline unsigned int
INTERNAL_SYSCALL_futex6 (union __attribute__ ((__transparent_union__))
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
  switch (futex_operation.decoded_futex_operation.operation)
    {
      case FUTEX_WAKE_OP:
	return __nacl_futex_wake_op (addr1.addr, addr2.addr, val1, val2, val3);
      case FUTEX_CMP_REQUEUE:
	return __nacl_futex_cmp_requeue (addr1.addr, addr2.addr,
					 val1, val2, val3);
      default:
	if (__builtin_constant_p (
	    futex_operation.decoded_futex_operation.operation))
	  __futex_emulation_unknown_operation ();
    }
    return 0;
}
#endif

#include_next <sysdep.h>

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
