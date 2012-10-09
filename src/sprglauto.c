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
#include "sprglauto.h"
#include "sprrle.h"
#include "const.h"

void
draw_sprglauto (const a_sprite *sprite, a_pixel *dest)
{
  a_pixel	*cur = dest;	/* current writting possition */
  a_u8		*pc;		/* program counter */
  a_u8		*epc;		/* end of program code */

  assert (sprite->all.kind == S_RLE_GLENZ_AUTO);

  pc = sprite->rle.code;
  epc = sprite->rle.end_code;

  while (pc < epc) {
    unsigned m, n;
    m = *pc++;
    n = *pc++;
    if (n == 0 && m == 0) {	/* end of line */
      cur += sprite->rle.line_skip;
    } else {
      cur += m;
      for (; n; --n) {
	a_pixel c = *pc;
	if (c == 1)
	  *cur = glenz[0][*cur];
	else if (c == 111)
	  *cur = glenz[2][*cur];
	else if (c == 127)
	  *cur = glenz[3][*cur];
	else if (c == 143)
	  *cur = glenz[4][*cur];
	else if (c == 159)
	  *cur = glenz[5][*cur];
	else if (c == 15)
	  *cur = glenz[7][*cur];
	else if (c == 16)
	  *cur = glenz[6][*cur];
	else
	  *cur = c;
	++cur;
	++pc;
      }
    }
  }
}

a_sprite *
compile_sprglauto (const a_pixel *src, a_pixel transp_color,
		   unsigned int block_height, unsigned int block_width,
		   unsigned int src_width, unsigned int dest_width)
{
  a_sprite *s;
  s = compile_sprrle (src, transp_color, block_height, block_width,
		      src_width, dest_width);
  s->all.kind = S_RLE_GLENZ_AUTO;
  s->draw = draw_sprglauto;
  return s;
}

void
free_sprglauto (a_sprite *prog)
{
  assert (prog->all.kind == S_RLE_GLENZ_AUTO);
  free (prog->rle.code);
  free (prog);
}
