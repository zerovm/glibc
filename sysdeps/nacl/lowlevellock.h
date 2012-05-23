/* Copyright (C) 2003, 2004, 2006, 2007 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>, 2003.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _LOWLEVELLOCK_H
#define _LOWLEVELLOCK_H	1

#include <time.h>
#include <sys/param.h>
#include <bits/pthreadtypes.h>
#include <kernel-features.h>

/* FUTEX_WAIT is not implemented under NaCl */
#define FUTEX_WAIT_ABS		32
#define FUTEX_WAKE		1
#define FUTEX_REQUEUE		3
#define FUTEX_CMP_REQUEUE	4
#define FUTEX_WAKE_OP		5
#define FUTEX_OP_CLEAR_WAKE_IF_GT_ONE	((4 << 24) | 1)
#define FUTEX_LOCK_PI		6
#define FUTEX_UNLOCK_PI		7
#define FUTEX_TRYLOCK_PI	8
#define FUTEX_PRIVATE_FLAG	128

/* Atomic and errno need TLS, TLS needs futex emulation so first define
   constants, then include these files.  */
# include <errno.h>
# include <atomic.h>

/* Values for 'private' parameter of locking macros.  Yes, the
   definition seems to be backwards.  But it is not.  The bit will be
   reversed before passing to the system call.  */
#define LLL_PRIVATE	0
#define LLL_SHARED	FUTEX_PRIVATE_FLAG


#if !defined NOT_IN_libc || defined IS_IN_rtld
/* In libc.so or ld.so all futexes are private.  */
# ifdef __ASSUME_PRIVATE_FUTEX
#  define __lll_private_flag(fl, private) \
  ((fl) | FUTEX_PRIVATE_FLAG)
# else
#  define __lll_private_flag(fl, private) \
  ((fl) | THREAD_GETMEM (THREAD_SELF, header.private_futex))
# endif
#else
# ifdef __ASSUME_PRIVATE_FUTEX
#  define __lll_private_flag(fl, private) \
  (((fl) | FUTEX_PRIVATE_FLAG) ^ (private))
# else
#  define __lll_private_flag(fl, private) \
  (__builtin_constant_p (private)					      \
   ? ((private) == 0							      \
      ? ((fl) | THREAD_GETMEM (THREAD_SELF, header.private_futex))	      \
      : (fl))								      \
   : ((fl) | (((private) ^ FUTEX_PRIVATE_FLAG)				      \
	      & THREAD_GETMEM (THREAD_SELF, header.private_futex))))
# endif	      
#endif


/* Delay in spinlock loop.  */
#define BUSY_WAIT_NOP	asm ("rep; nop")


#define lll_futex_wait(futex, val, private) \
  lll_futex_timed_wait_abs (futex, val, NULL, private)

#define lll_futex_timed_wait_abs(ftx, val, timespec, private)		\
    ({									\
      INTERNAL_SYSCALL_DECL (err);					\
      INTERNAL_SYSCALL (futex, err, 4, (ftx),				\
			__lll_private_flag (FUTEX_WAIT_ABS, private),	\
			(val), (timespec));				\
      if (__builtin_expect (err, 0))					\
	__set_errno (err);						\
      -err;								\
    })

#define lll_futex_wake(ftx, nr, private)				\
   INLINE_SYSCALL(futex, 3, (ftx),					\
		  __lll_private_flag (FUTEX_WAKE, private),		\
		  (nr))

#define lll_robust_dead(futexv, private)				\
do									\
  {									\
    int *__futexp = &(futexv);						\
    atomic_or (__futexp, FUTEX_OWNER_DIED);				\
    INLINE_SYSCALL(futex, 3, __futexp,					\
		   __lll_private_flag (FUTEX_WAKE, private), 1);	\
  }									\
while (0)

/* Returns non-zero if error happened, zero if success.  */
#define lll_futex_requeue(ftx, nr_wake, nr_move, mutex, val, private)	     \
   INLINE_SYSCALL(futex, 6, (ftx),					     \
		  __lll_private_flag (FUTEX_CMP_REQUEUE, private),	     \
		  (nr_wake), (nr_move), (mutex),			     \
		  val)

/* Returns non-zero if error happened, zero if success.  */
#define lll_futex_wake_unlock(ftx, nr_wake, nr_wake2, ftx2, private)	     \
   INLINE_SYSCALL(futex, 6, (ftx),					     \
		  __lll_private_flag (FUTEX_WAKE_OP, private),		     \
		  (nr_wake), (nr_wake2), (ftx2),			     \
		  FUTEX_OP_CLEAR_WAKE_IF_GT_ONE)


#define __lll_trylock(futex) \
  (atomic_compare_and_exchange_val_acq (futex, 1, 0) != 0)
#define lll_trylock(futex) __lll_trylock (&(futex))


#define __lll_robust_trylock(futex, id) \
  (atomic_compare_and_exchange_val_acq (futex, id, 0) != 0)
#define lll_robust_trylock(futex, id) \
  __lll_robust_trylock (&(futex), id)


#define __lll_cond_trylock(futex) \
  (atomic_compare_and_exchange_val_acq (futex, 2, 0) != 0)
#define lll_cond_trylock(futex) __lll_cond_trylock (&(futex))


extern void __lll_lock_wait_private (int *futex) attribute_hidden;
extern void __lll_lock_wait (int *futex, int private) attribute_hidden;
extern int __lll_robust_lock_wait (int *futex, int private) attribute_hidden;


#define __lll_lock(futex, private)					      \
  ((void) ({								      \
    int *__futex = (futex);						      \
    if (__builtin_expect (atomic_compare_and_exchange_bool_acq (__futex,      \
								1, 0), 0))    \
      {									      \
	if (__builtin_constant_p (private) && (private) == LLL_PRIVATE)	      \
	  __lll_lock_wait_private (__futex);				      \
	else								      \
	  __lll_lock_wait (__futex, private);				      \
      }									      \
  }))
#define lll_lock(futex, private) __lll_lock (&(futex), private)


#define __lll_robust_lock(futex, id, private)				      \
  ({									      \
    int *__futex = (futex);						      \
    int __val = 0;							      \
									      \
    if (__builtin_expect (atomic_compare_and_exchange_bool_acq (__futex, id,  \
								0), 0))	      \
      __val = __lll_robust_lock_wait (__futex, private);		      \
    __val;								      \
  })
#define lll_robust_lock(futex, id, private) \
  __lll_robust_lock (&(futex), id, private)


#define __lll_cond_lock(futex, private)					      \
  ((void) ({								      \
    int *__futex = (futex);						      \
    if (__builtin_expect (atomic_compare_and_exchange_bool_acq (__futex, 2,   \
								0), 0))	      \
      __lll_lock_wait (__futex, private);				      \
  }))
#define lll_cond_lock(futex, private) __lll_cond_lock (&(futex), private)


#define __lll_robust_cond_lock(futex, id, private)			      \
  ({									      \
    int *__futex = (futex);						      \
    int __val = 0;							      \
    int __id = (id) | FUTEX_WAITERS;					      \
									      \
    if (__builtin_expect (atomic_compare_and_exchange_bool_acq (__futex,      \
								__id, 0), 0)) \
      __val = __lll_robust_lock_wait (__futex, private);		      \
    __val;								      \
  })
#define lll_robust_cond_lock(futex, id, private) \
  __lll_robust_cond_lock (&(futex), id, private)


extern int __lll_timedlock_wait (int *futex, const struct timespec *,
				 int private) attribute_hidden;
extern int __lll_robust_timedlock_wait (int *futex, const struct timespec *,
					int private) attribute_hidden;


#define __lll_timedlock(futex, abstime, private)			      \
  ({									      \
     int *__futex = (futex);						      \
     int __val = 0;							      \
									      \
     if (__builtin_expect (atomic_compare_and_exchange_bool_acq (__futex, 1,  \
								 0), 0))      \
       __val = __lll_timedlock_wait (__futex, abstime, private);	      \
     __val;								      \
  })
#define lll_timedlock(futex, abstime, private) \
  __lll_timedlock (&(futex), abstime, private)


#define __lll_robust_timedlock(futex, abstime, id, private)		      \
  ({									      \
    int *__futex = (futex);						      \
    int __val = 0;							      \
									      \
    if (__builtin_expect (atomic_compare_and_exchange_bool_acq (__futex, id,  \
								0), 0))	      \
      __val = __lll_robust_timedlock_wait (__futex, abstime, private);	      \
    __val;								      \
  })
#define lll_robust_timedlock(futex, abstime, id, private) \
  __lll_robust_timedlock (&(futex), abstime, id, private)


#define __lll_unlock(futex, private)					      \
  ((void) ({								      \
    int *__futex = (futex);						      \
    int __val = atomic_exchange_rel (__futex, 0);			      \
									      \
    if (__builtin_expect (__val > 1, 0))				      \
      lll_futex_wake (__futex, 1, private);				      \
  }))
#define lll_unlock(futex, private) __lll_unlock(&(futex), private)


#define __lll_robust_unlock(futex, private)				      \
  ((void) ({								      \
    int *__futex = (futex);						      \
    int __val = atomic_exchange_rel (__futex, 0);			      \
									      \
    if (__builtin_expect (__val & FUTEX_WAITERS, 0))			      \
      lll_futex_wake (__futex, 1, private);				      \
  }))
#define lll_robust_unlock(futex, private) \
  __lll_robust_unlock(&(futex), private)


#define lll_islocked(futex) \
  (futex != 0)

/* Initializers for lock.  */
#define LLL_LOCK_INITIALIZER		(0)
#define LLL_LOCK_INITIALIZER_LOCKED	(1)

/* NaCl can not wake up us after the death of the thread.  Instead thread
   which is about to die changes tid to -2, wakes us up and then calls TCB.
   TCB will clear the tid field and this is when the thread is truly dead.
   We are waiting in a tight loop here (note: tid filed is volatile).  */
#define lll_wait_tid(tid) \
  do							\
    {							\
      __typeof (tid) __tid;				\
      volatile __typeof (tid) *__ptid = &tid;		\
      while ((__tid = (*__ptid)) != 0)			\
	if (__tid != -2)				\
	  lll_futex_wait (&(tid), __tid, LLL_SHARED);	\
    }							\
  while (0)

extern int __lll_timedwait_tid (int *, const struct timespec *)
     attribute_hidden;

#define lll_timedwait_tid(tid, abstime) \
  ({							\
    int __res = 0;					\
    if ((tid) != 0)					\
      __res = __lll_timedwait_tid (&(tid), (abstime));	\
    __res;						\
  })

#endif	/* lowlevellock.h */
