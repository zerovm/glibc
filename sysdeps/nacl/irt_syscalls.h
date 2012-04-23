#ifndef _IRT_SYSCALLS_H
#define _IRT_SYSCALLS_H

#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <poll.h>
#include <stddef.h>
#include <fcntl.h>
#include <time.h>

#include <nacl_stat.h>

struct dirent;
struct nacl_abi_stat;
struct timeval;
struct timespec;
struct sockaddr;
struct msghdr;

#define socklen_t unsigned int

extern size_t (*__nacl_irt_query)(const char *interface_ident,
                                  void *table, size_t tablesize);

extern void (*__nacl_irt_exit) (int status);
extern int (*__nacl_irt_gettod) (struct timeval *tv);
extern int (*__nacl_irt_clock) (clock_t *ticks);
extern int (*__nacl_irt_nanosleep) (const struct timespec *req,
                                    struct timespec *rem);
extern int (*__nacl_irt_sched_yield) (void);
extern int (*__nacl_irt_sysconf) (int name, int *value);

extern int (*__nacl_irt_mkdir) (const char* pathname, mode_t mode);
extern int (*__nacl_irt_rmdir) (const char* pathname);
extern int (*__nacl_irt_chdir) (const char* pathname);
extern int (*__nacl_irt_getcwd) (char* buf, size_t size, int *len);

extern int (*__nacl_irt_epoll_create) (int size, int *fd);
extern int (*__nacl_irt_epoll_create1) (int flags, int *fd);
extern int (*__nacl_irt_epoll_ctl) (int epfd, int op, int fd,
                                    struct epoll_event *event);
extern int (*__nacl_irt_epoll_pwait) (int epfd, struct epoll_event *events,
            int maxevents, int timeout, const sigset_t *sigmask,
            size_t sigset_size, int *count);
extern int (*__nacl_irt_epoll_wait) (int epfd, struct epoll_event *events,
                                 int maxevents, int timeout, int *count);
extern int (*__nacl_irt_poll) (struct pollfd *fds, nfds_t nfds,
                           int timeout, int *count);
extern int (*__nacl_irt_ppoll) (struct pollfd *fds, nfds_t nfds,
            const struct timespec *timeout, const sigset_t *sigmask,
            size_t sigset_size, int *count);
extern int (*__nacl_irt_socket) (int domain, int type, int protocol, int *sd);
extern int (*__nacl_irt_accept) (int sockfd, struct sockaddr *addr,
                                 socklen_t *addrlen, int *sd);
extern int (*__nacl_irt_bind) (int sockfd, const struct sockaddr *addr,
                               socklen_t addrlen);
extern int (*__nacl_irt_listen) (int sockfd, int backlog);
extern int (*__nacl_irt_connect) (int sockfd, const struct sockaddr *addr,
                                  socklen_t addrlen);
extern int (*__nacl_irt_send) (int sockfd, const void *buf, size_t len,
                               int flags, int *count);
extern int (*__nacl_irt_sendmsg) (int sockfd, const struct msghdr *msg,
                                  int flags, int *count);
extern int (*__nacl_irt_sendto) (int sockfd, const void *buf, size_t len,
            int flags, const struct sockaddr *dest_addr, socklen_t addrlen,
            int *count);
extern int (*__nacl_irt_recv) (int sockfd, void *buf, size_t len, int flags,
                               int *count);
extern int (*__nacl_irt_recvmsg) (int sockfd, struct msghdr *msg,
                                  int flags, int *count);
extern int (*__nacl_irt_recvfrom) (int sockfd, void *buf, size_t len, int flags,
            struct sockaddr *dest_addr, socklen_t* addrlen, int *count);
extern int (*__nacl_irt_select) (int nfds, fd_set *readfds,
                                 fd_set *writefds, fd_set *exceptfds,
                                 const struct timeval *timeout, int *count);
extern int (*__nacl_irt_pselect) (int nfds, fd_set *readfds,
            fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout,
			void* sigmask, int *count);
extern int (*__nacl_irt_getpeername) (int sockfd, struct sockaddr *addr,
                                      socklen_t *addrlen);
extern int (*__nacl_irt_getsockname) (int sockfd, struct sockaddr *addr,
                                      socklen_t *addrlen);
extern int (*__nacl_irt_getsockopt) (int sockfd, int level, int optname,
                      void *optval, socklen_t *optlen);
extern int (*__nacl_irt_setsockopt) (int sockfd, int level, int optname,
                      const void *optval, socklen_t optlen);
extern int (*__nacl_irt_socketpair) (int domain, int type, int protocol,
                                     int sv[2]);
extern int (*__nacl_irt_shutdown) (int sockfd, int how);


extern int (*__nacl_irt_open) (const char *pathname, int oflag, mode_t cmode,
                               int *newfd);
extern int (*__nacl_irt_close) (int fd);
extern int (*__nacl_irt_read) (int fd, void *buf, size_t count, size_t *nread);
extern int (*__nacl_irt_write) (int fd, const void *buf, size_t count,
                                size_t *nwrote);
extern int (*__nacl_irt_seek) (int fd, nacl_abi_off_t offset, int whence,
                               nacl_abi_off_t *new_offset);
extern int (*__nacl_irt_dup) (int fd, int *newfd);
extern int (*__nacl_irt_dup2) (int fd, int newfd);
extern int (*__nacl_irt_fstat) (int fd, struct nacl_abi_stat *);
extern int (*__nacl_irt_stat) (const char *pathname, struct nacl_abi_stat *);
extern int (*__nacl_irt_getdents) (int fd, struct dirent *, size_t count,
                                   size_t *nread);

extern int (*__nacl_irt_sysbrk)(void **newbrk);
extern int (*__nacl_irt_mmap)(void **addr, size_t len, int prot, int flags,
                              int fd, nacl_abi_off_t off);
extern int (*__nacl_irt_munmap)(void *addr, size_t len);

extern int (*__nacl_irt_dyncode_create) (void *dest, const void *src,
            size_t size);
extern int (*__nacl_irt_dyncode_modify) (void *dest, const void *src,
            size_t size);
extern int (*__nacl_irt_dyncode_delete) (void *dest, size_t size);

extern int (*__nacl_irt_thread_create) (void *start_user_address, void *stack,
                                        void *thread_ptr);
extern void (*__nacl_irt_thread_exit) (int32_t *stack_flag);
extern int (*__nacl_irt_thread_nice) (const int nice);

extern int (*__nacl_irt_mutex_create) (int *mutex_handle);
extern int (*__nacl_irt_mutex_destroy) (int mutex_handle);
extern int (*__nacl_irt_mutex_lock) (int mutex_handle);
extern int (*__nacl_irt_mutex_unlock) (int mutex_handle);
extern int (*__nacl_irt_mutex_trylock) (int mutex_handle);

extern int (*__nacl_irt_cond_create) (int *cond_handle);
extern int (*__nacl_irt_cond_destroy) (int cond_handle);
extern int (*__nacl_irt_cond_signal) (int cond_handle);
extern int (*__nacl_irt_cond_broadcast) (int cond_handle);
extern int (*__nacl_irt_cond_wait) (int cond_handle, int mutex_handle);
extern int (*__nacl_irt_cond_timed_wait_abs) (int cond_handle, int mutex_handle,
                                              const struct timespec *abstime);

extern int (*__nacl_irt_tls_init) (void *tdb);
extern void *(*__nacl_irt_tls_get) (void);

extern int (*__nacl_irt_open_resource) (const char* file, int *fd);

extern int (*__nacl_irt_clock_getres) (clockid_t clk_id, struct timespec *res);
extern int (*__nacl_irt_clock_gettime) (clockid_t clk_id, struct timespec *tp);
#undef socklen_t

#ifdef _LIBC
void init_irt_table (void) attribute_hidden;
#endif
#endif

#if defined(_LIBC) || defined (__need_emulated_syscalls)
#ifndef _IRT_EMULATED_SYSCALLS_H
#define _IRT_EMULATED_SYSCALLS_H 1

#ifndef _LINUX_TYPES_H
#define ustat __kernel_ustat
#include <linux/sysctl.h>
#undef ustat
#ifdef _LIBC
#include <misc/sys/ustat.h>
#else
#include <sys/ustat.h>
#endif
#endif

#include <linux/getcpu.h>
#include <linux/posix_types.h>
#if !defined (_LIBC) || defined(IS_IN_librt)
#include <mqueue.h>
#endif
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <streams/stropts.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <sys/ptrace.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <time.h>
#include <utime.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/timex.h>
#include <sys/types.h>
#include <sys/utsname.h>
#ifdef __i386__
#include <sys/vm86.h>
#endif
#include <unistd.h>

#ifdef _LIBC
struct robust_list_head;
#else
struct robust_list_head
{
  void *list;
  long int futex_offset;
  void *list_op_pending;
};
#endif

#endif
#endif
