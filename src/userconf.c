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
#include "userdir.h"
#include "misc.h"
#include "getshline.h"
#include "userconf.h"
#include "argv.h"
#include "extras.h"
#include "musicfiles.h"
#include "rsc_files.h"
#include "debugmsg.h"
#include "errors.h"
#include "vars.h"
#include "persona.h"

int
read_userconf (const char* file, bool secure)
{
  FILE* fs;
  int firstline = 0, endline = 0;
  char* buf = 0;
  size_t bufsize = 0;
#define MAX_ARGC 10

  dmsg (D_SECTION|D_FILE, "reading configuration file: %s ...", file);

  fs = fopen (file, "r");

  if (!fs) {
    dmsg (D_SECTION|D_FILE, "... could not open.");
    dperror ("fopen");
    return 0;
  }

  while (getshline_numbered
	 (&firstline, &endline, &buf, &bufsize, fs) != -1) {
    int argc;
    char* argv[MAX_ARGC];

    argv[0] = strtok (buf, " \t\n");
    if (!strcasecmp (argv[0], "Options:")) {
      /* fetch the options */
      argc = 1;
      while (argc < MAX_ARGC) {
	argv [argc] = strtok (0, " \t\n");
	if (argv[argc] == 0)
	  break;
	else
	  ++argc;
      }
      /* process the options */
      {
	int err;
	argv[0] = 0;
	err = parse_argv (argc, argv, file, firstline);
	if (err)
	  return err;
      }
    } else if (!strcasecmp (argv[0], "setenv:")) {
      char *s;
      /* get the variable name */
      argv [1] = strtok (0, " \t\n");
      if (argv[1] == 0) {
	wmsg (_("%s:%d: missing variable name"), file, firstline);
	goto non_fatal_error;
      }
      argv[2] = strtok (0, "\n");
      dmsg (D_SYSTEM, "setenv(%s,%s)", argv[1], argv[2]);
      XMALLOC_ARRAY (s, strlen (argv[1]) + strlen (argv[2]) + 2);
      sprintf (s, "%s=%s", argv[1], argv[2]);
      putenv (s);
      /* Can't free s, because most implementation of putenv
	 will not copy the string but add the pointer to the environment
	 directly.  So we create a memory leak, but in this case
	 it doesn't really matters.
	 FIXME: Use setenv when possible.  Provide a setenv implementation
	 (using putenv) for architectures lacking putenv.  */
      /* free (s); */
    } else if (!strcasecmp (argv[0], "extradir:")) {
      argv[1] = strtok (0, "\n");
      add_extra_directory (argv[1]);
    } else if (!strcasecmp (argv[0], "soundconf:")) {
      argv[1] = strtok (0, "\n");
      read_sound_config_file (argv[1]);
    } else if (!strcasecmp (argv[0], "setrsc:")){
      /* get the resource name */
      argv[1] = strtok (0, " \t\n");
      if (argv[1] == 0) {
	wmsg (_("%s:%d: missing resource name"), file, firstline);
	goto non_fatal_error;
      }
      argv[2] = strtok (0, "\n");
      set_rsc_file (argv[1], argv[2], secure);
    } else if (!strcasecmp (argv[0], "ifdef")){
      argv[1] = strtok (0, " \t\n");
      if (argv[1] == 0)
	wmsg (_("%s:%d: missing variable name"), file, firstline);
      if (argv[1] == 0 || var_get_value (argv[1]) == 0)
	/* Skip all lines until "endif", this is very basic: it doesn't
	   handled nested ifdef/endif.  I don't really care, some
	   day this config file should be parsed by guile or librep
	   and thus be much powerful. */
	while (getshline_numbered (&firstline, &endline,
				   &buf, &bufsize, fs) != -1) {
	  argv[0] = strtok (buf, " \t\n");
	  if (!strcasecmp (argv[0], "endif"))
	    break;
	}
    } else if (!strcasecmp (argv[0], "endif")) {
      /* Ignore.  */
    } else if (!strcasecmp (argv[0], "keepsgid:")) {
      if (!secure)
	emsg (_("%s:%d: "
		"%s: can only be used from the system configuration file"),
	      file, firstline, "keepsgid");
      argv[1] = strtok (0, " \t\n");
      if (!strcasecmp (argv[1], "yes")) {
	keep_sgid = true;
      } else if (!strcasecmp (argv[1], "no")) {
	keep_sgid = false;
      } else {
	emsg (_("%s:%d: %s: unknown value: %s"),
	      file, firstline, "keepsgid", argv[1]);
      }
    } else if (!strcasecmp (argv[0], "keepsuid:")) {
      if (!secure)
	emsg (_("%s:%d: "
		"%s: can only be used from the system configuration file"),
	      file, firstline, "keepsuid");
      argv[1] = strtok (0, " \t\n");
      if (!strcasecmp (argv[1], "yes")) {
	keep_suid = true;
      } else if (!strcasecmp (argv[1], "no")) {
	keep_suid = false;
      } else {
	emsg (_("%s:%d: %s: unknown value: %s"),
	      file, firstline, "keepsuid", argv[1]);
      }
    } else {
      wmsg (_("%s:%d: unknown keyword `%s'"), file, firstline, argv[0]);
      return 1;
    }
  non_fatal_error:
    ;
  }
  free (buf);
  fclose (fs);
  dmsg (D_SECTION|D_FILE, "... finished reading %s.", file);
  return 0;
}
