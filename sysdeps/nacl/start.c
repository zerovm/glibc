/* This is the universal C startup code (crt1.o) for NaCl applications.
   The initial stack and register state is arranged by the NaCl trusted
   runtime as for a normal C function call of one argument.  */

#include <stdint.h>

int main (int argc, char **argv, char **envp);

int __libc_csu_init (int argc, char **argv, char **envp);
void __libc_csu_fini (void);

void __libc_start_main (int (*main) (int argc, char **argv, char **envp),
                        int argc, char **argv,
                        int (*init) (int argc, char **argv, char **envp),
                        void (*fini) (void),
                        void (*rtld_fini) (void),
                        void *stack_end);

/* This is the true entry point for untrusted code.
   It's called with the normal C ABI, taking one argument.
   This is a pointer to stack space containing these words:
   [0]             cleanup function pointer (always NULL in actual startup)
   [1]             envc, count of envp[] pointers
   [2]             argc, count of argv[] pointers
   [3]             argv[0..argc] pointers, argv[argc] being NULL
   [3+argc]        envp[0..envc] pointers, envp[envc] being NULL
   [3+argc+envc]   auxv[] pairs
*/
void
_start (uint32_t *info)
{
  void (*rtld_fini) (void) = (void (*) (void)) info[0];
  int argc = info[2];
  char **argv = (void *) &info[3];

  /* The generic code actually assumes that envp follows argv
     and that auxv follows envp.  */

  __libc_start_main (&main, argc, argv,
                     &__libc_csu_init, &__libc_csu_fini, rtld_fini,
                     __builtin_frame_address (0));

  /* That should not return.  Make sure we crash if it did.  */
  while (1) *(volatile int *) 0;
}
