/* Copyright (C) 1991, 1995, 1996, 1997 Free Software Foundation, Inc.
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

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

#include <zrt.h> //UMASK_ENV

/* Set the file creation mask to MASK, returning the old mask.  */
mode_t
__umask (mask)
     mode_t mask;
{
    int current_umask;
    const char* current_umask_str = getenv(UMASK_ENV);
    if ( current_umask_str != NULL ){
        sscanf( current_umask_str, "%o", &current_umask);
    }

    /*save new umask as envvar and return current*/
    char umask_str[11];
    sprintf( umask_str, "%o", mask );
    setenv( UMASK_ENV, umask_str, 1 );
    return current_umask;
}

weak_alias (__umask, umask)
#include <stub-tag.h>
