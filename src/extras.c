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

/*
 *   Create the list of extra levels.
 */

#include "system.h"
#include "extras.h"
#include "generic_list.h"
#include "hedlite.h"
#include "misc.h"
#include "rsc_files.h"
#include "debugmsg.h"
#include "levellst.h"

typedef struct {
  a_filename	filename;
  char		is_in_user_dir;	/* Is this extra level a user level?
				 (user levels come from the ~/.heroes/level/
				 directory) */
} an_extradir_info;

static void free_extradir_info (an_extradir_info* ei);

NEW_LIST (an_extradir, an_extradir_info*, STD_EQUAL, free_extradir_info);

/* a_level_list is used for building a temporaly list of all extra
   levels seen in directories.  The list will then be converted to an
   array, by copying the pointed struct, therefore the destructor should
   free only the struct, not the elements pointer by the struct's members. */
NEW_LIST (a_level, an_extra_level*, STD_EQUAL, free);

an_extradir_list edir;

unsigned int extra_nbr = 0;	/* the total number of extra levels */
unsigned int extra_user_nbr = 0; /* The number of user levels from */

an_extra_level *extra_list = 0;	/* The list of extra-levels, the user's
				   extra-levels are at the beginning */
char *extra_selected_list = 0;	/* For each level: 1 if selected, 0 if not */


static void free_extradir_info (an_extradir_info* ei)
{
  free (ei->filename);
  free (ei);
}

/* compare two extra-levels for sorting,
   we want to sort user's levels first, and then alphabetically */
static int
cmp_extralevels (const an_extra_level* l, const an_extra_level* r)
{
  int d = r->is_in_user_dir - l->is_in_user_dir;

  if (d != 0)
    return d;
  return strcasecmp (l->level_name, r->level_name);
}

/* Browse a directory, adds the levels found to ll.
   Update extra_nbr and extra_user_nbr. */
static void
browse_extra_directory (an_extradir_info* edi, a_level_list* ll)
{
  DIR* dir;
  struct dirent* de;
  int n = 0;

  dmsg (D_FILE, "browsing directory %s ...", edi->filename);

  dir = opendir (edi->filename);
  if (!dir) {
    /* Always output an error message when handling a user directory.
       If not, we are probably reading a default extra directory,
       maybe system wide configured or hard coded in the source;
       it's best to assume this is not an error (this allow the
       addition of extra levels as packages or such).  */
    if (edi->is_in_user_dir)
      perror (edi->filename);
    dperror ("scandir");
    return;
  }

  while ((de = readdir (dir)))
    if (select_file_lvl (de)) {
      /* add the file to the list */
      char* fn;
      NEW (an_extra_level, tmp);

      XMALLOC_ARRAY (fn, (strlen (edi->filename) + 1 +
			  strlen (de->d_name) + 1));
      tmp->level_name = xstrdup (de->d_name);
      sprintf (fn, "%s/%s", edi->filename, tmp->level_name);
      tmp->full_name = fn;
      tmp->is_in_user_dir = edi->is_in_user_dir;

      strupr (tmp->level_name);
      if ((fn = strchr (tmp->level_name, '.')))
	*fn = 0;

      a_level_push (ll, tmp);
      ++n;
    }

  closedir (dir);

  dmsg (D_FILE, "... %d files", n);

  extra_nbr +=n;
  if (edi->is_in_user_dir)
    extra_user_nbr += n;
}

void
browse_extra_directories (void)
{
  an_extradir_list ed = edir;
  a_level_list ll = 0;
  a_level_list ll_cur;
  unsigned int i;

  /* build the list of the files found in each directory */
  while (ed) {
    browse_extra_directory (ed->car, &ll);
    ed = ed->cdr;
  }

  /* return if no extra level was found */
  if (!extra_nbr) {
    XFREE0 (extra_list);
    XFREE0 (extra_selected_list);
    return;
  }

  /* convert the list to an array (BTW, this array is called extra_list :)) */
  XREALLOC_ARRAY (extra_list, extra_nbr);
  for (i = 0, ll_cur = ll; ll_cur; ll_cur = ll_cur->cdr, ++i)
    extra_list[i] = *(ll_cur->car);
  assert (i == extra_nbr);

  /* ll is now useless */
  a_level_clear (&ll);

  /* sort the files list */
  qsort (extra_list, extra_nbr, sizeof(*extra_list),
	 (int (*)(const void*,const void*))cmp_extralevels);

  /* allocate the selection array */
  XREALLOC_ARRAY (extra_selected_list, extra_nbr);
  memset (extra_selected_list, 0, extra_nbr);
}

void
add_extra_directory (a_filename fn)
{
  NEW (an_extradir_info, tmp);
  tmp->filename = xstrdup (fn);
  tmp->is_in_user_dir = 0;
  an_extradir_push (&edir, tmp);
}

static void
add_extra_in_user_directory (a_filename fn)
{
  NEW (an_extradir_info, tmp);
  tmp->filename = xstrdup (fn);
  tmp->is_in_user_dir = 1;
  an_extradir_push (&edir, tmp);
}

void
add_default_extra_directories (void)
{
  char* t;
  dmsg (D_SECTION, "setup default extra directory");
  t = get_rsc_file ("extra-levels-dir");
  if (t) {
    add_extra_directory (t);
    free (t);
  }
  if (!create_levels_output_dir ())
    add_extra_in_user_directory (levels_output_dir);
}

void
free_extra_list (void)
{
  unsigned int i;

  if (extra_nbr == 0)
    return;

  dmsg (D_MISC, "freeing extra list");

  for (i = 0; i < extra_nbr; ++i) {
    free (extra_list[i].full_name);
    free (extra_list[i].level_name);
  }
  extra_nbr = 0;
  extra_user_nbr = 0;
  XFREE0 (extra_list);
  XFREE0 (extra_selected_list);
}

void
free_extra_directories (void)
{
  dmsg (D_MISC, "free extra directories");
  an_extradir_clear (&edir);

  free_levels_output_dir ();
}
