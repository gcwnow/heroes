/*------------------------------------------------------------------.
| Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
|                                                                   |
| This file is part of Heroes.                                      |
|                                                                   |
| Heroes is free software; you can redistribute it and/or modify it |
| under the terms of the GNU General Public License version 2 as    |
| published by the Free Software Foundation.                        |
|                                                                   |
| Heroes is distributed in the hope that it will be useful, but     |
| WITHOUT ANY WARRANTY; without even the implied warranty of        |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU |
| General Public License for more details.                          |
|                                                                   |
| You should have received a copy of the GNU General Public License |
| along with this program; if not, write to the Free Software       |
| Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA          |
| 02111-1307 USA                                                    |
`------------------------------------------------------------------*/

#include "system.h"
#include "levellst.h"
#include "lvl.h"
#include "debugmsg.h"
#include "errors.h"
#include "rsc_files.h"

/* select only *.lvl files */
int
select_file_lvl (const struct dirent *d)
{
  int l = strlen (d->d_name);
  return (l > 4 && d->d_name[l - 4] == '.' && d->d_name[l - 3] == 'l'
	  && d->d_name[l - 2] == 'v' && d->d_name[l - 1] == 'l');
}

a_level_info *level_list = 0;
size_t level_list_size = 0;
static size_t level_list_max = 0;

static void
read_level_dir (const char *dirname)
{
  DIR *dir;
  struct dirent* de;
  int n = 0;

  dmsg (D_FILE | D_SECTION, "reading level list from %s", dirname);

  dir = opendir (dirname);
  if (!dir) {
    dperror ("opendir");
    emsg (_("cannot open directory %s"), dirname);
  }

  while ((de = readdir (dir)))
    if (select_file_lvl (de)) {
      char *filename;
      a_level tmp_lvl;

      if (level_list_size >= level_list_max) {
	level_list_max += 32;
	XREALLOC_ARRAY (level_list, level_list_max);
      }

      filename = xmalloc (strlen (dirname) + 1 + strlen (de->d_name) + 1);
      sprintf (filename, "%s/%s", dirname, de->d_name);
      level_list[level_list_size].name = filename;

      dmsg (D_FILE, "loading header from %s", filename);
      lvl_load_file (filename, &tmp_lvl, false);
      level_list[level_list_size].wrapped =
	(tmp_lvl.tile_width_wrap != DONT_WRAP
	 && tmp_lvl.tile_height_wrap != DONT_WRAP);
      lvl_free (&tmp_lvl);

      ++level_list_size;
    }

  closedir (dir);
  dmsg (D_FILE, "... %d files", n);
}


/*  Level should be played in the following order:

    niv010.lvl
    niv020.lvl
    niv030.lvl
    ...
    niv100.lvl
    niv011.lvl
    niv021.lvl
    niv031.lvl
    ...
    niv101.lvl
    niv019.lvl
    niv029.lvl
    niv039.lvl
    ...
    niv109.lvl

    I.e, sorted on the last digit first.
*/

static int
cmp_levels (const void *a, const void *b)
{
  const a_level_info *lia = a;
  const a_level_info *lib = b;
  int la = strlen (lia->name);
  int lb = strlen (lib->name);
  if (la != lb || la < 6) {
    /* This should not happen, unless someone intentionally add some
       strange level files to the directory.  */
    return strcmp (lia->name, lib->name);
  } else {
    /* Sort on the last digit first.  */
    int ret = (int) lia->name[la - 5] - (int) lib->name[lb - 5];
    if (ret)
      return ret;
    else
      /* Then use the remaining of the name.  */
      return strncmp (lia->name, lib->name, la - 5);
  }
}

int
read_level_list (void)
{
  char *dirname = get_non_null_rsc_file ("levels-dir");
  read_level_dir (dirname);
  free (dirname);

  /* Sort the levels array (files are numbered and we really
     want to play them in the right order).  */
  qsort (level_list, level_list_size, sizeof (*level_list), cmp_levels);
  return 0;
}

void
free_level_list (void)
{
  size_t i;
  for (i = 0; i < level_list_size; ++i)
    free (level_list[i].name);
  free (level_list);
  level_list = 0;
  level_list_max = 0;
  level_list_size = 0;
}

void
print_level_list (void)
{
  unsigned int i;
  for (i = 0; i < level_list_size; ++i)
    printf ("%3d. %s%s\n", i + 1, level_list[i].name,
	    level_list[i].wrapped ? " (fully wrapped)" : "");
}
