#include <sysdep.h>

	.text
ENTRY(__fabsl)
	fldt	4(%esp)
	fabs
	NACLRET
END(__fabsl)
weak_alias (__fabsl, fabsl)
