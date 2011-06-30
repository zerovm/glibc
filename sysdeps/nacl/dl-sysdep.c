/* The COOKIE is the argument to _dl_start, which is passed through from
   the first C function argument set up by the NaCl trusted runtime's
   startup sequence.  We extract the arguments et al from here rather
   than from the traditional Unix/ELF stack setup.  */

#define DL_FIND_ARG_COMPONENTS(cookie, argc, argv, envp, auxp)	\
  do {                                                          \
    uint32_t *_info = (void *) (cookie);                        \
    int _envc = _info[1];                                       \
    (argc) = _info[2];                                          \
    (argv) = (void *) &_info[3];                                \
    (envp) = &(argv)[(argc) + 1];                               \
    (auxp) = (void *) &(envp)[_envc + 1];                       \
  } while (0)

#include <sysdeps/unix/sysv/linux/dl-sysdep.c>
