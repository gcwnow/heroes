/*------------------------------------------------------------------.
| Copyright 1997, 1998, 2000, 2001  Alexandre Duret-Lutz            |
|                                    <duret_g@epita.fr>             |
|                                                                   |
| This file is part of Heroes.                                      |
|                                                                   |
| Heroes is free software; you can redistribute it and/or modify it |
| under the terms of the GNU General Public License version 2 as    |
| published by the Free Software Foundation.                        |
|                                                                   |
| Heroes is distributed in the hope that it will be useful, but     |
| WITHOUT ANY WARRANTY; without even the implied warranty of        |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU |
| General Public License for more details.                          |
|                                                                   |
| You should have received a copy of the GNU General Public License |
| along with this program; if not, write to the Free Software       |
| Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA          |
| 02111-1307 USA                                                    |
`------------------------------------------------------------------*/

#include "system.h"
#include "const.h"
#include "video.h"
#include "fastmem.h"
#include "visuals.h"
#include "bytesex.h"
#include "timer.h"
#include "heroes.h"
#include "camera.h"

int rotosinus[256];
int rotocosinus[256];
int rotosinus2[256];
int rotocosinus2[256];
signed char moyensinus[512];
int mulxbuf[200];
int angle;
a_pixel *srcroto;

void
rotozoom_buffer (void)
{
  a_pixel *dest = render_buffer[1] + xbuf;
  int x, y = 200;
  int debutx, debuty;
  int inx, iny;
  int deltax, deltay;
  int deltax2, deltay2;

  srcroto = corner[0];
  angle = (player[col2plr[0]].rotozoom & 511) >> 1;
  if (player[col2plr[0]].rotozoom_direction)
    angle = (-angle) & 255;
  deltay = rotosinus[angle];	/* 2; */
  deltax = rotocosinus[angle];	/* 2; */
  deltay2 = rotosinus2[angle];	/* 2; */
  deltax2 = rotocosinus2[angle];	/* 2; */
  debutx = (160 << 16) - 160 * deltax - 100 * deltax2;
  debuty = (100 << 16) - 160 * deltay - 100 * deltay2;

  do {
    inx = debutx;
    iny = debuty;
    x = 320;

    if ((unsigned) inx >= (320 << 16) || (unsigned) iny >= (200 << 16))
      do {
	*dest++ = 0;
	inx += deltax;
	iny += deltay;
	x--;
      } while ((unsigned) inx >= (320 << 16)
	       || (unsigned) iny >= (200 << 16));
    do {
      *dest++ =
	*(srcroto + (((unsigned) inx) >> 16) +
	  mulxbuf[((unsigned) iny) >> 16]);
      x--;
      inx += deltax;
      iny += deltay;

    } while ((unsigned) inx < (320 << 16) && (unsigned) iny < (200 << 16)
	     && x > 0);
    if (x > 0)
      do {
	*dest++ = 0;
	x--;
      } while (x > 0);

    debutx += deltax2;
    debuty += deltay2;
    dest += xbuf - 320;
  } while (--y);
}

void
rotozoom_half_buffer (int c)
{
  a_pixel *dest = render_buffer[c] + xbuf - 180 + xbuf;
  int x, y = 200;
  int debutx, debuty;
  int inx, iny;
  int deltax, deltay;
  int deltax2, deltay2;

  srcroto = corner[c];

  angle = (player[col2plr[c]].rotozoom & 511) >> 1;
  if (player[col2plr[0]].rotozoom_direction)
    angle = (-angle) & 255;
  deltay = rotosinus[angle];	/* 2; */
  deltax = rotocosinus[angle];	/* 2; */
  deltay2 = rotosinus2[angle];	/* 2; */
  deltax2 = rotocosinus2[angle];	/* 2; */
  debutx = (80 << 16) - 80 * deltax - 100 * deltax2;
  debuty = (100 << 16) - 80 * deltay - 100 * deltay2;

  do {
    inx = debutx;
    iny = debuty;
    x = 160;

    if ((unsigned) inx >= (160 << 16) || (unsigned) iny >= (200 << 16))
      do {
	*dest++ = 0;
	inx += deltax;
	iny += deltay;
	x--;
	if (x <= 0)
	  goto endline;
      } while ((unsigned) inx >= (160 << 16)
	       || (unsigned) iny >= (200 << 16));
    do {
      *dest++ =
	*(srcroto + (((unsigned) inx) >> 16) +
	  mulxbuf[((unsigned) iny) >> 16]);
      x--;
      if (x <= 0)
	goto endline;
      inx += deltax;
      iny += deltay;

    } while ((unsigned) inx < (160 << 16) && (unsigned) iny < (200 << 16)
	     && x > 0);
    if (x > 0)
      do {
	*dest++ = 0;
	x--;
      } while (x > 0);
  endline:
    debutx += deltax2;
    debuty += deltay2;
    dest += xbuf - 160;
  } while (--y);
}

static void
horizontal_zoom_wave (a_pixel *src, a_pixel *dest, int oldsize, int newsize)
{
  unsigned long int x = 0;
  unsigned long int deltax = 4 * (((1 + oldsize) << 16) / newsize);
  unsigned long int rest = newsize & 3;

  /* treat pixel by group of four to go faster */
  dest -= 4;
  newsize /= 4;
  do {
    a_pixel *s;
    dest += 4;
    s = src + (x >> 16);
    dest[0] = s[0];
    dest[1] = s[1];
    x += deltax;
    dest[2] = s[2];
    dest[3] = s[3];
  } while (--newsize);
  /* treat the remaining pixels individualy */
  dest += 4;
  deltax /= 4;
  while (rest--) {
    a_pixel *s = src + (x >> 16);
    *dest++ = *s;
    x += deltax;
  }
}

static void
horizontal_zoom_flip (a_pixel *src, a_pixel *dest, int oldsize, int newsize)
{
  unsigned long int x = 0;
  unsigned long int deltax = 2 * (((1 + oldsize) << 16) / newsize);
  dest -= 2;
  newsize /= 2;
  do {
    a_pixel *s;
    dest += 2;
    s = src + (x >> 16);
    dest[0] = s[0];
    x += deltax;
    dest[1] = s[1];
  } while (--newsize);
}

static void
vertical_zoom_wave (a_pixel *src, a_pixel *dest, int oldsize, int newsize)
{
  unsigned long int y = 0;	/* current line << 16 */
  unsigned long int deltay = ((1 + oldsize) << 16) / newsize;
  a_u32 tmp1, tmp2;
  newsize--;
  do {
    a_pixel *p;
    p = src + ((y >> 16) * xbuf);
    tmp1 = GETWORD(p);
    p = src + (((y + deltay) >> 16) * xbuf);
    tmp2 = GETWORD(p);
    y += deltay << 1;
    *(a_u32 *) dest = tmp1;
    *(a_u32 *) (dest + xbuf) = tmp2;
    dest += xbuf * 2;
    newsize -= 2;
  } while (newsize > 0);
  if (newsize == 0) {
    a_pixel *p;
    p = src + ((y >> 16) * xbuf);
    tmp1 = GETWORD(p);
    *(a_u32 *) dest = tmp1;
  }
}

static int
which_line (int y, int a)
{
  float an;
  a &= 1023;
  an = a * 2 * 3.1415926535 / 1024;
  return (ceil ((200.0 * y) / (200.0 * cos (an) - y * sin (an))));
}

static int
which_column (int y, int a)
{
  float an;
  a &= 1023;
  an = a * 2 * 3.1415926535 / 1024;
  return (ceil (-4 * (sin (an) * y + 200) / 5));
}
static int
which_offset (int y, int a)
{
  float an;
  a &= 1023;
  an = a * 2 * 3.1415926535 / 1024;
  return (ceil ((-160 * 200) / (y * sin (an) + 200)));
}

void
wave_buffer (void)
{
  int i, p = player[col2plr[0]].waves * 7, left, right, j;
  int waves_begin = player[col2plr[0]].waves_begin;
  for (i = 0; i < 320; i += 4) {
    left = ((16 + moyensinus[(i * 2 + p) & 511]) * waves_begin) / 128;
    right = ((16 + moyensinus[(i + p) & 511]) * waves_begin) / 128;
    for (j = 0; j < left; j++)
      *(a_u32 *) (render_buffer[1] + i + j * xbuf) = 0;
    vertical_zoom_wave (corner[0] + i, render_buffer[1] + i + left * xbuf,
			200, 200 - right - left);
    for (j = 0; j < right; j++)
      *(a_u32 *) (render_buffer[1] + i + (200 - j) * xbuf) = 0;
  }
  for (i = 0; i < 200; i++) {
    left = ((16 + moyensinus[(i * 2 + p) & 511]) * waves_begin) / 128;
    right = ((16 + moyensinus[(i - p) & 511]) * waves_begin) / 128;
    for (j = 0; j < left; j++)
      render_buffer[0][i * xbuf + j] = 0;
    horizontal_zoom_wave (render_buffer[1] + i * xbuf,
			  render_buffer[0] + i * xbuf + left, 320,
			  320 - right - left);
    for (j = 0; j < right; j++)
      render_buffer[0][i * xbuf + 319 - j] = 0;
  }
}

void
wave_half_buffer (int c)
{
  int i, p = player[col2plr[c]].waves * 7, left, right, j;
  int waves_begin = player[col2plr[c]].waves_begin;
  for (i = 0; i < 160; i += 4) {
    left = ((16 + moyensinus[(i * 2 + p) & 511]) * waves_begin) / 128;;
    right = ((16 + moyensinus[(i + p) & 511]) * waves_begin) / 128;;
    for (j = 0; j < left; j++)
      *(a_u32 *) (render_buffer[c] + 200 + i + j * xbuf) = 0;
    vertical_zoom_wave (corner[c] + i,
			render_buffer[c] + 200 + i + left * xbuf, 200,
			200 - right - left);
    for (j = 0; j < right; j++)
      *(a_u32 *) (render_buffer[c] + 200 + i + (200 - j) * xbuf) = 0;
  }
  for (i = 0; i < 200; i++) {
    left = ((16 + moyensinus[(i * 2 + p) & 511]) * waves_begin) / 128;;
    right = ((16 + moyensinus[(i - p) & 511]) * waves_begin) / 128;;
    for (j = 0; j < left; j++)
      *(render_buffer[c] + i * xbuf + j) = 0;
    horizontal_zoom_wave (render_buffer[c] + 200 + i * xbuf,
			  render_buffer[c] + i * xbuf + left, 160,
			  160 - right - left);
    for (j = 0; j < right; j++)
      *(render_buffer[c] + i * xbuf + 159 - j) = 0;
  }
}

void
flip_buffer (int p2)
{
  int i, y, left, j;
  for (i = 0; i < 200; i++) {
    y = which_line (i - 100, p2);
    if (y >= -100 && y < 100) {
      left = which_offset (y, p2) + 160;
      if (left >= 0) {
	y += 100;
	for (j = 0; j < left; j++)
	  render_buffer[1][i * xbuf + j] = 0;
	horizontal_zoom_wave (corner[0] + y * xbuf,
			      render_buffer[1] + i * xbuf + left, 320,
			      320 - left - left);
	for (j = 320 - left; j < 320; j++)
	  render_buffer[1][i * xbuf + j] = 0;
      } else {
	left = which_column (y, p2) + 160;
	y += 100;
	horizontal_zoom_flip (corner[0] + y * xbuf + left,
			      render_buffer[1] + i * xbuf,
			      320 - left - left, 320);
      }
    } else {
      memset (render_buffer[1] + i * xbuf, 0, 320);
    }
  }
}

#define corner_buffer_glenz 1
static void
corner_buffer_begin (int i)
{
  int j, k, l;
  a_pixel *src;
  a_pixel *dest;
  a_pixel *dest2;
  a_pixel *glenzl = glenz[corner_buffer_glenz];
  a_pixel *glenzd = glenz[0];

  dest2 = corner[0] + xbuf * (200 - i - 1);
  for (j = 200 - i - 1, l = 0; j < 200; j++, l++) {
    src = corner[0] + j * xbuf + l;
    dest = corner[0] + j * xbuf;
    for (k = l; k != 0; k--)
      *dest++ = 0;
    for (k = i + 1; k != 0; k--) {
      *dest = glenzl[*src];
      src += xbuf;
      dest++;
    }
    if (l > 9) {
      dest[0] = glenzd[dest[0]];
      dest[1] = glenzd[dest[1]];
      dest[2] = glenzd[dest[2]];
      dest[3] = glenzd[dest[3]];
      dest[4] = glenzd[dest[4]];
      dest[5] = glenzd[dest[5]];
      dest[6] = glenzd[dest[6]];
      dest[7] = glenzd[dest[7]];
      dest[8] = glenzd[dest[8]];
      dest[9] = glenzd[dest[9]];
      dest[10] = glenzd[dest[10]];
    }
    i--;
  }
  for (j = 200 - i - 1, l = 0; j <= 200; j++, l++) {
    dest2[-xbuf] = glenzd[dest2[-xbuf]];
    *dest2 = glenzd[glenzd[*dest2]];
    dest2 += xbuf + 1;
  }
}

static void
corner_buffer_middle (int i)
{
  int j, k, l;
  a_pixel *src;
  a_pixel *dest;
  a_pixel *dest2;
  a_pixel *glenzl = glenz[corner_buffer_glenz];
  a_pixel *glenzd = glenz[0];

  i -= 199;
  dest2 = corner[0] + i;
  for (j = 0, l = 0; j < 200; j++, l++) {
    src = corner[0] + j * xbuf + i + l;
    dest = corner[0] + j * xbuf;
    for (k = i + l; k != 0; k--)
      *dest++ = 0;
    for (k = 200 - l; k != 0; k--) {
      *dest = glenzl[*src];
      src += xbuf;
      dest++;
    }
    if (l > 9) {
      dest[0] = glenzd[dest[0]];
      dest[1] = glenzd[dest[1]];
      dest[2] = glenzd[dest[2]];
      dest[3] = glenzd[dest[3]];
      dest[4] = glenzd[dest[4]];
      dest[5] = glenzd[dest[5]];
      dest[6] = glenzd[dest[6]];
      dest[7] = glenzd[dest[7]];
      dest[8] = glenzd[dest[8]];
      dest[9] = glenzd[dest[9]];
      dest[10] = glenzd[dest[10]];
    }
  }
  for (l = 0; l < 200; l++) {
    dest2[2] = glenzd[dest2[2]];
    *dest2 = glenzd[glenzd[*dest2]];
    dest2 += xbuf + 1;
  }
}

static void
corner_buffer_end (int i)
{
  int j, k, l;
  a_pixel *src;
  a_pixel *dest;
  a_pixel *dest2;
  a_pixel *glenzl = glenz[corner_buffer_glenz];
  a_pixel *glenzd = glenz[0];

  i -= 319;
  dest2 = corner[0] + i + 120;
  for (j = 0, l = 120; j < 200; j++, l++) {
    dest = corner[0] + j * xbuf;
    if (i + l < 320) {
      src = corner[0] + j * xbuf + i + l;
      for (k = i + l; k != 0; k--)
	*dest++ = 0;
      for (k = 320 - l - i; k != 0; k--) {
	*dest = glenzl[*src];
	src += xbuf;
	dest++;
      }
    } else
      memset (dest, 0, 320);
  }
  for (l = 0; i + l < 200; l++) {
    dest2[1] = glenzd[dest2[1]];
    *dest2 = glenzd[glenzd[*dest2]];
    dest2 += xbuf + 1;
  }
}

extern int
corner_buffer (void)
{
  int t2 = read_htimer (corner_htimer);

  if (t2 < 200)
    corner_buffer_begin (t2);
  else if (t2 < 320)
    corner_buffer_middle (t2);
  else if (t2 < 520)
    corner_buffer_end (t2);
  else
    return 1;
  return 0;
}

extern void
compute_lut (void)
{
  int i;
  for (i = 0; i != 256; i++) {
    rotosinus[i] = ceil (sin (i * 3.141592653 / 128.0) * 65536 / 1.2);
    rotocosinus[i] = ceil (cos (i * 3.141592653 / 128.0) * 65536);
    rotosinus2[i] =
      ceil (sin (i * 3.141592653 / 128.0 + 3.141592653 / 2.0) * 65536);
    rotocosinus2[i] =
      ceil (cos (i * 3.141592653 / 128.0 + 3.141592653 / 2.0) * 65536 * 1.2);
  }
  for (i = 0; i != 512; i++)
    moyensinus[i] = ceil (sin (i * 2.0 * 3.141592653 / 512.0) * 16.2);
  for (i = 0; i != 200; i++)
    mulxbuf[i] = i * xbuf;
}
