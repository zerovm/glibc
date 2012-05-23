#include <futex_emulation.h>

#include <assert.h>
#include <errno.h>
#include <irt_syscalls.h>
#include <tls.h>


static int global_futex_emulation_mutex_desc = -1;
static LIST_HEAD (waiters_list);

void __nacl_futex_init (void)
{
  assert (global_futex_emulation_mutex_desc == -1);
  __nacl_irt_mutex_create (&global_futex_emulation_mutex_desc);
}

void __nacl_futex_fini (void)
{
  __nacl_irt_mutex_destroy (global_futex_emulation_mutex_desc);
}

int __nacl_futex_wait (volatile int *addr, int val, unsigned int bitset,
		       const struct timespec *timeout)
{
  int retcode = -EINTR;

  if (__nacl_irt_mutex_lock (global_futex_emulation_mutex_desc))
    goto ret_no_unlock;

  if (*addr != val)
    {
      retcode = -EWOULDBLOCK;
      goto ret_unlock;
    }
  else
    {
      struct thread_wait_list_node *self = &THREAD_SELF->thread_wait_list_node;

      if (!self->desc_is_initialized)
	{
	  if (__nacl_irt_cond_create (&self->condvar_desc))
	    goto ret_unlock;
	  self->desc_is_initialized = 1;
	}

      self->addr = addr;
      self->bitset = bitset;

      list_add (&self->list, &waiters_list);

      if (timeout)
	{
	  retcode = __nacl_irt_cond_timed_wait_abs (
	      self->condvar_desc, global_futex_emulation_mutex_desc, timeout);
	  /* TODO(khim): Do something about linux<->NaCl error codes mix.  */
	  if (retcode == 110)
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
	  if (__nacl_irt_cond_wait (self->condvar_desc,
				    global_futex_emulation_mutex_desc))
	    retcode = -EINTR;
	  else
	    retcode = 0;
	}
      if (self->list.next)
        list_del (&self->list);
      /* Cannot really do anything if error happens here.  */
      __nacl_irt_mutex_unlock (global_futex_emulation_mutex_desc);
      return retcode;
    }

ret_unlock:
  if (!__nacl_irt_mutex_unlock (global_futex_emulation_mutex_desc))
    retcode = 0;
ret_no_unlock:
  return retcode;
}

/* Note: global_futex_emulation_mutex_desc must be taken when called.  */
static int nacl_futex_wake_nolock (volatile int *addr, int nwake,
				   unsigned int bitset,
				   int *count)
{
  int retcode = 0;
  list_t *entry;
  list_t *prev;
  *count = 0;

  if (nwake <= 0)
    return 0;

  list_for_each_prev_safe (entry, prev, &waiters_list)
    {
      struct thread_wait_list_node *curr;

      curr = list_entry (entry, struct thread_wait_list_node, list);

      if ((curr->addr == addr) && (curr->bitset & bitset))
	{
	  list_del (entry);
	  /* Mark the list entry as freed */
	  entry->next = NULL;
	  /* Cannot really do anything if error happens here.  */
	  __nacl_irt_cond_signal (curr->condvar_desc);
	  (*count)++;
	  nwake--;
	  if (nwake <= 0)
	    return 0;
	}
    }

  return retcode;
}

int __nacl_futex_wake (volatile int *addr, int nwake, unsigned int bitset,
		       int *count)
{
  int retcode = 0;

  if (__nacl_irt_mutex_lock (global_futex_emulation_mutex_desc))
    goto ret_no_unlock;

  retcode = nacl_futex_wake_nolock (addr, nwake, bitset, count);

  /* Cannot really do anything if error happens here.  */
  __nacl_irt_mutex_unlock (global_futex_emulation_mutex_desc);
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
			    } futex_operation,
			  int *count)
{
  int retcode = 0;
  int operation_argument;
  int old_value;

  if (__nacl_irt_mutex_lock (global_futex_emulation_mutex_desc))
    goto ret_no_unlock;

  operation_argument =
    futex_operation.decoded_wake_op_operation.operation_argument;
  if (futex_operation.decoded_wake_op_operation.operation_argument_shifted)
    operation_argument = 1 << operation_argument;

  switch (futex_operation.decoded_wake_op_operation.operation)
    {
      case 0/*FUTEX_OP_SET*/:
	old_value = __sync_lock_test_and_set (addr2, operation_argument);
	break;
      case 1/*FUTEX_OP_ADD*/:
	old_value = __sync_fetch_and_add (addr2, operation_argument);
	break;
      case 2/*FUTEX_OP_OR*/:
	old_value = __sync_fetch_and_or (addr2, operation_argument);
	break;
      case 3/*FUTEX_OP_ANDN*/:
	old_value = __sync_fetch_and_and (addr2, ~operation_argument);
	break;
      case 4/*FUTEX_OP_XOR*/:
	old_value = __sync_fetch_and_xor (addr2, operation_argument);
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

  retcode = nacl_futex_wake_nolock (addr1, nwake1, __FUTEX_BITSET_MATCH_ANY,
				    count);

  if (!retcode && old_value)
    {
      int count2;
      retcode = nacl_futex_wake_nolock (addr2, nwake2, __FUTEX_BITSET_MATCH_ANY,
					&count2);
      (*count) += count2;
    }

ret_unlock:
  /* Cannot really do anything if error happens here.  */
  __nacl_irt_mutex_unlock (global_futex_emulation_mutex_desc);
ret_no_unlock:
  return retcode;
}

int __nacl_futex_cmp_requeue (volatile int *addr1, volatile int *addr2,
			      int nwake, int nrequeue, int val, int *count)
{
  int retcode = -EINTR;

  if (__nacl_irt_mutex_lock (global_futex_emulation_mutex_desc))
    goto ret_no_unlock;

  if (*addr1 != val)
    {
      retcode = -EAGAIN;
      goto ret_unlock;
    }
  else
    {
      list_t *entry;
      retcode = nacl_futex_wake_nolock (addr1, nwake, __FUTEX_BITSET_MATCH_ANY,
					count);

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
	      count++;
	      nrequeue--;
	    }
	}

ret_unlock_nocheck:
      /* Cannot really do anything if error happens here.  */
      __nacl_irt_mutex_unlock (global_futex_emulation_mutex_desc);
      return retcode;
    }

ret_unlock:
  if (!__nacl_irt_mutex_unlock (global_futex_emulation_mutex_desc))
    retcode = 0;
ret_no_unlock:
  return retcode;
}
