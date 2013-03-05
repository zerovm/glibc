/* Copyright (C) 1992, 1994, 1995, 1996, 1997 Free Software Foundation, Inc.
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

#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include <irt_zcalls.h>

/* Create a directory named PATH with protections MODE.  */
int
__mkdir (path, mode)
     const char *path;
     mode_t mode;
{
    mode_t mask;
    struct stat statbuf;

    if (path == NULL)
	{
	    __set_errno (EINVAL);
	    return -1;
	}

    /* Check for some errors.  */
    if (__stat (path, &statbuf) < 0)
	{
	    if (errno != ENOENT)
		return -1;
	    /* There is no file by that name.  Good.  */
	}
    else
	{
	    __set_errno (EEXIST);
	    return -1;
	}

    /* Race condition, but how else to do it?  */
    mask = __umask (0777);
    (void) __umask (mask);
    mode &= ~mask;

    /*zcall*/
    if( __zcall_mkdir(path, mode) != 0 ){
	return -1;
    }
    else{
	return 0;
    }
}

weak_alias (__mkdir, mkdir)
