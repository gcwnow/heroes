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
#include "strack.h"
#include "prefs.h"
#include "argv.h"
#include "musicfiles.h"
#include "debugmsg.h"
#include "errors.h"

char *soundtrack_author = 0;
char *soundtrack_title = 0;
a_sprite *soundtrack_author_sprite = 0;
a_sprite *soundtrack_title_sprite = 0;

unsigned last_rank = 0;

static void
load_and_setup_sound_track (a_sound_track* st)
{
  if (st) {
    load_soundtrack (st->filename);
    soundtrack_title = st->title;
    soundtrack_author = st->author;
    last_rank = st->rank;
  } else {
    soundtrack_title = 0;
    soundtrack_author = 0;
  }
  FREE_SPRITE0 (soundtrack_title_sprite);
  FREE_SPRITE0 (soundtrack_author_sprite);
}

void
load_soundtrack_from_alias (const char* alias)
{
  if (!nosound) {
    a_sound_track *st = get_sound_track_from_alias (alias);
    dmsg (D_SOUND_TRACK, "loading sound track from alias %s", alias);
    load_and_setup_sound_track (st);
  }
}

void
load_next_soundtrack (void)
{
  if (!nosound) {
    a_sound_track *st = get_sound_track_from_rank (last_rank + 1);
    dmsg (D_SOUND_TRACK, "loading next sound track");
    load_and_setup_sound_track (st);
  }
}

void
load_prev_soundtrack (void)
{
  if (!nosound) {
    a_sound_track *st = get_sound_track_from_rank (last_rank - 1);
    dmsg (D_SOUND_TRACK, "loading previous sound track");
    load_and_setup_sound_track (st);
  }
}
