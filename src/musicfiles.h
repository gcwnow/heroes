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

#ifndef HEROES__MUSICFILES__H
#define HEROES__MUSICFILES__H

typedef struct sound_track_s {
  char* alias;		/* name used in level files */
  char* filename;
  char* title;
  char* author;
  unsigned rank;
} a_sound_track;

a_sound_track* get_sound_track_from_alias (const char* alias);
a_sound_track* get_sound_track_from_rank (unsigned rank);

void add_sound_track_cons (char* alias, char* filename,
			   char* title, char* author);
int read_sound_config_file (char* filename);
void freeze_sound_track_list (void);
void init_sound_track_list (void);
void uninit_sound_track_list (void);
void print_sound_track_list (void);
void print_sound_track_list_stat (void);

#endif /* HEROES__MUSICFILES__H */
