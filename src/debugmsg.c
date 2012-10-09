/*------------------------------------------------------------------------.
| Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
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
#include "debugmsg.h"
#include "debughash.h"
#include "errors.h"
#include "misc.h"
#include "basename.h"

enum debug_lvl debug_level = 0;
const char* program_name = 0;

#ifndef dmsg

#if defined VA_START
void
dmsg (enum debug_lvl dlvl, const char* msg, ...)
#else
void
dmsg (dlvl, msg, va_alist)
     enum debug_lvl dlvl;
     const char* msg;
     va_dcl;
#endif
{
  if (dlvl & debug_level) {
#ifdef VA_START
    va_list args;
#endif
    fprintf (stdout, "%s: ", program_name);
#ifdef VA_START
    VA_START (args, msg);
# if HAVE_VPRINTF
    vfprintf (stdout, msg, args);
# else
    _doprnt (msg, args, stdout);
# endif /* HAVE_VPRINTF */
    va_end (args);
#else
    fprintf (stdout, msg, va_alist);
#endif /* VA_START */
    putc ('\n', stdout);
    fflush (stdout);
  }
}

void
dperror (const char* s)
{
  if (debug_level) {
    fprintf (stderr, "%s: ", program_name);
    fflush (stderr);
    perror (s);
  }
}

#endif /* !dmsg */

void
dmsg_parse_string (const char* option)
{
  char* buf;

  if (option) {
#ifndef USE_HEROES_DEBUG
    wmsg ("\
Ignoring debug channel specifications: debug messages are not\n\
compiled in.  Install Heroes with 'configure --enable-heroes-debug' if\n\
you want that feature.");
#else
    if (option[0] == '-' || (option[0] >= '0' && option[0] <= '9')) {
      /* the option is a number */
      debug_level = atol (option);
    } else {
      /* the option is a string */
      buf = xstrdup (option);
      strlwr (buf);
      option = strtok (buf, " \t:,|&");
      while (option) {
	int neg = 0;
	int val;

	if (option[0] == '-' || option[0] == '!') {
	  neg = 1;
	  ++option;
	}

	/* lookup the channel number associated to the given name */
	val = get_channel_number (option);

	if (neg)
	  debug_level &= ~val;
	else
	  debug_level |= val;

	option = strtok (0, " \t:,|&");
      }
      free (buf);
    }
    dmsg (D_MISC, "set debug level to %x", debug_level);
#endif /* HEROES_DEBUG */
  }
}

void
dmsg_init (const char* prgname)
{
  program_name = base_name (prgname);

  dmsg_parse_string (getenv ("HEROES_DEBUG"));
}
