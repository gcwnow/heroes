/*------------------------------------------------------------------------.
| Copyright 2000  Alexandre Duret-Lutz <duret_g@epita.fr>                 |
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
#include "getshline.h"
#include "musicfiles.h"
#include "misc.h"
#include "rsc_files.h"
#include "debugmsg.h"
#include "errors.h"
#include "hash.h"
#include "dirname.h"
#include "cast.h"

/* we maintain the sound tracks in two structures: a hash and
   an array.  The hash is used for most lookups, and when building
   the sound tracks database.  The array is initialized once
   the hash has been finished, it is sorted (w.r.t. the alias name)
   and is used by the sound tracks selector in the game (which
   need to be able to load the `next' or `previous' sound track). */

static Hash_table *st_hash;
static a_sound_track **st_array = 0;
static unsigned n_st = 0;	/* number of sound tracks */

/* comparison function for the hash */
static bool
st_equ (const void *left, const void *right)
{
  const a_sound_track *l = left;
  const a_sound_track *r = right;
  return !strcasecmp (l->alias, r->alias);
}

/* comparison function for qsort */
static int
st_cmp (const void *left, const void *right)
{
  const a_sound_track *const *l = left;
  const a_sound_track *const *r = right;
  return strcasecmp ((*l)->alias, (*r)->alias);
}

/* This is like hash_string, defined in lib/hash.c, but it maps lower
   and upercase strings to the same value.  */
static unsigned
hash_case_string (const char *string, unsigned n_buckets)
{
  unsigned value = 0;

  while (*string) {
    value = ((value * 31 + (int) (unsigned char) TOLOWER (*string))
	     % n_buckets);
    ++string;
  }
  return value;
}

static unsigned
st_hasher (const void *data, unsigned size)
{
  const a_sound_track *d = data;
  return hash_case_string (d->alias, size);
}

static void
st_uninit (a_sound_track *st)
{
  free (st->alias);
  free (st->filename);
  free (st->title);
  free (st->author);
}

static void
st_free (void *data)
{
  a_sound_track *st = data;
  st_uninit (st);
  free (st);
}

static void
st_init (a_sound_track *st,
	 const char *alias, const char *filename,
	 const char *title, const char *author)
{
  st->alias = xstrdup (alias);
  st->filename = xstrdup (filename);
  st->title = xstrdup (title);
  st->author = xstrdup (author);
  st->rank = 0;
}

static a_sound_track *
st_cons (char *alias, char *filename, char *title, char *author)
{
  NEW (a_sound_track, st);
  st_init (st, alias, filename, title, author);
  return st;
}

void
add_sound_track_cons (char *alias, char *filename, char *title, char *author)
{
  /* If the alias already exists, overide it.  Otherwise, create it.  */
  a_sound_track *st = get_sound_track_from_alias (alias);
  if (st) {
    st_uninit (st);
    st_init (st, alias, filename, title, author);
  } else if (!hash_insert (st_hash, st_cons (alias, filename, title, author)))
    xalloc_die ();
}

a_sound_track *
get_sound_track_from_alias (const char *alias)
{
  a_sound_track st;
  st.alias = const_cast_string (alias);
  return hash_lookup (st_hash, &st);
}

a_sound_track *
get_sound_track_from_rank (unsigned rank)
{
  return st_array[rank % n_st];
}

int
read_sound_config_file (char *filename)
{
  FILE *fs;
  char *buf = 0;
  size_t bufsize = 0;
  int firstline = 0, endline = 0;
  char *expfilename = rsc_expand (filename);
  char *dir = dir_name (expfilename);
  size_t dirlen = strlen (dir);

  dmsg (D_SECTION | D_FILE, "reading sound config file: %s ...", expfilename);

  fs = fopen (expfilename, "r");

  if (!fs) {
    dmsg (D_SECTION | D_FILE, "... could not open.");
    dperror ("fopen");
    free (expfilename);
    free (dir);
    return 0;
  }

  while (getshline_numbered (&firstline, &endline, &buf, &bufsize, fs) != -1) {
    char *alias = strtok (buf, ":\n");
    char *file = strtok (0, ":\n");
    char *title = strtok (0, ":\n");
    char *author = strtok (0, "\n");
    if (!alias || !alias[0])
      wmsg (_("%s:%d: missing alias name"), filename, firstline);
    else if (!file || !file[0])
      wmsg (_("%s:%d: missing file name"), filename, firstline);
    else if (!title || !title[0])
      wmsg (_("%s:%d: missing title"), filename, firstline);
    else if (!author || !author[0])
      wmsg (_("%s:%d: missing author"), filename, firstline);
    else {
      if (dir && file[0] != '/') {
	char *tmp = malloc (dirlen + 1 + strlen (file) + 1);
	sprintf (tmp, "%s/%s", dir, file);
	add_sound_track_cons (alias, tmp, title, author);
	free (tmp);
      } else {
	add_sound_track_cons (alias, file, title, author);
      }
    }
  }
  fclose (fs);
  free (buf);
  free (dir);
  free (expfilename);
  dmsg (D_SECTION|D_FILE,"... done.");

  return 0;
}

void
init_sound_track_list (void)
{
  dmsg (D_MISC, "initialize sound track hash");

  st_hash = hash_initialize (17, NULL, st_hasher, st_equ, st_free);
  if (!st_hash)
    xalloc_die ();
}

void
uninit_sound_track_list (void)
{
  dmsg (D_MISC, "free sound track hash");
  hash_free (st_hash);
  XFREE0 (st_array);
  n_st = 0;
}

void
print_sound_track_list (void)
{
  unsigned pos;
  for (pos = 0; pos < n_st; ++pos)
    printf ("%s:%s:%s:%s\n",
	    st_array[pos]->alias, st_array[pos]->filename,
	    st_array[pos]->title, st_array[pos]->author);
}

void
print_sound_track_list_stat (void)
{
  hash_print_statistics (st_hash, stdout);
}

void
freeze_sound_track_list (void)
{
  a_sound_track *s;
  a_sound_track **p;
  unsigned pos;

  n_st = hash_get_n_entries (st_hash);
  if (!n_st)
    return;

  /* create an array of all existing aliases */

  XMALLOC_ARRAY (st_array, n_st);
  pos = 0;
  for (s = hash_get_first (st_hash); s; s = hash_get_next (st_hash, s))
    st_array[pos++] = s;

  assert (pos == n_st);

  /* sort it */
  qsort (st_array, n_st, sizeof (*st_array), st_cmp);

  /* number each sound track */
  p = st_array;
  for (pos = 0; pos < n_st; ++pos)
    (*p++)->rank = pos;
}
