#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <nacl_stat.h>
#include <nacl_syscalls.h>
#include <irt.h>
#undef stat
#include <irt_syscalls.h>
#ifdef IS_IN_rtld
#include <ldsodefs.h>
#endif
#include "trusted-dirs.h"

#include "zrt.h" //__query_zcalls, struct zcalls_xxxx_t
#include "irt_zcalls.h"

int nacl_irt_getcwd(char* buf, size_t size, int *len){
    char* ret = __getcwd(buf, size);
    if ( ret != NULL ){
	*len = strlen(buf);
	return 0;
    }
    return -1;
}


static int not_implemented(void) {
    return (38 /* ENOSYS */);
}

size_t (*__nacl_irt_query) (const char *interface_ident,
			    void *table, size_t tablesize);

int (*__nacl_irt_chdir) (const char* pathname);
int (*__nacl_irt_getcwd) (char* buf, size_t size, int *len);

void (*__nacl_irt_exit) (int status);
int (*__nacl_irt_gettod) (struct timeval *tv);
int (*__nacl_irt_clock) (clock_t *ticks);
int (*__nacl_irt_nanosleep) (const struct timespec *req, struct timespec *rem);
int (*__nacl_irt_sched_yield) (void);
int (*__nacl_irt_sysconf) (int name, int *value);

int (*__nacl_irt_open) (const char *pathname, int oflag, mode_t cmode,
			int *newfd);
int (*__nacl_irt_close) (int fd);
int (*__nacl_irt_read) (int fd, void *buf, size_t count, size_t *nread);
int (*__nacl_irt_write) (int fd, const void *buf, size_t count, size_t *nwrote);
int (*__nacl_irt_seek) (int fd, off_t offset, int whence, off_t *new_offset);
int (*__nacl_irt_dup) (int fd);
int (*__nacl_irt_dup2) (int fd, int newfd);
int (*__nacl_irt_fstat) (int fd, struct stat *);
int (*__nacl_irt_stat) (const char *pathname, struct stat *);
int (*__nacl_irt_getdents) (int fd, struct dirent *, size_t count,
			    size_t *nread);
int (*__nacl_irt_socket) (int domain, int type, int protocol, int *sd);
int (*__nacl_irt_accept) (int sockfd, struct sockaddr *addr,
                          socklen_t *addrlen, int *sd);
int (*__nacl_irt_bind) (int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int (*__nacl_irt_listen) (int sockfd, int backlog);
int (*__nacl_irt_connect) (int sockfd, const struct sockaddr *addr,
                           socklen_t addrlen);
int (*__nacl_irt_send) (int sockfd, const void *buf, size_t len, int flags,
                        int *count);
int (*__nacl_irt_sendmsg) (int sockfd, const struct msghdr *msg, int flags,
                           int *count);
int (*__nacl_irt_sendto) (int sockfd, const void *buf, size_t len, int flags,
                          const struct sockaddr *dest_addr, socklen_t addrlen,
			  int *count);
int (*__nacl_irt_recv) (int sockfd, void *buf, size_t len, int flags, int *count);
int (*__nacl_irt_recvmsg) (int sockfd, struct msghdr *msg, int flags, int *count);
int (*__nacl_irt_recvfrom) (int sockfd, void *buf, size_t len, int flags,
                            struct sockaddr *dest_addr, socklen_t* addrlen, int *count);

int (*__nacl_irt_epoll_create) (int size, int *fd);
int (*__nacl_irt_epoll_create1) (int flags, int *fd);
int (*__nacl_irt_epoll_ctl) (int epfd, int op, int fd,
                             struct epoll_event *event);
int (*__nacl_irt_epoll_pwait) (int epfd, struct epoll_event *events,
			       int maxevents, int timeout, const sigset_t *sigmask, size_t sigset_size,
			       int *count);
int (*__nacl_irt_epoll_wait) (int epfd, struct epoll_event *events,
			      int maxevents, int timeout, int *count);
int (*__nacl_irt_poll) (struct pollfd *fds, nfds_t nfds,
			int timeout, int *count);
int (*__nacl_irt_ppoll) (struct pollfd *fds, nfds_t nfds,
			 const struct timespec *timeout, const sigset_t *sigmask,
			 size_t sigset_size, int *count);
int (*__nacl_irt_select) (int nfds, fd_set *readfds,
			  fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout,
			  int *count);
int (*__nacl_irt_pselect) (int nfds, fd_set *readfds,
			   fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout,
			   void* sigmask, int *count);
int (*__nacl_irt_getpeername) (int sockfd, struct sockaddr *addr,
                               socklen_t *addrlen);
int (*__nacl_irt_getsockname) (int sockfd, struct sockaddr *addr,
                               socklen_t *addrlen);
int (*__nacl_irt_getsockopt) (int sockfd, int level, int optname,
			      void *optval, socklen_t *optlen);
int (*__nacl_irt_setsockopt) (int sockfd, int level, int optname,
			      const void *optval, socklen_t optlen);
int (*__nacl_irt_socketpair) (int domain, int type, int protocol, int sv[2]);
int (*__nacl_irt_shutdown) (int sockfd, int how);


int (*__nacl_irt_sysbrk) (void **newbrk);
int (*__nacl_irt_mmap) (void **addr, size_t len, int prot, int flags,
			int fd, off_t off);
int (*__nacl_irt_munmap) (void *addr, size_t len);

int (*__nacl_irt_dyncode_create) (void *dest, const void *src, size_t size);
int (*__nacl_irt_dyncode_modify) (void *dest, const void *src, size_t size);
int (*__nacl_irt_dyncode_delete) (void *dest, size_t size);

int (*__nacl_irt_thread_create) (void *start_user_address, void *stack,
				 void *thread_ptr);
void (*__nacl_irt_thread_exit) (int32_t *stack_flag);
int (*__nacl_irt_thread_nice) (const int nice);

int (*__nacl_irt_mutex_create) (int *mutex_handle);
int (*__nacl_irt_mutex_destroy) (int mutex_handle);
int (*__nacl_irt_mutex_lock) (int mutex_handle);
int (*__nacl_irt_mutex_unlock) (int mutex_handle);
int (*__nacl_irt_mutex_trylock) (int mutex_handle);

int (*__nacl_irt_cond_create) (int *cond_handle);
int (*__nacl_irt_cond_destroy) (int cond_handle);
int (*__nacl_irt_cond_signal) (int cond_handle);
int (*__nacl_irt_cond_broadcast) (int cond_handle);
int (*__nacl_irt_cond_wait) (int cond_handle, int mutex_handle);
int (*__nacl_irt_cond_timed_wait_abs) (int cond_handle, int mutex_handle,
				       const struct timespec *abstime);

int (*__nacl_irt_tls_init) (void *tdb);
void *(*__nacl_irt_tls_get) (void);

int (*__nacl_irt_open_resource) (const char* file, int *fd);

int (*__nacl_irt_clock_getres) (clockid_t clk_id, struct timespec *res);
int (*__nacl_irt_clock_gettime) (clockid_t clk_id, struct timespec *tp);

#define DEFAULT_UNINITIALIZED not_implemented

static void set_irt_uninitialized(){
    __nacl_irt_query=DEFAULT_UNINITIALIZED;
    __nacl_irt_chdir=DEFAULT_UNINITIALIZED;
    __nacl_irt_getcwd=DEFAULT_UNINITIALIZED;
    __nacl_irt_exit=DEFAULT_UNINITIALIZED;
    __nacl_irt_gettod=DEFAULT_UNINITIALIZED;
    __nacl_irt_clock=DEFAULT_UNINITIALIZED;
    __nacl_irt_nanosleep=DEFAULT_UNINITIALIZED;
    __nacl_irt_sched_yield=DEFAULT_UNINITIALIZED;
    __nacl_irt_sysconf=DEFAULT_UNINITIALIZED;
    __nacl_irt_open=DEFAULT_UNINITIALIZED;
    __nacl_irt_close=DEFAULT_UNINITIALIZED;
    __nacl_irt_read=DEFAULT_UNINITIALIZED;
    __nacl_irt_write=DEFAULT_UNINITIALIZED;
    __nacl_irt_seek=DEFAULT_UNINITIALIZED;
    __nacl_irt_dup=DEFAULT_UNINITIALIZED;
    __nacl_irt_dup2=DEFAULT_UNINITIALIZED;
    __nacl_irt_fstat=DEFAULT_UNINITIALIZED;
    __nacl_irt_stat=DEFAULT_UNINITIALIZED;
    __nacl_irt_getdents=DEFAULT_UNINITIALIZED;
    __nacl_irt_socket=DEFAULT_UNINITIALIZED;
    __nacl_irt_accept=DEFAULT_UNINITIALIZED;
    __nacl_irt_bind=DEFAULT_UNINITIALIZED;
    __nacl_irt_listen=DEFAULT_UNINITIALIZED;
    __nacl_irt_connect=DEFAULT_UNINITIALIZED;
    __nacl_irt_send=DEFAULT_UNINITIALIZED;
    __nacl_irt_sendmsg=DEFAULT_UNINITIALIZED;
    __nacl_irt_sendto=DEFAULT_UNINITIALIZED;
    __nacl_irt_recv=DEFAULT_UNINITIALIZED;
    __nacl_irt_recvmsg=DEFAULT_UNINITIALIZED;
    __nacl_irt_recvfrom=DEFAULT_UNINITIALIZED;
    __nacl_irt_epoll_create=DEFAULT_UNINITIALIZED;
    __nacl_irt_epoll_create1=DEFAULT_UNINITIALIZED;
    __nacl_irt_epoll_ctl=DEFAULT_UNINITIALIZED;
    __nacl_irt_epoll_pwait=DEFAULT_UNINITIALIZED;
    __nacl_irt_epoll_wait=DEFAULT_UNINITIALIZED;
    __nacl_irt_poll=DEFAULT_UNINITIALIZED;
    __nacl_irt_ppoll=DEFAULT_UNINITIALIZED;
    __nacl_irt_select=DEFAULT_UNINITIALIZED;
    __nacl_irt_pselect=DEFAULT_UNINITIALIZED;
    __nacl_irt_getpeername=DEFAULT_UNINITIALIZED;
    __nacl_irt_getsockname=DEFAULT_UNINITIALIZED;
    __nacl_irt_getsockopt=DEFAULT_UNINITIALIZED;
    __nacl_irt_setsockopt=DEFAULT_UNINITIALIZED;
    __nacl_irt_socketpair=DEFAULT_UNINITIALIZED;
    __nacl_irt_shutdown=DEFAULT_UNINITIALIZED;
    __nacl_irt_sysbrk=DEFAULT_UNINITIALIZED;
    __nacl_irt_mmap=DEFAULT_UNINITIALIZED;
    __nacl_irt_munmap=DEFAULT_UNINITIALIZED;
    __nacl_irt_dyncode_create=DEFAULT_UNINITIALIZED;
    __nacl_irt_dyncode_modify=DEFAULT_UNINITIALIZED;
    __nacl_irt_dyncode_delete=DEFAULT_UNINITIALIZED;
    __nacl_irt_thread_create=DEFAULT_UNINITIALIZED;
    __nacl_irt_thread_exit=DEFAULT_UNINITIALIZED;
    __nacl_irt_thread_nice=DEFAULT_UNINITIALIZED;
    __nacl_irt_mutex_create=DEFAULT_UNINITIALIZED;
    __nacl_irt_mutex_destroy=DEFAULT_UNINITIALIZED;
    __nacl_irt_mutex_lock=DEFAULT_UNINITIALIZED;
    __nacl_irt_mutex_unlock=DEFAULT_UNINITIALIZED;
    __nacl_irt_mutex_trylock=DEFAULT_UNINITIALIZED;
    __nacl_irt_cond_create=DEFAULT_UNINITIALIZED;
    __nacl_irt_cond_destroy=DEFAULT_UNINITIALIZED;
    __nacl_irt_cond_signal=DEFAULT_UNINITIALIZED;
    __nacl_irt_cond_broadcast=DEFAULT_UNINITIALIZED;
    __nacl_irt_cond_wait=DEFAULT_UNINITIALIZED;
    __nacl_irt_cond_timed_wait_abs=DEFAULT_UNINITIALIZED;
    __nacl_irt_tls_init=DEFAULT_UNINITIALIZED;
    __nacl_irt_tls_get=DEFAULT_UNINITIALIZED;
    __nacl_irt_open_resource=DEFAULT_UNINITIALIZED;
    __nacl_irt_clock_getres=DEFAULT_UNINITIALIZED;
    __nacl_irt_clock_gettime=DEFAULT_UNINITIALIZED;

    __zcall_fcntl=DEFAULT_UNINITIALIZED;
    __zcall_rename=DEFAULT_UNINITIALIZED;
    __zcall_readlink=DEFAULT_UNINITIALIZED;
    __zcall_symlink=DEFAULT_UNINITIALIZED;
    __zcall_statvfs=DEFAULT_UNINITIALIZED;
    __zcall_link=DEFAULT_UNINITIALIZED;
    __zcall_unlink=DEFAULT_UNINITIALIZED;
    __zcall_rmdir=DEFAULT_UNINITIALIZED;
    __zcall_mkdir=DEFAULT_UNINITIALIZED;
    __zcall_chmod=DEFAULT_UNINITIALIZED;
    __zcall_fchmod=DEFAULT_UNINITIALIZED;
    __zcall_chown=DEFAULT_UNINITIALIZED;
    __zcall_fchown=DEFAULT_UNINITIALIZED;
    __zcall_ftruncate=DEFAULT_UNINITIALIZED;
    __zcall_get_phys_pages=DEFAULT_UNINITIALIZED;
    __zcall_get_avphys_pages=DEFAULT_UNINITIALIZED;
    __zcall_fchdir=DEFAULT_UNINITIALIZED;
}

void
init_irt_table (void)
{
    set_irt_uninitialized();

    /*setup zrt zcalls*/
    INIT_ZCALLS;

    /*init additional handlers*/
    INIT_ZCALLS_NONSYSCALLS;
}
