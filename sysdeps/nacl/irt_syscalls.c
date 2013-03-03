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



static void nacl_irt_exit (int status) {
    NACL_SYSCALL (exit) (status);
    while (1) (*(void (*)(void)) 0) ();  /* Crash.  */
}

static int nacl_irt_gettod (struct timeval *tv) {
    return -NACL_SYSCALL (gettimeofday) (tv, NULL);
}

static int nacl_irt_clock (clock_t *ticks) {
    *ticks = NACL_SYSCALL (clock) ();  /* Cannot fail.  */
    return 0;
}

static int nacl_irt_nanosleep (const struct timespec *req,
                               struct timespec *rem) {
    return -NACL_SYSCALL (nanosleep) (req, rem);
}

static int nacl_irt_sched_yield (void) {
    return -NACL_SYSCALL (sched_yield) ();
}

static int nacl_irt_sysconf (int name, int *value) {
    return -NACL_SYSCALL (sysconf) (name, value);
}

static int nacl_irt_open (const char *pathname, int oflag, mode_t cmode,
                          int *newfd) {
    int rv = NACL_SYSCALL (open) (pathname, oflag, cmode);
    if (rv < 0)
	return -rv;
    *newfd = rv;
    return 0;
}

static int nacl_irt_close (int fd) {
    return -NACL_SYSCALL (close) (fd);
}

static int nacl_irt_read (int fd, void *buf, size_t count, size_t *nread) {
    int rv = NACL_SYSCALL (read) (fd, buf, count);
    if (rv < 0)
	return -rv;
    *nread = rv;
    return 0;
}

static int nacl_irt_write (int fd, const void *buf, size_t count,
                           size_t *nwrote) {
    int rv = NACL_SYSCALL (write) (fd, buf, count);
    if (rv < 0)
	return -rv;
    *nwrote = rv;
    return 0;
}

static int nacl_irt_seek (int fd, nacl_abi_off_t offset, int whence,
                          off_t *new_offset) {
    int rv = NACL_SYSCALL (lseek) (fd, &offset, whence);
    if (rv < 0)
	return -rv;
    *new_offset = offset;
    return 0;
}

static int nacl_irt_dup (int fd, int *newfd) {
    int rv = NACL_SYSCALL (dup) (fd);
    if (rv < 0)
	return -rv;
    *newfd = rv;
    return 0;
}

static int nacl_irt_dup2 (int fd, int newfd) {
    int rv = NACL_SYSCALL (dup2) (fd, newfd);
    if (rv < 0)
	return -rv;
    return 0;
}

static int nacl_irt_fstat (int fd, struct stat *st) {
    return -NACL_SYSCALL (fstat) (fd, st);
}

static int nacl_irt_stat (const char *pathname, struct stat *st) {
    return -NACL_SYSCALL (stat) (pathname, st);
}

static int nacl_irt_getdents (int fd, struct dirent *buf, size_t count,
                              size_t *nread) {
    int rv = NACL_SYSCALL (getdents) (fd, buf, count);
    if (rv < 0)
	return -rv;
    *nread = rv;
    return 0;
}

static int nacl_irt_sysbrk (void **newbrk) {
    /*
     * The syscall does not actually indicate error.  It just returns the
     * new current value, which is unchanged if something went wrong.
     * But if the requested value was below the end of the data segment,
     * the new value will be greater, but this is not "going wrong".
     * Here we just approximate a saner interface: you get what you requested,
     * you did a "probe" request passing NULL in, or it's an error.
     * TODO(mcgrathr): this interface should just go away!!
     */
    void *requested = *newbrk;
    void *got = NACL_SYSCALL (sysbrk) (requested);

    if (got == requested || requested == NULL) {
	*newbrk = got;
	return 0;
    }

    return (12 /* ENOSYS */);
}

static int nacl_irt_mmap (void **addr, size_t len,
                          int prot, int flags, int fd, off_t off) {
    uint32_t rv = (uintptr_t) NACL_SYSCALL (mmap) (*addr, len, prot, flags,
						   fd, &off);
    if ((uint32_t) rv > 0xffff0000u)
	return -(int32_t) rv;
    *addr = (void *) (uintptr_t) rv;
    return 0;
}

static int nacl_irt_munmap (void *addr, size_t len) {
    return -NACL_SYSCALL (munmap) (addr, len);
}

static int nacl_irt_dyncode_create(void *dest, const void *src, size_t size) {
    return -NACL_SYSCALL(dyncode_create)(dest, src, size);
}

static int nacl_irt_dyncode_modify(void *dest, const void *src, size_t size) {
    return -NACL_SYSCALL(dyncode_modify)(dest, src, size);
}

static int nacl_irt_dyncode_delete(void *dest, size_t size) {
    return -NACL_SYSCALL(dyncode_delete)(dest, size);
}

static int nacl_irt_thread_create (void *start_user_address, void *stack,
                                   void *thread_ptr) {
    return -NACL_SYSCALL (thread_create) (start_user_address, stack,
					  thread_ptr, 0);
}

static void nacl_irt_thread_exit (int32_t *stack_flag) {
    NACL_SYSCALL (thread_exit) (stack_flag);
    while (1) (*(void (*)(void)) 0) ();  /* Crash.  */
}

static int nacl_irt_thread_nice (const int nice) {
    return -NACL_SYSCALL (thread_nice) (nice);
}

static int nacl_irt_mutex_create (int *mutex_handle) {
    int rv = NACL_SYSCALL (mutex_create) ();
    if (rv < 0)
	return -rv;
    *mutex_handle = rv;
    return 0;
}

/*
 * Today a mutex handle is just an fd and we destroy it with close.
 * But this might not always be so.
 */
static int nacl_irt_mutex_destroy (int mutex_handle) {
    return -NACL_SYSCALL (close) (mutex_handle);
}

static int nacl_irt_mutex_lock (int mutex_handle) {
    return -NACL_SYSCALL (mutex_lock) (mutex_handle);
}

static int nacl_irt_mutex_unlock(int mutex_handle) {
    return -NACL_SYSCALL (mutex_unlock) (mutex_handle);
}

static int nacl_irt_mutex_trylock(int mutex_handle) {
    return -NACL_SYSCALL (mutex_trylock) (mutex_handle);
}

static int nacl_irt_cond_create (int *cond_handle) {
    int rv = NACL_SYSCALL (cond_create) ();
    if (rv < 0)
	return -rv;
    *cond_handle = rv;
    return 0;
}

/*
 * Today a cond handle is just an fd and we destroy it with close.
 * But this might not always be so.
 */
static int nacl_irt_cond_destroy (int cond_handle) {
    return -NACL_SYSCALL (close) (cond_handle);
}

static int nacl_irt_cond_signal (int cond_handle) {
    return -NACL_SYSCALL (cond_signal) (cond_handle);
}

static int nacl_irt_cond_broadcast (int cond_handle) {
    return -NACL_SYSCALL (cond_broadcast) (cond_handle);
}

static int nacl_irt_cond_wait (int cond_handle, int mutex_handle) {
    return -NACL_SYSCALL (cond_wait) (cond_handle, mutex_handle);
}

static int nacl_irt_cond_timed_wait_abs (int cond_handle, int mutex_handle,
                                         const struct timespec *abstime) {
    return -NACL_SYSCALL (cond_timed_wait_abs) (cond_handle, mutex_handle,
						abstime);
}

static int nacl_irt_tls_init (void *tdb) {
    return -NACL_SYSCALL (tls_init) (tdb);
}

static void *nacl_irt_tls_get (void) {
    return NACL_SYSCALL (tls_get) ();
}

static int nacl_irt_open_as_resource (const char *pathname, int *newfd) {
    return __nacl_irt_open (pathname, O_RDONLY, 0, newfd);
}

/* Load files from DL_DST_LIB using IRT's open_resource. Other paths
   will be processed using regular open syscall.

   Note: nacl_mount may change this logic if needed.  */
static int (*___nacl_irt_open_resource) (const char* file, int *fd);
static int nacl_irt_open_resource (const char *pathname, int *newfd) {
    if (memcmp (DL_DST_LIB "/", pathname, sizeof (DL_DST_LIB)))
	return __nacl_irt_open (pathname, O_RDONLY, 0, newfd);
    else
	return ___nacl_irt_open_resource (pathname + sizeof (DL_DST_LIB) - 1,
					  newfd);
}

static int nacl_irt_clock_getres(clockid_t clk_id,
                                 struct timespec *res) {
    return -NACL_SYSCALL (clock_getres) (clk_id, res);
}

static int nacl_irt_clock_gettime(clockid_t clk_id,
                                  struct timespec *tp) {
    return -NACL_SYSCALL (clock_gettime) (clk_id, tp);
}

static size_t no_interface (const char *interface_ident,
                            void *table, size_t tablesize) {
    return 0;
}

static int not_implemented(void) {
    return (38 /* ENOSYS */);
}

size_t (*__nacl_irt_query) (const char *interface_ident,
			    void *table, size_t tablesize);

int (*__nacl_irt_mkdir) (const char* pathname, mode_t mode);
int (*__nacl_irt_rmdir) (const char* pathname);
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
int (*__nacl_irt_dup) (int fd, int *newfd);
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

void
init_irt_table (void)
{
    /*setup zrt zcalls*/
    INIT_ZCALLS;

    /*init additional handlers*/
    INIT_ZCALLS_NONSYSCALLS;
}

size_t nacl_interface_query(const char *interface_ident,
                            void *table, size_t tablesize) {
    return (*__nacl_irt_query)(interface_ident, table, tablesize);
}
