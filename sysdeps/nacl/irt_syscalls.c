#include <sys/stat.h>
#include <nacl_stat.h>
#include <nacl_syscalls.h>
#define stat nacl_abi_stat
#include <irt.h>
#undef stat
#include <irt_syscalls.h>
#ifdef IS_IN_rtld
#include <ldsodefs.h>
#endif

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

static int nacl_irt_open_resource (const char *pathname, int *newfd) {
  return __nacl_irt_open(pathname, O_RDONLY, 0, newfd);
}

static size_t no_interface(const char *interface_ident,
                           void *table, size_t tablesize) {
  return 0;
}

size_t (*__nacl_irt_query) (const char *interface_ident,
			    void *table, size_t tablesize);

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
int (*__nacl_irt_fstat) (int fd, struct nacl_abi_stat *);
int (*__nacl_irt_stat) (const char *pathname, struct nacl_abi_stat *);
int (*__nacl_irt_getdents) (int fd, struct dirent *, size_t count,
			    size_t *nread);

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

void
init_irt_table (void)
{
  union {
    struct nacl_irt_basic nacl_irt_basic;
    struct nacl_irt_fdio nacl_irt_fdio;
    struct nacl_irt_filename nacl_irt_filename;
    struct nacl_irt_memory nacl_irt_memory;
    struct nacl_irt_dyncode nacl_irt_dyncode;
    struct nacl_irt_thread nacl_irt_thread;
    struct nacl_irt_mutex nacl_irt_mutex;
    struct nacl_irt_cond nacl_irt_cond;
    struct nacl_irt_tls nacl_irt_tls;
    struct nacl_irt_resource_open nacl_irt_resource_open;
  } u;

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_BASIC_v0_1, &u.nacl_irt_basic,
			sizeof(u.nacl_irt_basic)) == sizeof(u.nacl_irt_basic))
    {
      __nacl_irt_exit = u.nacl_irt_basic.exit;
      __nacl_irt_gettod = u.nacl_irt_basic.gettod;
      __nacl_irt_clock = u.nacl_irt_basic.clock;
      __nacl_irt_nanosleep = u.nacl_irt_basic.nanosleep;
      __nacl_irt_sched_yield = u.nacl_irt_basic.sched_yield;
      __nacl_irt_sysconf = u.nacl_irt_basic.sysconf;
    }
  else
    {
      __nacl_irt_exit = nacl_irt_exit;
      __nacl_irt_gettod = nacl_irt_gettod;
      __nacl_irt_clock = nacl_irt_clock;
      __nacl_irt_nanosleep = nacl_irt_nanosleep;
      __nacl_irt_sched_yield = nacl_irt_sched_yield;
      __nacl_irt_sysconf = nacl_irt_sysconf;
    }

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_FDIO_v0_1, &u.nacl_irt_fdio,
		        sizeof(u.nacl_irt_fdio)) == sizeof(u.nacl_irt_fdio))
    {
      __nacl_irt_close = u.nacl_irt_fdio.close;
      __nacl_irt_dup = u.nacl_irt_fdio.dup;
      __nacl_irt_dup2 = u.nacl_irt_fdio.dup2;
      __nacl_irt_read = u.nacl_irt_fdio.read;
      __nacl_irt_write = u.nacl_irt_fdio.write;
      __nacl_irt_seek = u.nacl_irt_fdio.seek;
      __nacl_irt_fstat = u.nacl_irt_fdio.fstat;
      __nacl_irt_getdents = u.nacl_irt_fdio.getdents;
    }
  else
    {
      __nacl_irt_close = nacl_irt_close;
      __nacl_irt_dup = nacl_irt_dup;
      __nacl_irt_dup2 = nacl_irt_dup2;
      __nacl_irt_read = nacl_irt_read;
      __nacl_irt_write = nacl_irt_write;
      __nacl_irt_seek = nacl_irt_seek;
      __nacl_irt_fstat = nacl_irt_fstat;
      __nacl_irt_getdents = nacl_irt_getdents;
    }

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_FILENAME_v0_1, &u.nacl_irt_filename,
		    sizeof(u.nacl_irt_filename)) == sizeof(u.nacl_irt_filename))
    {
      __nacl_irt_open = u.nacl_irt_filename.open;
      __nacl_irt_stat = u.nacl_irt_filename.nacl_abi_stat;
    }
  else
    {
      __nacl_irt_open = nacl_irt_open;
      __nacl_irt_stat = nacl_irt_stat;
    }

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_MEMORY_v0_1, &u.nacl_irt_memory,
		        sizeof(u.nacl_irt_memory)) == sizeof(u.nacl_irt_memory))
    {
      __nacl_irt_sysbrk = u.nacl_irt_memory.sysbrk;
      __nacl_irt_mmap = u.nacl_irt_memory.mmap;
      __nacl_irt_munmap = u.nacl_irt_memory.munmap;
    }
  else
    {
      __nacl_irt_sysbrk = nacl_irt_sysbrk;
      __nacl_irt_mmap = nacl_irt_mmap;
      __nacl_irt_munmap = nacl_irt_munmap;
    }

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_DYNCODE_v0_1, &u.nacl_irt_dyncode,
		      sizeof(u.nacl_irt_dyncode)) == sizeof(u.nacl_irt_dyncode))
    {
      __nacl_irt_dyncode_create = u.nacl_irt_dyncode.dyncode_create;
      __nacl_irt_dyncode_modify = u.nacl_irt_dyncode.dyncode_modify;
      __nacl_irt_dyncode_delete = u.nacl_irt_dyncode.dyncode_delete;
    }
  else
    {
      __nacl_irt_dyncode_create = nacl_irt_dyncode_create;
      __nacl_irt_dyncode_modify = nacl_irt_dyncode_modify;
      __nacl_irt_dyncode_delete = nacl_irt_dyncode_delete;
    }

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_THREAD_v0_1, &u.nacl_irt_thread,
			sizeof(u.nacl_irt_thread)) == sizeof(u.nacl_irt_thread))
    {
      __nacl_irt_thread_create = u.nacl_irt_thread.thread_create;
      __nacl_irt_thread_exit = u.nacl_irt_thread.thread_exit;
      __nacl_irt_thread_nice = u.nacl_irt_thread.thread_nice;
    }
  else
    {
      __nacl_irt_thread_create = nacl_irt_thread_create;
      __nacl_irt_thread_exit = nacl_irt_thread_exit;
      __nacl_irt_thread_nice = nacl_irt_thread_nice;
    }

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_MUTEX_v0_1, &u.nacl_irt_mutex,
		        sizeof(u.nacl_irt_mutex)) == sizeof(u.nacl_irt_mutex))
    {
      __nacl_irt_mutex_create = u.nacl_irt_mutex.mutex_create;
      __nacl_irt_mutex_destroy = u.nacl_irt_mutex.mutex_destroy;
      __nacl_irt_mutex_lock = u.nacl_irt_mutex.mutex_lock;
      __nacl_irt_mutex_unlock = u.nacl_irt_mutex.mutex_unlock;
      __nacl_irt_mutex_trylock = u.nacl_irt_mutex.mutex_trylock;
    }
  else
    {
      __nacl_irt_mutex_create = nacl_irt_mutex_create;
      __nacl_irt_mutex_destroy = nacl_irt_mutex_destroy;
      __nacl_irt_mutex_lock = nacl_irt_mutex_lock;
      __nacl_irt_mutex_unlock = nacl_irt_mutex_unlock;
      __nacl_irt_mutex_trylock = nacl_irt_mutex_trylock;
    }

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_COND_v0_1, &u.nacl_irt_cond,
		        sizeof(u.nacl_irt_cond)) == sizeof(u.nacl_irt_cond))
    {
      __nacl_irt_cond_create = u.nacl_irt_cond.cond_create;
      __nacl_irt_cond_destroy = u.nacl_irt_cond.cond_destroy;
      __nacl_irt_cond_signal = u.nacl_irt_cond.cond_signal;
      __nacl_irt_cond_broadcast = u.nacl_irt_cond.cond_broadcast;
      __nacl_irt_cond_wait = u.nacl_irt_cond.cond_wait;
      __nacl_irt_cond_timed_wait_abs = u.nacl_irt_cond.cond_timed_wait_abs;
    }
  else
    {
      __nacl_irt_cond_create = nacl_irt_cond_create;
      __nacl_irt_cond_destroy = nacl_irt_cond_destroy;
      __nacl_irt_cond_signal = nacl_irt_cond_signal;
      __nacl_irt_cond_broadcast = nacl_irt_cond_broadcast;
      __nacl_irt_cond_wait = nacl_irt_cond_wait;
      __nacl_irt_cond_timed_wait_abs = nacl_irt_cond_timed_wait_abs;
    }

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_TLS_v0_1, &u.nacl_irt_tls,
		        sizeof(u.nacl_irt_tls)) == sizeof(u.nacl_irt_tls))
    {
      __nacl_irt_tls_init = u.nacl_irt_tls.tls_init;
      __nacl_irt_tls_get = u.nacl_irt_tls.tls_get;
    }
  else
    {
      __nacl_irt_tls_init = nacl_irt_tls_init;
      __nacl_irt_tls_get = nacl_irt_tls_get;
    }

  if (__nacl_irt_query &&
      __nacl_irt_query (NACL_IRT_RESOURCE_OPEN_v0_1, &u.nacl_irt_resource_open,
	  sizeof(u.nacl_irt_resource_open)) == sizeof(u.nacl_irt_resource_open))
    {
      __nacl_irt_open_resource = u.nacl_irt_resource_open.open_resource;
#ifdef IS_IN_rtld
      if (_dl_argc == 1)
        {
          static const char *argv[] = { "/runnable-ld.so", "/main.nexe", 0 };
          _dl_argc = 2;
          _dl_argv = (char **)argv;
        }
#endif
    }
  else
    __nacl_irt_open_resource = nacl_irt_open_resource;

  if (!__nacl_irt_query)
    __nacl_irt_query = no_interface;
}

size_t nacl_interface_query(const char *interface_ident,
                            void *table, size_t tablesize) {
  return (*__nacl_irt_query)(interface_ident, table, tablesize);
}
