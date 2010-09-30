#include <sysdep.h>

	.text
ENTRY(__fabs)
	fldl	4(%esp)
	fabs
	NACLRET
END(__fabs)
weak_alias (__fabs, fabs)
