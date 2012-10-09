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


#ifndef HEROES__HEROES__H
#define HEROES__HEROES__H

#include "timer.h"

extern a_timer clock_htimer;
extern a_timer blink_htimer;
extern a_timer bonus_anim_htimer;
extern a_timer tiles_anim_htimer;
extern a_timer corner_htimer;
extern a_timer waving_htimer;
extern a_timer background_htimer;
extern a_timer corner_htimer;
extern a_timer demo_trigger_htimer;
extern a_timer sound_track_htimer;
extern long event_time;
extern a_timer event_htimer;
extern a_timer update_htimer;

extern void compute_corner (int p, int n);
extern void play_demo (void);

extern int level_is_finished;
extern char enable_blit;
extern int rounds;

extern void grow_trail (int pl, int size);
extern void shrink_trail (int pl, int size);

#endif /* HEROES__HEROES__H */
