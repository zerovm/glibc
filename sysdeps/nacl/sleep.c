/* Implementation of the POSIX sleep function using nanosleep.
   Copyright (C) 2012 Free Software Foundation, Inc.
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

#include <time.h>
#include <unistd.h>

unsigned int
__sleep (unsigned int seconds)
{
  struct timespec ts = { seconds, 0 };
  if (__nanosleep (&ts, &ts) == 0)
    return 0;

  /* Round up remaining time.  */
  return (unsigned int) ts.tv_sec + (ts.tv_nsec >= 500000000L);
}
weak_alias (__sleep, sleep)
