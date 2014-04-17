/* Copyright (C) 1991, 1993, 1995, 1996, 1997 Free Software Foundation, Inc.
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
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>

#include <irt_zcalls.h>

static uint strtouint_nolocale(const char* str, int base, int *err ){
    #define CURRENT_CHAR str[idx]
    int idx;
    uint delta;
    int numlen = strlen(str);
    uint res = 0;
    uint append=1;
    for ( idx=numlen-1; idx >= 0; idx-- ){
	if ( CURRENT_CHAR >= '0' && CURRENT_CHAR <= '9' ){
	    delta = append* (uint)(CURRENT_CHAR - '0');
	    if ( !(delta > UINT_MAX-res) )
		res += delta;
	    else{
		res=0;
		*err = 1;
		return 0;
	    }
	    append *= base;
	}
    }
    return res;
}

/* Get the real group ID of the calling process.  */
gid_t
__getgid ()
{
    long int gid=0;
    int err=0;
    char *env;
    if ( (env=getenv(PASSWD_GID)) == NULL || 
	 (gid=strtouint_nolocale(env, 10, &err )) <= 0 ||
	 err!=0 ){
	gid = GROUP_ID_STUB;
    }
    return gid;
}

weak_alias (__getgid, getgid)

