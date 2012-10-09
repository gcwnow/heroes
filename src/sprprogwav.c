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
#include "sprprogwav.h"
#include "timer.h"

unsigned char text_waving_step = 0;
static a_timer text_waving_htimer = 0;

void
init_text_waving_step (void)
{
  text_waving_htimer = new_htimer (T_LOCAL, HZ (70));
  text_waving_step = 0;
}

void
uninit_text_waving_step (void)
{
  free_htimer (text_waving_htimer);
}

void
update_text_waving_step (void)
{
  text_waving_step += read_htimer (text_waving_htimer);
}

static int
waving_offset (int pos)
{
  return ceil (sin (((text_waving_step + pos*2) & 31) * 3.141592653 / 16.0)
	       * 1.7) * xbuf;
}

void
draw_sprprogwav (const a_sprite *sprite, a_pixel *dest)
{
  a_sprite_prog_list *list;
  int pos = 0;

  assert (sprite->all.kind == S_PROG || sprite->all.kind == S_PROG_WAV);

  for (list = sprite->prog.list; list; list = list->cdr) {
    a_sprite *s = list->car;
    s->draw (s, dest + list->offset + waving_offset (pos++));
  }
}

a_sprite *
end_sprprogwav (void)
{
  a_sprite *s = end_sprprog ();
  s->all.kind = S_PROG_WAV;	/* overwrite */
  s->all.draw = draw_sprprogwav; /* overwrite */
  return s;
}
