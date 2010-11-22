
/* This file overrides sysdeps/nacl/syscalls.list for x86-32 and x86-64 NaCl.
   Also make_sysd_rules.py explicitly enforces the override.  It's because
   x86-64 build requires umount.o but does not generate a rule to compile it: no
   relevant entry in sysd-syscalls.  With no entry in syscalls.list the x64-32
   build picks an umount.S that it fails to compile.

   TODO(pasko): find a better way for an override. */

int __unimplemented_syscall(const char *name);

int __umount (void);
libc_hidden_proto (__umount);
int __umount (void)
{
  return __unimplemented_syscall ("umount");
}
strong_alias (__umount, __umount_nocancel);
libc_hidden_def (__umount);

int umount (void);
libc_hidden_proto (umount);
weak_alias (__umount, umount);
libc_hidden_weak (umount);
