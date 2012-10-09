/*------------------------------------------------------------------.
| Copyright 1997, 1998, 2000, 2001  Alexandre Duret-Lutz            |
|                                    <duret_g@epita.fr>             |
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
#include "structs.h"
#include "scores.h"
#include "misc.h"
#include "userdir.h"
#include "debugmsg.h"
#include "rsc_files.h"
#include "getshline.h"
#include "errors.h"
#include "persona.h"
#include "filelock.h"

top_score highs[5][10];

static char *name = 0;
static FILE *fscores = 0;

void
init_scores (void)
{
  name = sys_persona_if_needed ("score-file", "a+t");
  dmsg (D_SYSTEM | D_FILE, "opening %s", name);
  fscores = fopen (name, "a+t");
  if (!fscores)
    dperror ("fopen");
  user_persona ();
}

void
reinit_scores_if_needed (void)
{
  char *newname = get_non_null_rsc_file ("score-file");
  if (strcmp (newname, name)) {
    free_scores ();
    init_scores ();
  }
  free (newname);
}

static int
cmp_scores (const void *r1, const void *r2)
{
  return (((const top_score *) r2)->points - ((const top_score *) r1)->points);
}

void
sort_scores (void)
{
  int i;
  for (i = 0; i < 5; i++)
    qsort (highs[i], 10, sizeof (top_score), cmp_scores);
}

int
find_score_by_gameid (gameid_ptr gid)
{
  int i;
  for (i = 0; i < 10; i++)
    if (equal_gameid (highs[0][i].gid, gid))
      return (i);
  return (-1);
}

void
clear_scores (void)
{
  int i, j;
  for (i = 0; i < 5; i++)
    for (j = 0; j < 10; j++) {
      strncpy (highs[i][j].name, "\0", 8);
      empty_gameid (highs[i][j].gid);
      highs[i][j].points = (10 - j) * 250;
    }
}

void
write_scores_locked (void)
{
  unsigned int i, j;

  if (fscores == 0) {
    wmsg (_("cannot write %s"), name);
  } else {
    fflush (fscores);
    if (ftruncate (fileno (fscores), 0) != 0)
      emsg (_("%s: truncate error"), name);
  }

  if (fscores) {
    dmsg (D_FILE, "writing scores to %s", name);

    /* Write down scores to disk.  */
    for (i = 0; i < 5; ++i)
      for (j = 0; j < 10; ++j) {
	char *gidtxt = gameid_to_text (highs[i][j].gid);
	fprintf (fscores, "%u %u %u\n %s\n  %s\n",
		 i, j, highs[i][j].points, gidtxt, highs[i][j].name);
	free (gidtxt);
      }
    /* We need to seek to the beginning of file before
       calling file_unlock on some systems.  */
    fseek (fscores, 0L, SEEK_SET);
    dmsg (D_FILE | D_SYSTEM, "unlocking %s", name);
    file_unlock (fileno (fscores));
  }
}

static void
load_scores_seek (bool exclusive)
{
  dmsg (D_FILE, "reading scores from %s", name);
  if (fscores) {
    fseek (fscores, 0L, SEEK_SET);
    dmsg (D_FILE | D_SYSTEM, "locking %s", name);
    file_lock (fileno (fscores), exclusive);
  }
}

void
write_scores (void)
{
  load_scores_seek (true);
  write_scores_locked ();
}

static void
load_scores_error (int line)
{
  wmsg (_("%s:%d: parse error.  Clearing score table."), name, line);
  clear_scores ();
}

static void
load_scores_read (void)
{
  unsigned int i, j;
  a_u32 points;
  int endline = 0;
  char* buf = 0;
  size_t bufsize = 0;
  int firstline = 0;

  clear_scores ();

  if (fscores == 0)
    return;

  /* Read the scores from disk.  */
  while (getshline_numbered
         (&firstline, &endline, &buf, &bufsize, fscores) != -1) {
    if (*buf != ' ') {
      if (sscanf (buf, "%u %u %u", &i, &j, &points) != 3
	  || i >= 5
	  || j >= 10) {
	load_scores_error (firstline);
	return;
      }
      highs[i][j].points = points;
    } else if (buf[1] != ' ') {
      if (text_to_gameid (buf + 1, highs[i][j].gid)) {
	load_scores_error (firstline);
	return;
      }
    } else {
      strncpy (highs[i][j].name, buf + 2, PLAYER_NAME_SIZE + 1);
      chomp (highs[i][j].name);
    }
  }
  free (buf);
}

void
load_scores (void)
{
  load_scores_seek (false);
  load_scores_read ();
  if (fscores) {
    /* We need to seek to the beginning of file before
       calling file_unlock on some systems.  */
    fseek (fscores, 0L, SEEK_SET);
    dmsg (D_FILE | D_SYSTEM, "unlocking %s", name);
    file_unlock (fileno (fscores));
  }
}

void
load_scores_and_keep_locked (void)
{
  load_scores_seek (true);
  load_scores_read ();
  if (fscores)
    fseek (fscores, 0L, SEEK_SET);
}

void
free_scores (void)
{
  dmsg (D_MISC, "free scores");

  if (name)
    free (name);
  name = 0;

  if (fscores) {
    fclose (fscores);
    fscores = 0;
  }
}

/* Insert an score entry in the score table.
   If NAME is NULL, we just return true or false, whether
   the score would have been inserted or not (if it's too low).  */
bool
insert_scores (int gamemode, const char *player_name,
	       a_gameid gid, a_u32 points)
{
  int mag = find_score_by_gameid (gid);
  /* If we are not goiing to override the score for an existing game,
     override the last entry of the game...  */
  if (mag == -1)
    mag = 9;
  /* ... unless the score is too low, of course.  */
  if (highs[gamemode][mag].points >= points)
    mag = -1;

  if (mag == -1)
    return false;

  if (player_name) {
    copy_gameid (highs[gamemode][mag].gid, gid);
    highs[gamemode][mag].points = points;
    strcpy (highs[gamemode][mag].name, player_name);
    sort_scores ();
  }
  return true;
}
