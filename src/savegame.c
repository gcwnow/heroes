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
#include "savegame.h"
#include "argv.h"
#include "misc.h"
#include "userdir.h"
#include "debugmsg.h"
#include "rsc_files.h"
#include "getshline.h"
#include "errors.h"
#include "persona.h"
#include "filelock.h"

saved_game saverec[10];

static char *name = 0;
static FILE *fsave = 0;

void
init_save_records (void)
{
  name = sys_persona_if_needed ("saved-games-file", "a+t");
  dmsg (D_FILE | D_SYSTEM, "opening %s", name);
  fsave = fopen (name, "a+t");
  if (!fsave)
    dperror ("fopen");
  user_persona ();
}

void
reinit_save_records_if_needed (void)
{
  char *newname = get_non_null_rsc_file ("saved-games-file");
  if (strcmp (newname, name)) {
    free_save_records ();
    init_save_records ();
  }
  free (newname);
}


void
clear_save_records (void)
{
  memset (saverec, 0, 10 * sizeof (saved_game));

  if (x10sav) {
    unsigned int i;
    for (i = 0; i < 10; i++) {
      sprintf (saverec[i].name, "LVL%u", (1 + i) * 10);
      saverec[i].level = (1 + i) * 10 - 1;
      saverec[i].points[0] = saverec[i].points[1] = saverec[i].points[2] =
	saverec[i].points[3] = (1 + i) * 1000;
      saverec[i].lifes[0] = saverec[i].lifes[1] = saverec[i].lifes[2] =
	saverec[i].lifes[3] = 9;
      empty_gameid (saverec[i].gid);
      saverec[i].used = 1;
    }
  }
}

void
write_save_records_locked (void)
{
  int i;

  if (fsave == 0) {
    wmsg (_("cannot write %s"), name);
    dperror (name);
  } else {
    fflush (fsave);
    if (ftruncate (fileno (fsave), 0) != 0)
      emsg (_("%s: truncate error"), name);
  }

  if (fsave) {
    dmsg (D_FILE, "saving games to %s", name);

    for (i = 0; i < 10; ++i) {
      saved_game *sg = saverec + i;
      char *gidtxt = gameid_to_text (sg->gid);
      fprintf (fsave, "%u %u %u %u %u %u %u %u %u %u\n %s\n  %s\n",
	       sg->level,
	       sg->points[0], sg->lifes[0],
	       sg->points[1], sg->lifes[1],
	       sg->points[2], sg->lifes[2],
	       sg->points[3], sg->lifes[3],
	       sg->used,
	       gidtxt,
	       sg->name);
      free (gidtxt);
    }
    /* We need to seek to the beginning of file before
       calling file_unlock on some systems.  */
    fseek (fsave, 0L, SEEK_SET);
    dmsg (D_FILE | D_SYSTEM, "unlocking %s", name);
    file_unlock (fileno (fsave));
  }
}

static void
load_save_records_error (int line)
{
  wmsg (_("%s:%d: parse error.  Clearing saved-games table."),
	name, line);
  clear_scores ();
}

static void
load_save_records_read (void)
{
  int i;
  int endline = 0;
  char* buf = 0;
  size_t bufsize = 0;
  int firstline = 0;

  clear_save_records ();
  if (fsave == 0)
    return;

  /* Read the scores from disk.  */
  i = 0;
  while (getshline_numbered
         (&firstline, &endline, &buf, &bufsize, fsave) != -1) {
    saved_game *sg = saverec + i;
    if (*buf != ' ') {
      unsigned int u[10];
      if (sscanf (buf, "%u %u %u %u %u %u %u %u %u %u",
		  u, u + 1, u + 2, u + 3, u + 4, u + 5, u + 6,
		  u + 7, u + 8, u + 9) != 10
	  || u[9] > 1) {
	load_save_records_error (firstline);
	return;
      }
      sg->level = u[0];
      sg->points[0] = u[1];
      sg->lifes[0] = u[2];
      sg->points[1] = u[3];
      sg->lifes[1] = u[4];
      sg->points[2] = u[5];
      sg->lifes[2] = u[6];
      sg->points[3] = u[7];
      sg->lifes[3] = u[8];
      sg->used = u[9];
    } else if (buf[1] != ' ') {
      if (text_to_gameid (buf + 1, sg->gid)) {
	load_save_records_error (firstline);
	return;
      }
    } else {
      strncpy (sg->name, buf + 2, 16);
      chomp (sg->name);
      ++i;
    }
    /* Exit if the savedgame array is full.  */
    if (i >= 10)
      break;
  }
  free (buf);
}

static void
load_save_records_seek (bool exclusive)
{
  dmsg (D_FILE, "reading saved games from %s", name);
  if (fsave) {
    fseek (fsave, 0L, SEEK_SET);
    dmsg (D_FILE | D_SYSTEM, "locking %s", name);
    file_lock (fileno (fsave), exclusive);
  }
}

void
load_save_records (void)
{
  load_save_records_seek (false);
  load_save_records_read ();
  if (fsave) {
    /* We need to seek to the beginning of file before
       calling file_unlock on some systems.  */
    fseek (fsave, 0L, SEEK_SET);
    dmsg (D_FILE | D_SYSTEM, "unlocking %s", name);
    file_unlock (fileno (fsave));
  }
}

void
load_save_records_and_keep_locked (void)
{
  load_save_records_seek (true);
  load_save_records_read ();
  if (fsave)
    fseek (fsave, 0L, SEEK_SET);
}

void
write_save_records (void)
{
  load_save_records_seek (true);
  write_save_records_locked ();
}

void
free_save_records (void)
{
  dmsg (D_MISC, "free save records");

  if (name)
    free (name);
  name = 0;

  if (fsave) {
    fclose (fsave);
    fsave = 0;
  }
}

void
write_save_one_record (int rec)
{
  saved_game save = saverec[rec];
  load_save_records_and_keep_locked ();
  saverec[rec] = save;
  write_save_records_locked ();
}
