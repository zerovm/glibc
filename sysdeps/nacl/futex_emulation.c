#include <futex_emulation.h>

#include <assert.h>
#include <errno.h>
#include <nacl_syscalls.h>
#include <tls.h>


static int global_futex_emulation_mutex_desc = -1;
static LIST_HEAD (waiters_list);

void __nacl_futex_init (void)
{
  assert (global_futex_emulation_mutex_desc == -1);
  global_futex_emulation_mutex_desc = NACL_SYSCALL (mutex_create) ();
}

void __nacl_futex_fini (void)
{
  NACL_SYSCALL (close) (global_futex_emulation_mutex_desc);
}

int __nacl_futex_wait (volatile int *addr, int val, unsigned int bitset,
		       const struct timespec *timeout)
{
  int retcode = -EINTR;

  if (NACL_SYSCALL (mutex_lock) (global_futex_emulation_mutex_desc))
    goto ret_no_unlock;

  if (*addr != val)
    {
      retcode = -EWOULDBLOCK;
      goto ret_unlock;
    }
  else
    {
      struct thread_wait_list_node *self = &THREAD_SELF->thread_wait_list_node;

      if (!self->descs_are_initialized)
	{
	  self->mutex_desc = NACL_SYSCALL (mutex_create) ();
	  if (self->mutex_desc < 0)
	    goto ret_unlock;
	  self->condvar_desc = NACL_SYSCALL (cond_create) ();
	  if (self->condvar_desc < 0)
	    goto ret_unlock;
	  self->descs_are_initialized = 1;
	}

      self->addr = addr;
      self->bitset = bitset;

      list_add_tail (&self->list, &waiters_list);

      if (NACL_SYSCALL (mutex_lock) (self->mutex_desc))
	{
	  list_del (&self->list);

	  /* Cannot really do anything if error happens here.  */
	  NACL_SYSCALL (mutex_unlock) (global_futex_emulation_mutex_desc);
	  goto ret_no_unlock;
	}

      if (NACL_SYSCALL (mutex_unlock) (global_futex_emulation_mutex_desc))
	{
	  /* Cannot really do anything if error happens here.  */
	  NACL_SYSCALL (mutex_unlock) (self->mutex_desc);
	  goto ret_no_unlock;
	}

      if (timeout)
	{
	  retcode = NACL_SYSCALL (cond_timed_wait_abs) (self->condvar_desc,
							self->mutex_desc,
							timeout);
	  /* TODO(khim): Do something about linux<->NaCl error codes mix.  */
	  if (retcode == -116)
	    retcode = -ETIMEDOUT;
	  else if (retcode)
	    retcode = -EINTR;
	}
      else
	{
	  /* Usually cond_wait is placed in a loop since spurious wakeups are
	     possible.  But here we don't need the loop and in fact can't have
	     the loop:
	       1. Spurious wakeups are permitted as per futex API.
	       2. We don't actually have a clue what we are trying to achieve
	          here.  Only higher level code does - and it usually contains
	          the loop.  */
	  if (NACL_SYSCALL (cond_wait) (self->condvar_desc, self->mutex_desc))
	    retcode = -EINTR;
	  else
	    retcode = 0;
	}
      /* Cannot really do anything if error happens here.  */
      NACL_SYSCALL (mutex_unlock) (self->mutex_desc);
      return retcode;
    }

ret_unlock:
  if (!NACL_SYSCALL (mutex_unlock) (global_futex_emulation_mutex_desc))
    retcode = 0;
ret_no_unlock:
  return retcode;
}

static int nacl_futex_wake_nolock (volatile int *addr, int nwake,
				   unsigned int bitset)
{
  int retcode = 0;
  list_t *entry;
  list_t *prev;

  if (nwake <= 0)
    return retcode;

  list_for_each_prev_safe (entry, prev, &waiters_list)
    {
      struct thread_wait_list_node *curr;

      curr = list_entry (entry, struct thread_wait_list_node, list);

      if ((curr->addr == addr) && (curr->bitset & bitset))
	{
	  if (!NACL_SYSCALL (mutex_lock) (curr->mutex_desc))
	    {
	      list_del (entry);
	      /* Cannot really do anything if error happens here.  */
	      NACL_SYSCALL (cond_signal) (curr->condvar_desc);
	      NACL_SYSCALL (mutex_unlock) (curr->mutex_desc);
	      retcode++;
	      nwake--;
	      if (nwake <= 0)
	        return retcode;
	    }
	}
    }

  return retcode;
}

int __nacl_futex_wake (volatile int *addr, int nwake, unsigned int bitset)
{
  int retcode = 0;

  if (NACL_SYSCALL (mutex_lock) (global_futex_emulation_mutex_desc))
    goto ret_no_unlock;

  retcode = nacl_futex_wake_nolock (addr, nwake, bitset);

  /* Cannot really do anything if error happens here.  */
  NACL_SYSCALL (mutex_unlock) (global_futex_emulation_mutex_desc);
ret_no_unlock:
  return retcode;
}

struct __attribute__ ((__packed__)) decoded_wake_op_operation
  {
    unsigned int compare_argument		:12;
    unsigned int operation_argument		:12;
    unsigned int compare			:4;
    unsigned int operation			:3;
    unsigned int operation_argument_shifted	:1;
  };

int __nacl_futex_wake_op (volatile int *addr1, volatile int *addr2,
			  int nwake1, int nwake2,
			  union __attribute__ ((__transparent_union__))
			    {
			      int encoded_futex_operation;
			      struct decoded_wake_op_operation
			        decoded_wake_op_operation;
			    } futex_operation)
{
  int retcode = 0;
  int operation_argument;
  int old_value;

  if (NACL_SYSCALL (mutex_lock) (global_futex_emulation_mutex_desc))
    goto ret_no_unlock;

  operation_argument =
    futex_operation.decoded_wake_op_operation.operation_argument;
  if (futex_operation.decoded_wake_op_operation.operation_argument_shifted)
    operation_argument = 1 << operation_argument;

  switch (futex_operation.decoded_wake_op_operation.operation)
    {
      case 0/*FUTEX_OP_SET*/:
	old_value = __sync_lock_test_and_set (addr1, operation_argument);
	break;
      case 1/*FUTEX_OP_ADD*/:
	old_value = __sync_fetch_and_add (addr1, operation_argument);
	break;
      case 2/*FUTEX_OP_OR*/:
	old_value = __sync_fetch_and_or (addr1, operation_argument);
	break;
      case 3/*FUTEX_OP_ANDN*/:
	old_value = __sync_fetch_and_and (addr1, ~operation_argument);
	break;
      case 4/*FUTEX_OP_XOR*/:
	old_value = __sync_fetch_and_xor (addr1, operation_argument);
	break;
      default:
	retcode = -ENOSYS;
	goto ret_unlock;
    }

  switch (futex_operation.decoded_wake_op_operation.compare)
    {
      case 0/*FUTEX_OP_CMP_EQ*/:
	old_value = old_value
	  == futex_operation.decoded_wake_op_operation.compare_argument;
	break;
      case 1/*FUTEX_OP_CMP_NE*/:
	old_value = old_value
	  != futex_operation.decoded_wake_op_operation.compare_argument;
	break;
      case 2/*FUTEX_OP_CMP_LT*/:
	old_value = old_value
	  < futex_operation.decoded_wake_op_operation.compare_argument;
	break;
      case 3/*FUTEX_OP_CMP_LE*/:
	old_value = old_value
	  <= futex_operation.decoded_wake_op_operation.compare_argument;
	break;
      case 4/*FUTEX_OP_CMP_GT*/:
	old_value = old_value
	  > futex_operation.decoded_wake_op_operation.compare_argument;
	break;
      case 5/*FUTEX_OP_CMP_GE*/:
	old_value = old_value
	  >= futex_operation.decoded_wake_op_operation.compare_argument;
	break;
      default:
	retcode = -ENOSYS;
	goto ret_unlock;
    }

  retcode = nacl_futex_wake_nolock (addr1, nwake1, __FUTEX_BITSET_MATCH_ANY);

  if (old_value)
    retcode += nacl_futex_wake_nolock (addr1, nwake1, __FUTEX_BITSET_MATCH_ANY);

ret_unlock:
  /* Cannot really do anything if error happens here.  */
  NACL_SYSCALL (mutex_unlock) (global_futex_emulation_mutex_desc);
ret_no_unlock:
  return retcode;
}

int __nacl_futex_cmp_requeue (volatile int *addr1, volatile int *addr2,
			      int nwake, int nrequeue, int val)
{
  int retcode = -EINTR;

  if (NACL_SYSCALL (mutex_lock) (global_futex_emulation_mutex_desc))
    goto ret_no_unlock;

  if (*addr1 != val)
    {
      retcode = -EAGAIN;
      goto ret_unlock;
    }
  else
    {
      list_t *entry;
      retcode = nacl_futex_wake_nolock (addr1, nwake, __FUTEX_BITSET_MATCH_ANY);

      if (retcode <= 0 || (waiters_list.next == &waiters_list) || nrequeue <= 0)
	goto ret_unlock_nocheck;

      list_for_each_prev (entry, &waiters_list)
	{
	  struct thread_wait_list_node *curr;

	  if (!nrequeue)
	    break;

	  curr = list_entry (entry, struct thread_wait_list_node, list);

	  if (curr->addr == addr1)
	    {
	      curr->addr = addr2;
	      retcode++;
	      nrequeue--;
	    }
	}

ret_unlock_nocheck:
      /* Cannot really do anything if error happens here.  */
      NACL_SYSCALL (mutex_unlock) (global_futex_emulation_mutex_desc);
      return retcode;
    }

ret_unlock:
  if (!NACL_SYSCALL (mutex_unlock) (global_futex_emulation_mutex_desc))
    retcode = 0;
ret_no_unlock:
  return retcode;
}
