#ifndef _NACL_RECEIVE_ARGS_H
#define _NACL_RECEIVE_ARGS_H

#include <stdlib.h>

#include "nacl_syscalls.h"

struct process_args;

struct process_args *argmsg_fetch (void);
size_t argmsg_get_size_on_stack (struct process_args *args);
void argmsg_move_to_stack (struct process_args *args,
                           void *buf, size_t buf_size);
void jump_to_elf_start (void *buf, uintptr_t entry_func, uintptr_t atexit_func);

/* We have a typedef for the size of argc on the stack because this
   varies between NaCl platforms.
   See http://code.google.com/p/nativeclient/issues/detail?id=1226
   TODO(mseaborn): Unify the ABIs.  */
#if defined(__x86_64__)
typedef int64_t argc_type;
#else
typedef int32_t argc_type;
#endif

#endif
