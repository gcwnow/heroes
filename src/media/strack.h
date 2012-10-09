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

#ifndef HEROES__SOUNDTRACK__H
#define HEROES__SOUNDTRACK__H

#include "sprite.h"

extern char *soundtrack_author;
extern char *soundtrack_title;
extern a_sprite *soundtrack_author_sprite;
extern a_sprite *soundtrack_title_sprite;

void adjust_volume (void);
void halve_volume (void);
int init_sound_engine (void);
void uninit_sound_engine (void);
void unload_soundtrack (void);
void play_soundtrack (void);
void print_drivers_list (void);
void decode_sound_options (char *option_string, const char *argv0);
void load_soundtrack (char *filename);

void load_soundtrack_from_alias (const char *alias);
void load_next_soundtrack (void);
void load_prev_soundtrack (void);

#endif /* HEROES__SOUNDTRACK__H */
