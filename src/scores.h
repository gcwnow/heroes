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

#ifndef HEROES__SCORES__H
#define HEROES__SCORES__H

#include "gameid.h"

#define PLAYER_NAME_SIZE 8
typedef struct
{
  char name[PLAYER_NAME_SIZE + 1];
  a_gameid gid;
  a_u32 points;
} top_score;

extern top_score highs[5][10];

void init_scores (void);
void reinit_scores_if_needed (void);
void sort_scores (void);
void clear_scores (void);
void write_scores (void);
void write_scores_locked (void);
bool insert_scores (int gamemode, const char *player_name,
		    a_gameid gid, a_u32 points);
void load_scores (void);
void load_scores_and_keep_locked (void);
void free_scores (void);
int find_score_by_gameid (gameid_ptr gid);

#endif /* HEROES__SCORES__H */
