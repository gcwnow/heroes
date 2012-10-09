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

#ifndef HEROES__BONUS__H
#define HEROES__BONUS__H

#include "pcx.h"
#include "timer.h"

extern int bonus_points[2][17];	/* interest of bonuses,
				   for CPU controled vehicles */

extern a_timer bonus_anim_htimer;
extern int bonus_anim_offset;

extern unsigned char *tile_bonus;
extern unsigned char *tile_bonus_cpu;
extern int *bonus_time;
extern int *bonus_list;

extern int txt_bonus_tempo[4];

extern void add_bonus (int pos_in_list, unsigned char what);
extern void add_random_bonus (int pos_in_list);
extern void rem_bonus (int pos);

/* globaly initialize bonuses */
extern void init_bonuses (void);
extern void uninit_bonuses (void);

/* reset and allocate bonus data for a given level */
extern int init_bonuses_level (void);
extern void uninit_bonuses_level (void);

extern void spread_bonuses (void);

extern void set_txt_bonus (int pl, const char *txt, int tempo);
extern void show_txt_bonus (int pl, a_pixel *dest);

extern void add_end_level_bonuses (void);
extern void apply_bonus (int pl, char bonus);

extern void update_player_bonus_vars (int pl);
extern void update_bonuses (void);

#endif
