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

#ifndef HEROES__SAVEGAME__H
#define HEROES__SAVEGAME__H

#include "gameid.h"

typedef struct
{
  char name[16];
  a_u32 level;
  a_u32 points[4];
  a_u32 lifes[4];
  a_gameid gid;
  bool used;
} saved_game;

extern saved_game saverec[10];

void init_save_records (void);
void reinit_save_records_if_needed (void);
void clear_save_records (void);
void write_save_records (void);
void write_save_records_locked (void);
void load_save_records (void);
void load_save_records_and_keep_locked (void);
void free_save_records (void);
void write_save_one_record (int rec);

#endif /* HEROES__SAVEGAME__H */
