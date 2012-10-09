/*-------------------------------------------------------------------.
| Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>            |
|                                                                    |
| This program is free software; you can redistribute it and/or      |
| modify it under the terms of the GNU General Public License as     |
| published by the Free Software Foundation; either version 2 of the |
| License, or (at your option) any later version.                    |
|                                                                    |
| This program is distributed in the hope that it will be useful,    |
| but WITHOUT ANY WARRANTY; without even the implied warranty of     |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  |
| General Public License for more details.                           |
|                                                                    |
| You should have received a copy of the GNU General Public License  |
| along with this program; if not, write to the Free Software        |
| Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA           |
| 02111-1307 USA                                                     |
`-------------------------------------------------------------------*/

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <unistd.h>
#include <stdio.h>

#if HAVE_FCNTL_H
# include <fcntl.h>
#endif

#if HAVE_SYS_FILE_H
# include <sys/file.h>
#endif

/* These two files are for Windows (MinGW actually).  */
#if HAVE_IO_H
# include <io.h>
#endif
#if HAVE_SYS_LOCKING_H
# include <sys/locking.h>
#endif

#include "filelock.h"

/* fcntl() locking is prefered because it works over NFS, otherwise
   we fall back to flock(), or _locking() (this one is for Windows).  */

int
file_lock (int fd, int exclusive)
{
#if HAVE_FCNTL && defined(F_SETLKW)

  struct flock lock;
  lock.l_type = exclusive ? F_WRLCK : F_RDLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;		/* Lock the whole file.  */
  return fcntl (fd, F_SETLKW, &lock);

#else /* !(HAVE_FCNTL && defined(F_SETLKW)) */
# if HAVE_FLOCK

  return flock (fd, exclusive ? LOCK_EX : LOCK_SH);

# else /* !HAVE_FLOCK */
#  if HAVE__LOCKING

  /* Lock the 10 next bytes.  */
  return _locking (fd, exclusive ? _LK_LOCK : _LK_RLCK, 10);

#  else /* !HAVE__LOCKING */

  /* FIXME: implement other kind of locking for system.  */
  return -1;

#  endif /* !HAVE_LOCKING */
# endif /* !HAVE_FLOCK */
#endif /* !(HAVE_FCNTL && defined(F_SETLKW)) */
}

int
file_unlock (int fd)
{
#if HAVE_FCNTL && defined(F_SETLKW)

  struct flock lock;
  lock.l_type = F_UNLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;		/* Unlock the whole file.  */
  return fcntl (fd, F_SETLKW, &lock);

#else /* !(HAVE_FCNTL && defined(F_SETLKW)) */
# if HAVE_FLOCK

  return flock (fd, LOCK_UN);

# else /* !HAVE_FLOCK */
#  if HAVE__LOCKING

  /* Some version of MinGW used to spell _LK_UNLCK with an O.  */
#   ifndef _LK_UNLCK
#    ifdef _LK_UNLOCK
#     define _LK_UNLCK _LK_UNLOCK
#    endif
#   endif
  /* Unlock the 10 next bytes.  */
  return _locking (fd, _LK_UNLCK, 10);

#  else /* !HAVE__LOCKING */

  /* FIXME: implement other kind of locking for system.  */
  return -1;

#  endif /* !HAVE__LOCKING */
# endif /* !HAVE_FLOCK */
#endif /* !(HAVE_FCNTL && defined(F_SETLKW)) */
}
