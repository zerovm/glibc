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

/*ZRT uses folowing envirnoment variables to init struct passwd :
LOGNAME - pw_name
UID     - pw_uid
GID     - pw_gid
HOME    - pw_dir
SHELL   - pw_shell
*/

const struct passwd s_default_passwd = {
    "root",          /* default username */
    "x",             /* default user password */
    USER_ID_STUB,    /* default user ID */
    GROUP_ID_STUB,   /* default group ID */
    "",              /* default user information */
    "/",             /* default home directory */
    ""               /* default shell program */
};


struct passwd* getpwuid(uid_t uid)
{
    char *env;
    if ( uid == getuid() ){
	static struct passwd passwd;
	passwd = s_default_passwd;

	if ( (env=getenv(PASSWD_NAME)) != NULL )
	    passwd.pw_name = env;
	if ( (env=getenv(PASSWD_HOME)) != NULL )
	    passwd.pw_dir = env;
	if ( (env=getenv(PASSWD_SHELL)) != NULL )
	    passwd.pw_shell = env;

	passwd.pw_uid = getuid();
	passwd.pw_gid = getgid();
	return &passwd;
    }
    else{
	__set_errno(ENOENT);    /*uid mismatch*/
    }
    return NULL;
}
