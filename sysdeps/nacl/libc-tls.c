
#include <csu/libc-tls.c>
#include <dl-tls.h>

void *
__tls_get_addr (tls_index *ti)
{
  return (char *)THREAD_SELF - ti->ti_offset;
}
