#include <irt_syscalls.h>
#include <nacl_stat.h>
#include <nacl_syscalls.h>

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

static int nacl_irt_fstat (int fd, struct nacl_abi_stat *st) {
  return -NACL_SYSCALL (fstat) (fd, st);
}

static int nacl_irt_stat (const char *pathname, struct nacl_abi_stat *st) {
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


static int not_implemented (void) {
  return (12 /* ENOSYS */);
}

size_t (*__nacl_irt_query) (const char *interface_ident,
                            void *table, size_t tablesize) = not_implemented;

void (*__nacl_irt_exit) (int status) = nacl_irt_exit;
int (*__nacl_irt_gettod) (struct timeval *tv) = nacl_irt_gettod;
int (*__nacl_irt_clock) (clock_t *ticks) = nacl_irt_clock;
int (*__nacl_irt_nanosleep) (const struct timespec *req, struct timespec *rem) =
  nacl_irt_nanosleep;
int (*__nacl_irt_sched_yield) (void) = nacl_irt_sched_yield;
int (*__nacl_irt_sysconf) (int name, int *value) = nacl_irt_sysconf;

int (*__nacl_irt_open) (const char *pathname, int oflag, mode_t cmode,
                        int *newfd) = nacl_irt_open;
int (*__nacl_irt_close) (int fd) = nacl_irt_close;
int (*__nacl_irt_read) (int fd, void *buf, size_t count, size_t *nread) =
  nacl_irt_read;
int (*__nacl_irt_write) (int fd, const void *buf, size_t count,
                         size_t *nwrote) = nacl_irt_write;
int (*__nacl_irt_seek) (int fd, off_t offset, int whence, off_t *new_offset) =
  nacl_irt_seek;
int (*__nacl_irt_dup) (int fd, int *newfd) = nacl_irt_dup;
int (*__nacl_irt_dup2) (int fd, int newfd) = nacl_irt_dup2;
int (*__nacl_irt_fstat) (int fd, struct nacl_abi_stat *) = nacl_irt_fstat;
int (*__nacl_irt_stat) (const char *pathname, struct nacl_abi_stat *) =
  nacl_irt_stat;
int (*__nacl_irt_getdents) (int fd, struct dirent *, size_t count,
                            size_t *nread) = nacl_irt_getdents;

int (*__nacl_irt_sysbrk) (void **newbrk) = nacl_irt_sysbrk;
int (*__nacl_irt_mmap) (void **addr, size_t len, int prot, int flags,
                        int fd, off_t off) = nacl_irt_mmap;
int (*__nacl_irt_munmap) (void *addr, size_t len) = nacl_irt_munmap;

int (*__nacl_irt_dyncode_create) (void *dest, const void *src, size_t size) =
  nacl_irt_dyncode_create;
int (*__nacl_irt_dyncode_modify) (void *dest, const void *src, size_t size) =
  nacl_irt_dyncode_modify;
int (*__nacl_irt_dyncode_delete) (void *dest, size_t size) =
  nacl_irt_dyncode_delete;

int (*__nacl_irt_thread_create) (void *start_user_address, void *stack,
                                 void *thread_ptr) = nacl_irt_thread_create;
void (*__nacl_irt_thread_exit) (int32_t *stack_flag) =
  nacl_irt_thread_exit;
int (*__nacl_irt_thread_nice) (const int nice) =
  nacl_irt_thread_nice;

int (*__nacl_irt_mutex_create) (int *mutex_handle) = nacl_irt_mutex_create;
int (*__nacl_irt_mutex_destroy) (int mutex_handle) = nacl_irt_mutex_destroy;
int (*__nacl_irt_mutex_lock) (int mutex_handle) = nacl_irt_mutex_lock;
int (*__nacl_irt_mutex_unlock) (int mutex_handle) = nacl_irt_mutex_unlock;
int (*__nacl_irt_mutex_trylock) (int mutex_handle) = nacl_irt_mutex_trylock;

int (*__nacl_irt_cond_create) (int *cond_handle) = nacl_irt_cond_create;
int (*__nacl_irt_cond_destroy) (int cond_handle) = nacl_irt_cond_destroy;
int (*__nacl_irt_cond_signal) (int cond_handle) = nacl_irt_cond_signal;
int (*__nacl_irt_cond_broadcast) (int cond_handle) = nacl_irt_cond_broadcast;
int (*__nacl_irt_cond_wait) (int cond_handle, int mutex_handle) =
  nacl_irt_cond_wait;
int (*__nacl_irt_cond_timed_wait_abs) (int cond_handle, int mutex_handle,
  const struct timespec *abstime) = nacl_irt_cond_timed_wait_abs;

int (*__nacl_irt_tls_init) (void *tdb) = nacl_irt_tls_init;
void *(*__nacl_irt_tls_get) (void) = nacl_irt_tls_get;

int (*__nacl_irt_ppapi_start) (const struct PP_StartFunctions *) =
  not_implemented;
void (*__nacl_irt_ppapi_register_thread_creator) (
  const struct PP_ThreadFunctions *) = not_implemented;
int (*__nacl_irt_open_resource) (const char* file, int *fd) = not_implemented;
