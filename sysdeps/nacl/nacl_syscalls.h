/*
 * Copyright 2008 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can
 * be found in the LICENSE file.
 */

/*
 * NaCl kernel / service run-time system call numbers
 */

/* TODO(mseaborn): Import this header file from outside rather than
   keeping a copy in the glibc tree. */

#ifndef NATIVE_CLIENT_SRC_TRUSTED_SERVICE_RUNTIME_INCLUDE_BITS_NACL_SYSCALLS_H_
#define NATIVE_CLIENT_SRC_TRUSTED_SERVICE_RUNTIME_INCLUDE_BITS_NACL_SYSCALLS_H_


#include <nacl_stat.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>


/* intentionally not using zero */

/*
 * TODO(bsy,sehr): these identifiers should be NACL_ABI_SYS_<name>.
 */

#define NACL_sys_null                    1

#define NACL_sys_open                   10
#define NACL_sys_close                  11
#define NACL_sys_read                   12
#define NACL_sys_write                  13
#define NACL_sys_lseek                  14
#define NACL_sys_ioctl                  15
#define NACL_sys_stat                   16
#define NACL_sys_fstat                  17
#define NACL_sys_chmod                  18
/* no fchmod emulation on windows */

#define NACL_sys_sysbrk                 20
#define NACL_sys_mmap                   21
#define NACL_sys_munmap                 22

#define NACL_sys_getdents               23

#define NACL_sys_exit                   30
#define NACL_sys_getpid                 31
#define NACL_sys_sched_yield            32
#define NACL_sys_sysconf                33

#define NACL_sys_gettimeofday           40
#define NACL_sys_clock                  41
#define NACL_sys_nanosleep              42

#define NACL_sys_multimedia_init        50
#define NACL_sys_multimedia_shutdown    51
#define NACL_sys_video_init             52
#define NACL_sys_video_shutdown         53
#define NACL_sys_video_update           54
#define NACL_sys_video_poll_event       55
#define NACL_sys_audio_init             56
#define NACL_sys_audio_shutdown         57
#define NACL_sys_audio_stream           58

#define NACL_sys_imc_makeboundsock      60
#define NACL_sys_imc_accept             61
#define NACL_sys_imc_connect            62
#define NACL_sys_imc_sendmsg            63
#define NACL_sys_imc_recvmsg            64
#define NACL_sys_imc_mem_obj_create     65
#define NACL_sys_imc_socketpair         66

#define NACL_sys_mutex_create           70
#define NACL_sys_mutex_lock             71
#define NACL_sys_mutex_trylock          72
#define NACL_sys_mutex_unlock           73
#define NACL_sys_cond_create            74
#define NACL_sys_cond_wait              75
#define NACL_sys_cond_signal            76
#define NACL_sys_cond_broadcast         77
#define NACL_sys_cond_timed_wait_abs    79

#define NACL_sys_thread_create          80
#define NACL_sys_thread_exit            81
#define NACL_sys_tls_init               82
#define NACL_sys_thread_nice            83
#define NACL_sys_tls_get                84

#define NACL_sys_srpc_get_fd            90

#define NACL_sys_sem_create             100
#define NACL_sys_sem_wait               101
#define NACL_sys_sem_post               102
#define NACL_sys_sem_get_value          103
 
#define NACL_sys_dyncode_copy           104

#define NACL_MAX_SYSCALLS               110


#define NACL_SYSCALL_ADDR(syscall_number) \
  (0x10000 + ((syscall_number) * 32))

#define NACL_SYSCALL(syscall) \
  ((TYPE_nacl_ ## syscall) NACL_SYSCALL_ADDR(NACL_sys_ ## syscall))


struct NaClImcMsgHdr;
struct nacl_abi_stat;
struct timeval;
struct timespec;

typedef int (*TYPE_nacl_read) (int desc, void *buf, size_t count);
typedef int (*TYPE_nacl_close) (int desc);
typedef int (*TYPE_nacl_fstat) (int fd, struct nacl_abi_stat *stbp);
typedef int (*TYPE_nacl_write) (int desc, void const *buf, size_t count);
typedef int (*TYPE_nacl_open) (char const *pathname, int flags, mode_t mode);
typedef int (*TYPE_nacl_lseek) (int desc, nacl_abi_off_t *offset, int whence);
typedef int (*TYPE_nacl_stat) (const char *file, struct nacl_abi_stat *st);

typedef int (*TYPE_nacl_imc_recvmsg) (int desc,
                                      struct NaClImcMsgHdr *nmhp,
                                      int flags);
typedef int (*TYPE_nacl_imc_sendmsg) (int desc,
                                      struct NaClImcMsgHdr const *nmhp,
                                      int flags);
typedef int (*TYPE_nacl_imc_accept) (int d);
typedef int (*TYPE_nacl_imc_connect) (int d);
typedef int (*TYPE_nacl_imc_makeboundsock) (int *dp);
typedef int (*TYPE_nacl_imc_socketpair) (int *d2);
typedef int (*TYPE_nacl_imc_mem_obj_create) (size_t nbytes);

typedef void *(*TYPE_nacl_mmap) (void *start, size_t length,
                                 int prot, int flags, int desc,
                                 nacl_abi_off_t *offset);

typedef int (*TYPE_nacl_munmap) (void *start, size_t length);

typedef void (*TYPE_nacl_thread_exit) (int32_t *stack_flag);
typedef int (*TYPE_nacl_thread_create) (void *start_user_address,
                                        void *stack,
                                        void *tdb,
                                        size_t tdb_size);
typedef int (*TYPE_nacl_thread_nice) (const int nice);

typedef int (*TYPE_nacl_mutex_create) (void);
typedef int (*TYPE_nacl_mutex_lock) (int mutex);
typedef int (*TYPE_nacl_mutex_unlock) (int mutex);
typedef int (*TYPE_nacl_mutex_trylock) (int mutex);
typedef int (*TYPE_nacl_cond_create) (void);
typedef int (*TYPE_nacl_cond_wait) (int cv, int mutex);
typedef int (*TYPE_nacl_cond_signal) (int cv);
typedef int (*TYPE_nacl_cond_broadcast) (int cv);
typedef int (*TYPE_nacl_cond_timed_wait_abs) (int condvar,
                                              int mutex,
                                              struct timespec *abstime);
typedef int (*TYPE_nacl_sem_create) (int32_t value);
typedef int (*TYPE_nacl_sem_wait) (int sem);
typedef int (*TYPE_nacl_sem_post) (int sem);

typedef int (*TYPE_nacl_getdents) (int desc, void *dirp, size_t count);
typedef int (*TYPE_nacl_gettimeofday) (struct timeval *tv, void *tz);
typedef int (*TYPE_nacl_sched_yield) (void);
typedef int (*TYPE_nacl_sysconf) (int name, int *res);
typedef void *(*TYPE_nacl_sysbrk) (void *p);
typedef pid_t (*TYPE_nacl_getpid) (void);
typedef clock_t (*TYPE_nacl_clock) (void);
typedef int (*TYPE_nacl_nanosleep) (const struct timespec *req,
                                    struct timespec *rem);
/* Don't use __attribute__((noreturn)) on this because we want the
   wrapper to handle it if the syscall does happen to return. */
typedef void (*TYPE_nacl_exit) (int status);
typedef void (*TYPE_nacl_null) (void);
typedef int (*TYPE_nacl_tls_init) (void *tdb, int size);
typedef void *(*TYPE_nacl_tls_get) (void);
typedef int (*TYPE_nacl_srpc_get_fd) (void);
typedef int (*TYPE_nacl_dyncode_copy) (void *dest, const void *src,
                                       size_t size);


struct NaClImcMsgIoVec {
  void    *base;
  size_t  length;
};

struct NaClImcMsgHdr {
  struct NaClImcMsgIoVec  *iov;
  size_t                  iov_length;
  int                     *descv;
  size_t                  desc_length;
  int                     flags;
};


ssize_t imc_sendmsg (int fd, const struct NaClImcMsgHdr *msg, int flags);
ssize_t imc_recvmsg (int fd, struct NaClImcMsgHdr *msg, int flags);


#endif
