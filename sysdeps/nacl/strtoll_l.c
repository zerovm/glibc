/* For NaCl we want the normal version, never the wordsize-64 version.  */
#include <bits/wordsize.h>

#undef __WORDSIZE
#define __WORDSIZE 32

#include <stdlib/strtoll_l.c>
