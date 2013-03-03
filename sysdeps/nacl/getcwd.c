/* Copyright (C) 1991,92,93,94,95,96,97,98,99 Free Software Foundation, Inc.
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

#ifdef	HAVE_CONFIG_H
# include "config.h"
#endif

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#if !defined __GNU_LIBRARY__ && !defined STDC_HEADERS
extern int errno;
#endif
#ifndef __set_errno
# define __set_errno(val) errno = (val)
#endif

#ifndef	NULL
# define NULL	0
#endif

#if defined HAVE_UNISTD_H || defined __GNU_LIBRARY__
# include <unistd.h>
#endif

#ifndef MAX
# define MAX(a, b) ((a) < (b) ? (b) : (a))
#endif

#if defined HAVE_LIMITS_H || defined STDC_HEADERS || defined __GNU_LIBRARY__
# include <limits.h>
#else
# include <sys/param.h>
#endif

#ifndef PATH_MAX
# ifdef	MAXPATHLEN
#  define PATH_MAX MAXPATHLEN
# else
#  define PATH_MAX 1024
# endif
#endif

#if !defined STDC_HEADERS && !defined __GNU_LIBRARY__
# undef	size_t
# define size_t	unsigned int
#endif

#if !__STDC__ && !defined const
# define const
#endif

#ifndef __GNU_LIBRARY__
# define __lstat	stat
#endif

#ifndef _LIBC
# define __getcwd getcwd
#endif

#ifndef GETCWD_RETURN_TYPE
# define GETCWD_RETURN_TYPE char *
#endif

/* Get the pathname of the current working directory, and put it in SIZE
   bytes of BUF.  Returns NULL if the directory couldn't be determined or
   SIZE was too small.  If successful, returns BUF.  In GNU, if BUF is
   NULL, an array is allocated with `malloc'; the array is SIZE bytes long,
   unless SIZE == 0, in which case it is as big as necessary.  */

GETCWD_RETURN_TYPE
__getcwd(char *buf, size_t size)
{
    size_t allocated = size;
    char *path;
    if (size == 0){
	if (buf != NULL)
	    {
		__set_errno (EINVAL);
		return NULL;
	    }

	allocated = PATH_MAX + 1;
    }

    if (buf != NULL)
	path = buf;
    else{
	path = malloc (allocated);
	if (path == NULL)
	    return NULL;
    }

    if ( allocated > 1 ){
	memcpy(path, "/\0", 2);
	return path;
    }
    else{
	__set_errno (ERANGE);
	return NULL;
    }
}

#if defined _LIBC && !defined __getcwd
weak_alias (__getcwd, getcwd)
#endif
