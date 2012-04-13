#ifndef _NACL_ASM_H
#define _NACL_ASM_H

#ifdef __native_client__
# define NACLRET popl %ecx; nacljmp %ecx
# define NACL_FSINCOS fld %st(0) ; fsin ; fxch ; fcos
#else
# define NACLRET ret
# define NACL_FSINCOS fsincos
#endif

#endif
