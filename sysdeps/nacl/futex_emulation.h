#ifndef _NACL_FUTEX_EMULATION_H
#define _NACL_FUTEX_EMULATION_H


#include <stdbool.h>
#include <time.h>
#include <list.h>

struct thread_wait_list_node
{
  list_t list;
  volatile int *addr;
  unsigned int bitset;
  int condvar_desc;
  bool desc_is_initialized;
};

#define __FUTEX_BITSET_MATCH_ANY 0xFFFFFFFF

void __nacl_futex_init (void);
void __nacl_futex_fini (void);

int __nacl_futex_wait (volatile int *addr, int val, unsigned int bitset,
		       const struct timespec *timeout);
int __nacl_futex_wake (volatile int *addr, int nwake, unsigned int bitset);
int __nacl_futex_wake_op (volatile int *addr1, volatile int *addr2, int nwake1,
			  int nwake2, int op);
int __nacl_futex_cmp_requeue (volatile int *addr1, volatile int *addr2, int nwake,
			      int nrequeue, int val);

#endif
