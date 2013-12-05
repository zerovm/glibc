#include <errno.h>
#include <irt_syscalls.h>
#include <irt_zcalls.h>  //__zcall_get_phys_pages, __zcall_get_phys_pages

/* Return the number of processors configured on the system. */
int
__get_nprocs_conf ()
{
  int result = 1;

  if (__nacl_irt_sysconf (1 /*NACL_ABI__SC_NPROCESSORS_ONLN*/, &result))
    return 1;
  return result;
}

/* Return the number of processors currently available on the system. */
int
__get_nprocs ()
{
  int result = 1;

  if (__nacl_irt_sysconf (1 /*NACL_ABI__SC_NPROCESSORS_ONLN*/, &result))
    return 1;
  return result;
}

/* Return the number of physical pages on the system. */
long int
__get_phys_pages ()
{
    return __zcall_get_phys_pages();
}

/* Return the number of available physical pages */
long int
__get_avphys_pages ()
{
    return __zcall_get_avphys_pages();
}
