/* Copyright (C) 1996, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1996.

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

#include <pwd.h>
#include <errno.h>

#include <irt_zcalls.h>

#ifdef	HAVE_CONFIG_H
# include "config.h"
#endif

const struct passwd s_passwd_data_1 = {
    "username",      /* username */
    "password",      /* user password */
    USER_ID_STUB,    /* user ID */
    GROUP_ID_STUB,   /* group ID */
    "userinfo",      /* user information */
    "/",             /* home directory */
    "zrt shell"      /* shell program */
};


struct passwd* getpwuid(uid_t uid)
{
    if ( uid == 1 )
	return &s_passwd_data_1;
    else{
	__set_errno(ENOENT);    /*uid mismatch*/
    }
    return NULL;
}
