/* Set flags signalling availability of kernel features.
   Copyright (C) 1999-2006, 2007, 2008 Free Software Foundation, Inc.
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

/* This file must not contain any C code.  At least it must be protected
   to allow using the file also in assembler files.  */

/* We only enable a few features uncoditionally for now.
   TODO(khim): investigate feasibility of other flags.  */


#ifndef __LINUX_KERNEL_VERSION
/* Skip detection of RLIMIT_STACK support: GLibC works fine both when
   __getrlimit returns ENOSYS and when it returns sensible values.
   Only when __getrlimit returns garbage can it fail - and this should
   not happen with NaCl.  */
# define __LINUX_KERNEL_VERSION 0x020618
#endif

/* Use `getcwd' system call: we don't support /proc/proc anyway.  */
#define __ASSUME_GETCWD_SYSCALL		1

/* Don't try to use obsolete 16-bit uid/gid syscalls.  */
#define __ASSUME_32BITUIDS		1

/* Assume `lchown' syscall is available: versions without lchown will
   try to compile non-existing 16-bit uid/gid syscalls.  */
#define __ASSUME_LCHOWN_SYSCALL		1

/* Assume realtime signals support: version without realtime signals will
   try to use non-existing check for obsolete 16bit uid/gid syscalls.  */
#define __ASSUME_REALTIME_SIGNALS	1
