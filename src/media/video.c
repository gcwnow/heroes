/*------------------------------------------------------------------------.
| Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
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
#include "video_low.h"
#include "fastmem.h"
#include "argv.h"

int scr_pitch;			/* rendering buffer pitch */

/* slow stretching routines */

static void
stretch_twofold (const a_pixel *s, a_pixel *d, unsigned width)
{
  int rows_left, columns_left;

  for (rows_left = 200; rows_left; --rows_left) {
    for (columns_left = width / 2; columns_left; --columns_left) {
      a_pixel t1, t2;
      t1 = s[0];
      t2 = s[1];
      d[0] = t1;
      d[640 + 2] = t2;
      d[1] = t1;
      d[640 + 3] = t2;
      d[640 + 0] = t1;
      d[2] = t2;
      d[640 + 1] = t1;
      d[3] = t2;
      s += 2;
      d += 4;
    }
    d += 2 * (scr_pitch - width);
    s += xbuf - width;
  }
}

static void
stretch_twofold_even (const a_pixel *s, a_pixel *d, unsigned width)
{
  int rows_left, columns_left;

  for (rows_left = 200; rows_left; --rows_left) {
    for (columns_left = width; columns_left; --columns_left) {
      d[1] = d[0] = *s;
      ++s;
      d += 2;
    }
    d += 2 * (scr_pitch - width);
    s += xbuf - width;
  }
}

static void
stretch_threefold (const a_pixel* s, a_pixel *d, unsigned width)
{
  int rows_left, columns_left;

  for (rows_left = 200; rows_left; --rows_left) {
    for (columns_left = width / 2; columns_left; --columns_left) {
      a_pixel t1, t2;
      t1 = s[0];
      t2 = s[1];
      d[0] = t1;
      d[960 + 3] = t2;
      d[2*960 + 1] = t1;
      d[4] = t2;
      d[960 + 2] = t1;
      d[2*960 + 5] = t2;
      d[960 + 0] = t1;
      d[2*960 + 3] = t2;
      d[1] = t1;
      d[960 + 4] = t2;
      d[2*960 + 2] = t1;
      d[5] = t2;
      d[2*960 + 0] = t1;
      d[3] = t2;
      d[960 + 1] = t1;
      d[2*960 + 4] = t2;
      d[2] = t1;
      d[960 + 5] = t2;
      s += 2;
      d += 6;
    }
    d += 3 * (scr_pitch - width);
    s += xbuf - width;
  }
}

static void
stretch_threefold_even (const a_pixel *s, a_pixel *d, unsigned width)
{
  int rows_left, columns_left;

  for (rows_left = 200 / 2; rows_left; --rows_left) {
    for (columns_left = width; columns_left; --columns_left) {
      a_pixel t1, t2;
      t1 = s[0];
      t2 = s[xbuf];
      d[0] = t1;
      d[0+960*4] = t2;
      d[1] = t1;
      d[2] = t1;
      d[1+960*4] = t2;
      d[0+960*2] = t1;
      d[1+960*2] = t1;
      d[2+960*4] = t2;
      d[2+960*2] = t1;
      ++s;
      d += 3;
    }
    d += 3 * (2 * scr_pitch - width);
    s += 2 * xbuf - width;
  }
}

static void
stretch_fourfold (const a_pixel *s, a_pixel *d, unsigned width)
{
  int rows_left, columns_left;

  for (rows_left = 200; rows_left; --rows_left) {
    a_u32 *d2 = (unsigned int *)d;
    for (columns_left = width; columns_left; --columns_left) {
      a_pixel c = *s;
      a_u32 i = (c << 24) | (c << 16) | (c << 8) | c;
      d2[0] = i;
      d2[320] = i;
      d2[320*2] = i;
      d2[320*3] = i;
      ++s;
      ++d2;
    }
    d += 4 * scr_pitch;
    s += xbuf - width;
  }
}

static void
stretch_fourfold_even (const a_pixel* s, a_pixel *d, unsigned width)
{
  int rows_left, columns_left;

  for (rows_left = 200; rows_left; --rows_left) {
    a_u32 *d2 = (unsigned int *)d;
    for (columns_left = width; columns_left; --columns_left) {
      a_pixel c = *s;
      a_u32 i = (c << 24) | (c << 16) | (c << 8) | c;
      d2[0] = i;
      d2[320*2] = i;
      ++s;
      ++d2;
    }
    d += 4 * scr_pitch;
    s += xbuf - width;
  }
}

static void
copy_screen_even (const a_pixel *s, a_pixel *d, unsigned width)
{
  int i;
  for (i = 200; i; --i, s += xbuf * 2, d += 2 * scr_pitch)
    fastmem4 (s, d, width / 4);
}

static void
copy_screen (const a_pixel *s, a_pixel *d, unsigned width)
{
  int i;
  for (i = 200; i; --i, s += xbuf, d += scr_pitch)
    fastmem4 (s, d, width / 4);
}

/* Copy the rendered display (s) to the visual (screen_rv).  This
   may require stretching, if the user asked for.  */
static void
copy_display (const a_pixel *s, a_pixel *d, unsigned width)
{
  /* the result of stretching routines is written directly
     to the video memory */
  if (stretch == 2) {
    if (even_lines)
      stretch_twofold_even (s, d, width);
    else
      stretch_twofold (s, d, width);
  } else if (stretch == 3) {
    if (even_lines)
      stretch_threefold_even (s, d, width);
    else
      stretch_threefold (s, d, width);
  } else if (stretch == 4) {
    if (even_lines)
      stretch_fourfold_even (s, d, width);
    else
      stretch_fourfold (s, d, width);
  } else {			/* stretch == 1 */
    if (even_lines)
      copy_screen_even (s, d, width);
    else
      copy_screen (s, d, width);
  }
}

void
init_video (void)
{
  init_video_low (stretch, &scr_pitch);
}

void
uninit_video (void)
{
  uninit_video_low ();
}

void
vsynchro (const a_pixel *s)
{
  vsynchro_low (s, copy_display);
}

void
vsynchro2 (const a_pixel *s1, const a_pixel *s2)
{
  vsynchro2_low (s1, s2, copy_display);
}
