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
#include "sprshade.h"

void draw_sprshade (const a_sprite *sprite, a_pixel *dest)
{
  a_pixel	*cur = dest;	/* current writting possition */
  a_u8		*pc;		/* program counter */
  a_u8		*epc;		/* end of program code */

  assert (sprite->all.kind == S_RLE_SHADE);

  pc = sprite->shade.code;
  epc = sprite->shade.end_code;

  while (pc < epc) {
    unsigned m, n;
    m = *pc++;
    n = *pc++;
    if (n == 0 && m == 0 && *pc == 0) {	/* end of line */
      ++pc;
      cur += sprite->shade.line_skip;
    } else {
      /* skip transparent pixels */
      cur += m;
      /* draw the opaque pixels */
      for (; n; --n)
	*cur++ = *pc++;
      /* draw the glenz pixels */
      for (m = *pc++; m; --m) {
	*cur = sprite->shade.glenz[*cur];
	++cur;
      }
    }
  }
}

a_sprite *compile_sprshade (const a_pixel *src, a_pixel transp_color,
			    a_pixel glenz_color, a_pixel *glenz_line,
			    unsigned int block_height,
			    unsigned int block_width,
			    unsigned int src_width, unsigned int dest_width)
{
  a_sprite* sprite;
  unsigned int row;
  unsigned int code_size;
  a_u8 *pc;			/* program counter */


  /* In the worst case (start with an opaque pixel and alternate
     transparant and opaque), we need four bytes to encode two
     pixels, plus four bytes for the first pixel, and three for the end
     of line. */
  code_size = block_height * ((block_width / 2 + 1) * 4 + 3);

  XMALLOC_VAR (sprite);
  XMALLOC_ARRAY (sprite->shade.code, code_size);

  /* encode the bloc */
  pc = sprite->shade.code;
  for (row = block_height; row; --row) {
    unsigned int m, n, s;
    const a_pixel *eol = src + block_width; /* end of line */

    /* encode a line */
    do {
      /* count the number of transparant pixels */
      for (m = 0; src < eol && m < 255 && *src == transp_color; ++src)
	++m;
      /* count the number of opaque pixels */
      for (n = 0; src + n < eol && n < 255
	     && src[n] != transp_color && src[n] != glenz_color;)
	++n;

      /* write the corresponding data */
      *pc++ = m;
      *pc++ = n;
      for (; n; --n)
	*pc++ = *src++;

      /* count the number of glenz pixels */
      for (s = 0; src < eol && s < 255 && *src == glenz_color; ++src)
	++s;
      /* record it */
      *pc++ = s;

    } while (src < eol);
    /* output an end of line */
    *pc++ = 0;
    *pc++ = 0;
    *pc++ = 0;

    /* prepare for next line */
    src += src_width - block_width;
  }

  sprite->shade.kind = S_RLE_SHADE;
  sprite->shade.draw = draw_sprshade;
  sprite->shade.end_code = pc;
  sprite->shade.line_skip = dest_width - block_width;
  sprite->shade.glenz = glenz_line;

  assert (pc < sprite->shade.code + code_size);

  return sprite;
}

void free_sprshade (a_sprite *prog)
{
  assert (prog->all.kind == S_RLE_SHADE);
  free (prog->shade.code);
  free (prog);
}
