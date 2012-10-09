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

#ifndef HEROES__ITEMS__H
#define HEROES__ITEMS__H

/*----------------------------------------------------------------.
| `Items' refers to the various items collectable during the game |
| (color pyramids, dollars, etc.).                                |
`----------------------------------------------------------------*/

#include "sprite.h"

extern a_sprite *big_dollar;
extern a_sprite *small_dollar;
#define NBR_CLOCK_FRAMES 8
extern a_sprite *clocks[NBR_CLOCK_FRAMES];
#define NBR_PYRAMIDS 5
extern a_sprite *pyramids[NBR_PYRAMIDS];
extern a_sprite *trails[16][12];
extern a_sprite *red_cross[4];
#define NBR_ARROW_FRAMES 12
extern a_sprite *tutorial_arrow[NBR_ARROW_FRAMES];

void init_items (void);
void uninit_items (void);

#endif /* HEROES__ITEMS__H */
