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
#include "debugmsg.h"
#include "errors.h"
#include "rsc_files.h"
#include "dirname.h"

char* userdir = 0;

/* Test if a file exists and is a directory */
int
exists_dir (char* dir)
{
  struct stat s;
  int err = stat (dir, &s);
  if (err) {
    if (errno == ENOENT)
      return 0;
    dperror ("stat");
    return -1;
  }
  if (!S_ISDIR(s.st_mode)) {
    wmsg ("`%s' is not a directory.\n", dir);
    return -1;
  }
  return 1;
}


/* create the $(user-dir) directory, if needed */
int
setup_userdir (void)
{
  dmsg (D_SECTION,"user directory setup");

  if (userdir)
    free (userdir);

  userdir = get_non_null_rsc_file ("user-dir");
  strip_trailing_slashes (userdir);

  {
    int err = exists_dir (userdir);

    if (err < 0)
      return 1;
    if (err == 0) {
      if (mkdir (userdir, 0700)) {
	dperror ("mkdir");
	wmsg ("Cannot create directory `%s'.", userdir);
	return 1;
      } else {
	wmsg ("Directory `%s' created.", userdir);
      }
    } else {
      dmsg (D_SYSTEM, "directory `%s' already exists", userdir);
    }
  }
  return 0;
}

void
free_userdir (void)
{
  dmsg (D_SYSTEM, "free userdir");
  free (userdir);
  userdir = 0;
}
