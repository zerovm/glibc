#ifndef _NACL_ASM_H
#define _NACL_ASM_H

#ifdef __native_client__
# define NACLRET popl %ecx; nacljmp %ecx
#else
# define NACLRET ret
#endif

#endif
