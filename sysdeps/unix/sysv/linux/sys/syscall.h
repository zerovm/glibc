/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _SYSCALL_H
#define _SYSCALL_H	1

/* This file should list the numbers of the system the system knows.
   But instead of duplicating this we use the information available
   from the kernel sources.  */
#include <asm/unistd.h>
#ifdef __i386__
#  define __NR_accept		2325
#  define __NR_bind		2326
#  define __NR_listen		2327
#  define __NR_connect		2328
#  define __NR_send		2329
#  define __NR_sendto		2330
#  define __NR_sendmsg		2331
#  define __NR_recv		2332
#  define __NR_recvfrom		2333
#  define __NR_recvmsg		2334
#  define __NR_shutdown		2335
#  define __NR_getsockopt		2336
#  define __NR_setsockopt		2337
#  define __NR_getsockname		2338
#  define __NR_getpeername		2339
#  define __NR_socketpair		2340
#  define __NR_socket		2341
#endif
#ifdef __x86_64__
#  define __NR_send		2329
#  define __NR_recv		2332
#endif

#ifndef _LIBC
/* The Linux kernel header file defines macros `__NR_<name>', but some
   programs expect the traditional form `SYS_<name>'.  So in building libc
   we scan the kernel's list and produce <bits/syscall.h> with macros for
   all the `SYS_' names.  */
# include <bits/syscall.h>
#endif

#endif
