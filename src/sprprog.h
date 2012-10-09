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

#ifndef HEROES__SPRPROG__H
#define HEROES__SPRPROG__H

#include "sprite.h"

/*-----------------------------------------------------------------.
| sprite-programs are singly linked list of sprites, which are all |
| drawn at once.                                                   |
`-----------------------------------------------------------------*/

/* it is safe to call the following on S_PROG_WAV too */
void draw_sprprog (const a_sprite *sprite, a_pixel *dest);
void free_sprprog (a_sprite *sprite);


/* secondary drawing functions, used in scores_menu:
   draw only the sprites whose offset are greater than min, or
   lower than max */
void draw_sprprog_clipped_left (const a_sprite *sprite, a_pixel *dest,
				int dest_row, int min_row);
void draw_sprprog_clipped_right (const a_sprite *sprite, a_pixel *dest,
				 int dest_row, int max_row);

/* The following is not thread safe, because when you build a sprprog
   the state is kept in static variables.  But this make the interface
   friendlier.  But it can be called recusrively, i.e. you can
   build a new prog (new,add...,end) while you are already building
   another prog.  new and end will actually save and restore the
   context.

   It is safe to do
     new_sprprog ();
     add_sprprog0 (compile_menu_text ("bar", T_CENTERED|T_WAVING, 5, 159));
     add_sprprog0 (compile_menu_text ("foo", T_FLUSHED_LEFT, 39, 56));
     end_sprprog ();
   even if compile_menu_text is itself using {new,add,end}_sprprog.
*/

/* call this to start a new sprprog */
void new_sprprog (void);

/* add as many sprites as you wish */
void add_sprprog (a_sprite *sprite, int offset);
/* likewise, with offset = 0 */
void add_sprprog0 (a_sprite *sprite);

/* finish the sprprog, and return it */
a_sprite *end_sprprog (void);


/* The following is an internal definition,
   which is made public for sprprogwav.c usage. */

struct sprite_prog_list_s {
  a_sprite_prog_list *cdr;
  a_sprite *car;
  int offset;			/* offset to add to the destination supplied */
};

#endif /* HEROES__SPRPROG__H */
