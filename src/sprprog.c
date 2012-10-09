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

#include "system.h"
#include "sprprog.h"
#include "generic_list.h"

void
draw_sprprog (const a_sprite *sprite, a_pixel *dest)
{
  a_sprite_prog_list *list;

  assert (sprite->all.kind == S_PROG || sprite->all.kind == S_PROG_WAV);

  for (list = sprite->prog.list; list; list = list->cdr) {
    a_sprite *s = list->car;
    s->draw (s, dest + list->offset);
  }
}

void
draw_sprprog_clipped_left (const a_sprite *sprite, a_pixel *dest,
			   int dest_col, int min_col)
{
  a_sprite_prog_list *list;

  assert (sprite->all.kind == S_PROG || sprite->all.kind == S_PROG_WAV);

  for (list = sprite->prog.list; list; list = list->cdr) {
    a_sprite *s = list->car;
    if (list->offset + dest_col > min_col)
      s->draw (s, dest + list->offset);
  }
}

void
draw_sprprog_clipped_right (const a_sprite *sprite, a_pixel *dest,
			    int dest_col, int max_col)
{
  a_sprite_prog_list *list;

  assert (sprite->all.kind == S_PROG || sprite->all.kind == S_PROG_WAV);

  for (list = sprite->prog.list; list; list = list->cdr) {
    a_sprite *s = list->car;
    if (list->offset + dest_col < max_col)
      s->draw (s, dest + list->offset);
  }
}


/* internal state */
static a_sprite_prog_list* prog = 0;
static a_sprite_prog_list** last = 0;

/* we need to save these two state, to allow recursive calls */
NEW_LIST (a_prog, a_sprite_prog_list*, STD_EQUAL, free);
NEW_LIST (a_last, a_sprite_prog_list**, STD_EQUAL, free);
a_prog_list prog_save = 0;
a_last_list last_save = 0;

void
new_sprprog (void)
{
  a_prog_push (&prog_save, prog);
  a_last_push (&last_save, last);
  last = &prog;
}


void
add_sprprog (a_sprite *sprite, int offset)
{
  XMALLOC_VAR (*last);
  (*last)->car = sprite;
  (*last)->offset = offset;
  last = &((*last)->cdr);
}

void
add_sprprog0 (a_sprite *sprite)
{
  add_sprprog (sprite, 0);
}

a_sprite *
end_sprprog (void)
{
  NEW (a_sprite, res);
  *last = 0;			/* terminate the list */
  res->prog.kind = S_PROG;
  res->prog.draw = draw_sprprog;
  res->prog.list = prog;
  prog = a_prog_pop (&prog_save);
  last = a_last_pop (&last_save);
  return res;
}

void
free_sprprog (a_sprite *sprite)
{
  a_sprite_prog_list *list = sprite->prog.list;
  while (list) {
    a_sprite_prog_list *next = list->cdr;
    free_sprite (list->car);
    free (list);
    list = next;
  }
  free (sprite);
}
