/*------------------------------------------------------------------------.
| Copyright 1997, 1998, 2000  Alexandre Duret-Lutz <duret_g@epita.fr>     |
|                                                                         |
| This file is part of Heroes.                                            |
|                                                                         |
| Heroes is free software; you can redistribute it and/or modify it under |
| the terms of the GNU General Public License version 2 as published by   |
| the Free Software Foundation.                                           |
|                                                                         |
| Heroes is distributed in the hope that it will be useful, but WITHOUT   |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   |
| for more details.                                                       |
|                                                                         |
| You should have received a copy of the GNU General Public License along |
| with this program; if not, write to the Free Software Foundation, Inc., |
| 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA                   |
`------------------------------------------------------------------------*/

#include "system.h"
#include "errors.h"
#include "video.h"
#include "strack.h"

extern const char* program_name;	/* defined in debugmsg.c */

int disable_wmsg = 0;

#if defined VA_START
void
wmsg (const char* msg, ...)
#else
void
wmsg (msg, va_alist)
     const char* msg;
     va_dcl;
#endif
{
#ifdef VA_START
  va_list args;
#endif

  if (disable_wmsg)
    return;
  fprintf (stderr, "%s: ", program_name);
#ifdef VA_START
  VA_START (args, msg);
# if HAVE_VPRINTF
  vfprintf (stderr, msg, args);
# else
  _doprnt (msg, args, stderr);
# endif /* HAVE_VPRINTF */
  va_end (args);
#else
  fprintf (stderr, msg, va_alist);
#endif /* VA_START */
  putc ('\n', stderr);
  fflush (stderr);
}


int disable_emsg = 0;

#if defined VA_START
void
emsg (const char* msg, ...)
#else
void
emsg (msg, va_alist)
     const char* msg;
     va_dcl;
#endif
{
#ifdef VA_START
  va_list args;
#endif

  if (!disable_emsg) {
    fprintf (stderr, "%s: ", program_name);
#ifdef VA_START
    VA_START (args, msg);
# if HAVE_VPRINTF
    vfprintf (stderr, msg, args);
# else
    _doprnt (msg, args, stderr);
# endif /* HAVE_VPRINTF */
    va_end (args);
#else
    fprintf (stderr, msg, va_alist);
#endif /* VA_START */
    putc ('\n', stderr);
    fflush (stderr);
  }
  exit_heroes (33);
}

void
exit_heroes (int code)
{
  uninit_sound_engine ();
  uninit_video ();
  exit (code);
}
