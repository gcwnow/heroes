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

#ifndef HEROES__SPRPROGWAV__H
#define HEROES__SPRPROGWAV__H

#include "sprite.h"
#include "sprprog.h"
#include "const.h"

/*-------------------------------------------------------------------.
| S_PROG_WAV is the same as S_PROG, except that the drawing function |
| will wave the sprites up and down                                  |
`-------------------------------------------------------------------*/

/* it is safe to call the following on S_PROG too */
void draw_sprprogwav (const a_sprite *sprite, a_pixel *dest);

/* the following is called inheritance in some languages :o) */
#define free_sprprogwav free_sprprog
#define new_sprprogwav new_sprprog
#define add_sprprogwav add_sprprog

a_sprite *end_sprprogwav (void);

#define draw_sprprogwav_if(cond, spr, dest) \
  ((cond) ? draw_sprprogwav : draw_sprprog) ((spr), (dest))

/* update_text_waving should be called periodically */
void init_text_waving_step (void);
void uninit_text_waving_step (void);
void update_text_waving_step (void);

extern unsigned char text_waving_step;

#endif /* HEROES__SPRPROGWAV__H */
