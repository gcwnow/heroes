/*------------------------------------------------------------------------.
| Copyright 1996, 1997, 1998, 2000, 2001  Alexandre Duret-Lutz            |
|                                                  <duret_g@epita.fr>     |
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

/*
 * Warning: this file is really ugly.  It is among the first files
 * I written when I was learning C...  I hope to correct things as time
 * goes.
 */

#include "system.h"

#include "video.h"
#include "pcx.h"
#include "errors.h"
#include "fastmem.h"
#include "keyb.h"
#include "keyvalues.h"
#include "font.h"
#include "structs.h"
#include "misc.h"
#include "hedlite.h"
#include "bytesex.h"
#include "hendian.h"
#include "userdir.h"
#include "rsc_files.h"
#include "debugmsg.h"
#include "const.h"
#include "dirname.h"

static a_pcx_image heditrsc;
static a_pcx_image tile_set_img;

unsigned short int xdalles = 0;
unsigned short int ydalles = 0;
unsigned short int xdallesdec = 0;
unsigned short int xplan = 0;
unsigned short int yplan = 0;
unsigned int tempd = DONT_WRAP;
unsigned int xplandec = 0;
unsigned int yplandec = 0;
unsigned int sprhide = 0;
unsigned int draw_collide_tests = 0;

static a_level_header hplaninfo = {
  0, 0, -1, -1,
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  "", "", ""
};

static char *tile_set_name;
static char *dallepie;
static char levelnomshort[13];
static char pcxnom[13];
static char nombre[5];
static FILE *ftmp;

static int square_offset[4] = { 0, 12, xbuf * 10, xbuf * 10 + 12 };
static int square_offset_320[4] = { 0, 12, 3200, 3212 };

static int cote = 0;

static a_pixel *hedit_buffer;

static a_tile *level_map;
static a_tile_info *ddef;	/* tiles definitions */
static unsigned char *outwaymap;
static unsigned char *hdradar;
static unsigned char *hdcolli;

/* A few callback functions */
static void tunnel_mod (int, int, int);
static void anim_mod (int, int, int);
static void fn0 (int, int, int);
static void speed_mod (int, int, int);
static void stop_mod (int, int, int);

void (*fnptr[type_nbr]) (int, int, int) = {
  &fn0, &stop_mod, &speed_mod, &tunnel_mod, &stop_mod,
  &anim_mod, &stop_mod, &stop_mod, &fn0
};

static const char *type_name[] = {
  "NONE", "STOP", "SPEED", "TUNNEL",
  "BOOM", "ANIM", "ICE", "DUST", "OUTWAY"
};

static char spd_test[9][12] = {
  {2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 8, 8},
  {2, 2, 2, 4, 4, 4, 4, 4, 4, 8, 8, 8},
  {2, 2, 2, 2, 4, 4, 4, 4, 8, 8, 8, 8},
  {2, 2, 2, 2, 2, 4, 4, 8, 8, 8, 8, 8},
  {2, 2, 2, 2, 2, 2, 8, 8, 8, 8, 8, 8},
  {2, 2, 2, 2, 2, 1, 1, 8, 8, 8, 8, 8},
  {2, 2, 2, 2, 1, 1, 1, 1, 8, 8, 8, 8},
  {2, 2, 2, 1, 1, 1, 1, 1, 1, 8, 8, 8},
  {2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 8, 8}
};

static char dir_test[9][12] = {
  {1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3},
  {1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3},
  {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3},
  {1, 1, 1, 1, 1, 2, 2, 3, 3, 3, 3, 3},
  {1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3},
  {1, 1, 1, 1, 1, 0, 0, 3, 3, 3, 3, 3},
  {1, 1, 1, 1, 0, 0, 0, 0, 3, 3, 3, 3},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3},
  {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3}
};

static void
rectangle_copy (int xs, int ys, int xd, int yd,
		int xc, int yc, a_pcx_image *source)
{
  int src_width = source->width;
  a_pixel *src = source->buffer + src_width * ys + xs;
  a_pixel *dest = hedit_buffer + xc + yc * xbuf;
  int j;
  for (j = yd; j > 0; j--) {
    memcpy (dest, src, xd);
    src += src_width;
    dest += xbuf;
  }
}

static void
copy_tile (int src_, a_pixel *dest)
{
  int i = tile_set_img.width;
  int j;
  a_pixel *src = tile_set_img.buffer + src_;
  for (j = 20; j > 0; j--) {
    fastmem4 (src, dest, 24 >> 2);
    src += i;
    dest += xbuf;
  }
}

static void
copy_tile_pcx (int src_, a_pixel *dest)
{
  int i = tile_set_img.width;
  int j;
  a_pixel *src = tile_set_img.buffer + src_;
  for (j = 20; j > 0; j--) {
    fastmem4 (src, dest, 24 >> 2);
    src += i;
    dest += hplaninfo.xt * 24;
  }
}

static void
copy_tile_transp (int src_, a_pixel *dest)
{
  int i = tile_set_img.width;
  int j, k;
  a_pixel *src = tile_set_img.buffer + src_;
  for (j = 20; j > 0; j--) {
    for (k = 24; k > 0; k--) {
      if (*src != 0)
	*dest = *src;
      src++;
      dest++;
    }
    src += i - 24;
    dest += xbuf - 24;
  }
}

static void
copy_tile_transp_pcx (int src_, a_pixel *dest)
{
  int i = tile_set_img.width;
  int j, k;
  a_pixel *src = tile_set_img.buffer + src_;
  for (j = 20; j > 0; j--) {
    for (k = 24; k > 0; k--) {
      if (*src != 0)
	*dest = *src;
      src++;
      dest++;
    }
    src += i - 24;
    dest += hplaninfo.xt * 24 - 24;
  }
}

static void
copy_square_transp (a_pixel *src, a_pixel *dest)
{
  int j, k;
  for (j = 10; j > 0; j--) {
    for (k = 12; k > 0; k--) {
      if (*src != 0)
	*dest = *src;
      src++;
      dest++;
    }
    src += 320 - 12;
    dest += xbuf - 12;
  }
}

static void
copy_square_transp_pcx (a_pixel *src, a_pixel *dest)
{
  int j, k;
  for (j = 10; j > 0; j--) {
    for (k = 12; k > 0; k--) {
      if (*src != 0)
	*dest = *src;
      src++;
      dest++;
    }
    src += 320 - 12;
    dest += hplaninfo.xt * 24 - 12;
  }
}

static void
frame (int x, int y, int xd, int yd, a_pixel col)
{
  int i;
  a_pixel *dest = hedit_buffer + y * xbuf + x;
  for (i = xd; i > 0; i--) {
    dest[yd * xbuf] = col;
    *dest++ = col;
  };
  for (i = yd; i >= 0; i--) {
    dest[-xd] = col;
    *dest = col;
    dest += xbuf;
  };
}

static void
frame_dashed (int x, int y, int xd, int yd, a_pixel col1, a_pixel col2)
{
  int i;
  a_pixel *dest = hedit_buffer + y * xbuf + x;
  for (i = (xd >> 1); i > 0; i--) {
    dest[(yd - 1) * xbuf] = col1;
    *dest++ = col2;
    dest[(yd - 1) * xbuf] = col2;
    *dest++ = col1;
  };
  xd--;
  dest--;
  for (i = (yd >> 1); i > 0; i--) {
    dest[-xd] = col2;
    *dest = col1;
    dest += xbuf;
    dest[-xd] = col1;
    *dest = col2;
    dest += xbuf;
  };
}

static void
draw_text (const char *texte, int posx, int posy, a_pixel coul, char cent)
{
  int i, j;
  int k1, k2, d = -1;
  a_pixel *dest = hedit_buffer + posx + posy * xbuf;
  a_pixel *src;
  const char *tmp = texte;
  for (; *tmp != 0; tmp++) {
    i = *tmp - font_first_ascii;
    d += font_width[i] + 1;
  }
  if (cent == 0)
    d = 0;
  if (cent == 1)
    d = -(d >> 1);
  if (cent == 2)
    d = -d;
  dest += d;
  for (; *texte != 0; texte++) {
    i = *texte - font_first_ascii;
    src = heditrsc.buffer + font_pos + ((int) (i) << 2);
    for (j = font_width[i]; j > 0; j--) {
      for (k1 = 320 * (font_height - 1), k2 = xbuf * (font_height - 1);
	   k1 >= 0; k1 -= 320, k2 -= xbuf)
	if (src[k1] != font_transp_color)
	  dest[k2] = coul;
      dest++;
      src++;
    }
    dest++;
  }
}


static void
transpa (a_pixel *source, a_pixel *dest, int xt, int yt, char coul)
{
  int x, y;
  for (y = yt; y > 0; y--) {
    for (x = xt; x > 0; x--)
      if (*source != coul)
	*dest++ = *source++;
      else {
	dest++;
	source++;
      }
    source += 320 - xt;
    dest += xbuf - xt;
  };
}

static void
transpac (a_pixel *source, a_pixel *dest, int xt, int yt, char coul)
{
  int x, y;
  for (y = yt; y > 0; y--) {
    for (x = xt; x > 0; x--)
      if (*source != coul)
	*dest++ = *source++;
      else {
	dest++;
	source++;
      }
    source += 320 - xt;
    dest += hplaninfo.xt * 24 - xt;
  };
}

static void
draw_check_point (int x, int y, a_pixel c)
{
  a_pixel *dest = hedit_buffer + x + y * xbuf;

  if (c)
    c = 8;

  *dest = 0;
  dest[0] = 0;
  dest[1] = 0;
  dest[2] = 0;
  dest[3] = 0;
  dest += xbuf;
  dest[0] = 0;
  dest[1] = c;
  dest[2] = c;
  dest[3] = 0;
  dest += xbuf;
  dest[0] = 0;
  dest[1] = c;
  dest[2] = c;
  dest[3] = 0;
  dest += xbuf;
  dest[0] = 0;
  dest[1] = 0;
  dest[2] = 0;
  dest[3] = 0;
}

static void
draw_left_panel (int t)
{
  switch (level_map[t].type) {
  case t_speed:
    transpa (IMGPOS (heditrsc, 20, 30 + level_map[t].info.param[0] * 12 + 1),
	     hedit_buffer + 294 + 88 * xbuf, 10, 9, 71);
    transpa (IMGPOS (heditrsc, 20, 30 + level_map[t].info.param[1] * 12 + 1),
	     hedit_buffer + 306 + 88 * xbuf, 10, 9, 71);
    transpa (IMGPOS (heditrsc, 20, 30 + level_map[t].info.param[2] * 12 + 1),
	     hedit_buffer + 294 + 99 * xbuf, 10, 9, 71);
    transpa (IMGPOS (heditrsc, 20, 30 + level_map[t].info.param[3] * 12 + 1),
	     hedit_buffer + 306 + 99 * xbuf, 10, 9, 71);
    rectangle_copy (0, 112, 30, 27, 290, 112, &heditrsc);

    if ((level_map[t].info.param[4] >> 4) > 0) {
      rectangle_copy (60, 168, 30, 32, 290, 168, &heditrsc);
      sprintf (nombre, "%u", level_map[t].info.param[4] & 0xf);
      draw_text (nombre, 311, 193, 8, 1);
    } else
      rectangle_copy (0, 168, 30, 32, 290, 168, &heditrsc);
    break;
  case t_dust:
  case t_ice:
  case t_boom:
  case t_stop:
    draw_check_point (297, 91, level_map[t].info.param[0]);
    draw_check_point (297 + 12, 91, level_map[t].info.param[1]);
    draw_check_point (297, 91 + 10, level_map[t].info.param[2]);
    draw_check_point (297 + 12, 91 + 10, level_map[t].info.param[3]);
    rectangle_copy (0, 112, 30, 27, 290, 112, &heditrsc);
  case t_outway:
    if ((level_map[t].info.param[4] >> 4) > 0) {
      rectangle_copy (60, 168, 30, 32, 290, 168, &heditrsc);
      sprintf (nombre, "%u", level_map[t].info.param[4] & 0xf);
      draw_text (nombre, 311, 193, 8, 1);
    } else
      rectangle_copy (0, 168, 30, 32, 290, 168, &heditrsc);
    break;
  case t_tunnel:
    transpa (IMGPOS (heditrsc, 20,
		     30 + level_map[t].info.tunnel.direction * 12 + 1),
	     hedit_buffer + 294 + 88 * xbuf, 10, 9, 71);
    transpa (IMGPOS (heditrsc, 20,
		     30 + level_map[t].info.tunnel.direction * 12 + 1),
	     hedit_buffer + 306 + 88 * xbuf, 10, 9, 71);
    transpa (IMGPOS (heditrsc, 20,
		     30 + level_map[t].info.tunnel.direction * 12 + 1),
	     hedit_buffer + 294 + 99 * xbuf, 10, 9, 71);
    transpa (IMGPOS (heditrsc, 20,
		     30 + level_map[t].info.tunnel.direction * 12 + 1),
	     hedit_buffer + 306 + 99 * xbuf, 10, 9, 71);
    rectangle_copy (120, 112, 30, 27, 290, 112, &heditrsc);
    sprintf (nombre, "%u", level_map[t].info.tunnel.output / hplaninfo.xt);
    draw_text (nombre, 307, 133, 8, 0);
    sprintf (nombre, "%u", level_map[t].info.tunnel.output % hplaninfo.xt);
    draw_text (nombre, 302, 133, 8, 2);
    break;
  case t_anim:
    rectangle_copy (150 + 60, 112, 30, 27, 290, 112, &heditrsc);
    sprintf (nombre, "%u", level_map[t].info.anim.speed);
    draw_text (nombre, 311, 133, 8, 1);
    rectangle_copy (0, 168, 30, 32, 290, 168, &heditrsc);
    break;
  default:
    rectangle_copy (0, 112, 30, 27, 290, 112, &heditrsc);
    rectangle_copy (0, 168, 30, 32, 290, 168, &heditrsc);
    break;

  }
}

static void
fn0 (int a ATTRIBUTE_UNUSED,
     int b ATTRIBUTE_UNUSED, int c ATTRIBUTE_UNUSED)
{
}

static void
speed_mod (int i, int x, int y)
{
  int l = 0;
  unsigned char m;

  if (y < 112) {
    if (y > 98) {
      l = 2;
      y = y - 99;
    } else
      y = y - 88;
    if (x >= 305) {
      l++;
      x = x - 305;
    } else
      x = x - 293;
    if (x > 11 || y > 8 || x < 0 || y < 0)
      return;
    m = spd_test[y][x] & 0xf;
    if (cote)
      ddef[i].info.param[l] ^= m;
    else
      level_map[i].info.param[l] ^= m;
  } else {
    m = 1 << ((y - 112) / 7);
    for (l = 3; l >= 0; l--) {
      if (cote)
	ddef[i].info.param[l] ^= m;
      else
	level_map[i].info.param[l] ^= m;
    }
  }
}

static void
stop_mod (int i, int x, int y)
{
  int l = 0;
  if (y < 112) {
    if (y > 98)
      l = 2;
    if (x >= 305)
      l++;
    if (cote)
      ddef[i].info.param[l] ^= 1;
    else
      level_map[i].info.param[l] ^= 1;
  } else {
    if (y >= 112 && y < 119) {
      if (cote)
	ddef[i].info.tunnel.output = 0x1010101;
      else
	level_map[i].info.tunnel.output = 0x1010101;
    } else if (y >= 119 && y < 126) {
      if (cote)
	ddef[i].info.tunnel.output = 0;
      else
	level_map[i].info.tunnel.output = 0;
    }
  }
}

static void
tunnel_mod (int i, int x ATTRIBUTE_UNUSED, int y)
{
  if (y > 126 && y < 133 && tempd != DONT_WRAP)
    level_map[i].info.tunnel.output = tempd;
}

static void
anim_mod (int i, int x, int y)
{
  unsigned char m;

  if (y >= 112 && x < 305) {
    m = ((y - 112) / 7);
    {
      if (m >= 2)
	level_map[i].info.anim.speed += ((m == 2) ? +1 : -1);
    }
  }
}

static void
anim_mod_bcl (int i, int x, int y)
{
  unsigned char m;

  if (y >= 172 && x < 305) {
    m = ((y - 172) / 7);
    {
      if (m >= 2 && (level_map[i].info.param[4] >> 4) > 0)

	level_map[i].info.param[4] =
	  (level_map[i].info.param[4] & 0xf0) |
	  ((level_map[i].info.param[4] + ((m == 2) ? +1 : -1)) & 0xf);
    }
  }
}

static void
update_middle_panel (void)
{
  rectangle_copy (xdalles, 0, 144, 200, 145, 0, &tile_set_img);
  frame (145 + xdallesdec, ydalles, 23, 19, 15);
  rectangle_copy (0, 64, 30, 6, 290, 64, &heditrsc);
  sprintf (nombre, "%u", (xdalles + xdallesdec) / 24);
  draw_text (nombre, 302, 64, 15, 2);
  sprintf (nombre, "%u", ydalles / 20);
  draw_text (nombre, 307, 64, 15, 0);
  vsynchro (hedit_buffer);
}

static void
draw_level_map (int xloc, int yloc, char c)
{
  int j;
  unsigned int i, k, m;
  int l, n;
  a_pixel *dest = hedit_buffer;
  int xx, yy = 7;
  for (k = yloc, l = 10; l > 0; l--, k = ((k + 1) & hplaninfo.ywrap)) {
    m = k * hplaninfo.xt;
    xx = 12;
    for (i = xloc, j = 6 + c; j > 0; j--, i = ((i + 1) & hplaninfo.xwrap)) {
      copy_tile (level_map[i + m].number, dest);
      if (sprhide == 0) {
	for (n = 0; n < 4; n++)
	  if (i + m == hplaninfo.start[n])
	    copy_square_transp (heditrsc.buffer + (16 + (n << 4)) * 320 +
				256 + (hplaninfo.start_way[n] & 0xf0),
				dest +
				square_offset[hplaninfo.start_way[n] & 0xf]);
	if (level_map[i + m].sprite != 0)
	  copy_tile_transp (level_map[i + m].sprite, dest);
      }
      if (draw_collide_tests) {
	if (level_map[i + m].type == t_outway)
	  transpa (heditrsc.buffer + 10 * 320 + 222, dest, 24, 20, 0);
	else
	  for (n = 0; n < 4; n++)
	    if (level_map[i + m].collision[n] == 0xf)
	      copy_square_transp (heditrsc.buffer + 10 * 320 + 222 +
				  square_offset_320[n],
				  dest + square_offset[n]);
	    else
	      copy_square_transp (heditrsc.buffer + 10 * 320 + 30 +
				  level_map[i + m].collision[n] * 12,
				  dest + square_offset[n]);
      }
      if (i + m == tempd)
	frame_dashed (xx - 12, yy - 7, 24, 20, 8, 15);
      dest += 24;
      xx += 24;
    }
    dest += 20 * xbuf - 24 * (6 + c);
    yy += 20;
  }
  vsynchro (hedit_buffer);
}

static unsigned int
curdallep (void)
{
  unsigned int i =
    ((xplan + xplandec / 24) & hplaninfo.xwrap) +
    ((yplan + yplandec / 20) & hplaninfo.ywrap) * hplaninfo.xt;
  return i;
}

static int
curdallepg (int c)
{
  int x = c % hplaninfo.xt;
  if (x > 0)
    return (c - 1);
  else if (hplaninfo.xwrap != DONT_WRAP)
    return (c + hplaninfo.xt - 1);
  return -1;
}

static int
curdalleph (int c)
{
  int y = c / hplaninfo.xt;
  if (y > 0)
    return (c - hplaninfo.xt);
  else if (hplaninfo.ywrap != DONT_WRAP)
    return (c + hplaninfo.xt * (hplaninfo.yt - 1));
  return -1;
}

static int
curdallepd (int c)
{
  unsigned int x = c % hplaninfo.xt;
  if (x + 1 < hplaninfo.xt)
    return (c + 1);
  else if (hplaninfo.xwrap != DONT_WRAP)
    return (c - hplaninfo.xt + 1);
  return -1;
}

static int
curdallepb (int c)
{
  unsigned int y = c / hplaninfo.xt;
  if (y + 1 < hplaninfo.yt)
    return (c + hplaninfo.xt);
  else if (hplaninfo.ywrap != DONT_WRAP)
    return (c % hplaninfo.xt);
  return -1;
}

static int
curdalled (void)
{
  int i =
    (xdalles + xdallesdec) / 24 + (ydalles / 20) * (tile_set_img.width / 24);
  return i;
}

static void
update_left_panel (void)
{
  draw_level_map (xplan, yplan, 0);
  {
    copy_tile (level_map
	       [((xplan + xplandec / 24) & hplaninfo.xwrap) +
		((yplan + yplandec / 20) & hplaninfo.ywrap) *
		hplaninfo.xt].number, hedit_buffer + 293 + 88 * xbuf);
    draw_left_panel (curdallep ());
    rectangle_copy (0, 71, 30, 13, 290, 71, &heditrsc);
    if (level_map[curdallep ()].sprite != 0)
      draw_text ("[S]", 309, 71, 8, 0);
    draw_text (type_name[level_map[curdallep ()].type], 304, 78, 8, 1);
  }
  frame (xplandec, yplandec, 23, 19, 8);
  rectangle_copy (0, 57, 30, 6, 290, 57, &heditrsc);
  sprintf (nombre, "%u", ((xplan + xplandec / 24) & hplaninfo.xwrap));
  draw_text (nombre, 302, 57, 8, 2);
  sprintf (nombre, "%u", ((yplan + yplandec / 20) & hplaninfo.ywrap));
  draw_text (nombre, 307, 57, 8, 0);
  vsynchro (hedit_buffer);
}

static void
departfix (void)
{
  unsigned int d = curdallep (), i;
  signed int x, y;
  static unsigned char c = 0;
  unsigned char l = 0, m, flag;

  copy_tile (level_map
	     [((xplan + xplandec / 24) & hplaninfo.xwrap) +
	      ((yplan + yplandec / 20) & hplaninfo.ywrap) *
	      hplaninfo.xt].number, hedit_buffer + 293 + 88 * xbuf);

  while (mouse12 () != 0);
  do {
    transpa (heditrsc.buffer + 31 + 20 * 320, hedit_buffer + 294 + 88 * xbuf, 10, 9,
	     71);
    for (i = 0; i < 4; i++)
      if (hplaninfo.start[i] == d && (hplaninfo.start_way[i] & 0xf) == 0)
	transpa (IMGPOS (heditrsc, 29 + i * 9,
			 31 + (hplaninfo.start_way[i] >> 4) * 12),
		 hedit_buffer + 294 + 88 * xbuf, 10, 9, 71);
    transpa (IMGPOS (heditrsc, 20, 31),
	     hedit_buffer + 306 + 88 * xbuf, 10, 9, 71);
    for (i = 0; i < 4; i++)
      if (hplaninfo.start[i] == d && (hplaninfo.start_way[i] & 0xf) == 1)
	transpa (IMGPOS (heditrsc, 29 + i * 9,
			 31 + (hplaninfo.start_way[i] >> 4) * 12),
		 hedit_buffer + 306 + 88 * xbuf, 10, 9, 71);
    transpa (IMGPOS (heditrsc, 20, 31),
	     hedit_buffer + 294 + 99 * xbuf, 10, 9, 71);
    for (i = 0; i < 4; i++)
      if (hplaninfo.start[i] == d && (hplaninfo.start_way[i] & 0xf) == 2)
	transpa (IMGPOS (heditrsc, 29 + i * 9,
			 31 + (hplaninfo.start_way[i] >> 4) * 12),
		 hedit_buffer + 294 + 99 * xbuf, 10, 9, 71);
    transpa (IMGPOS (heditrsc, 20, 31),
	     hedit_buffer + 306 + 99 * xbuf, 10, 9, 71);
    for (i = 0; i < 4; i++)
      if (hplaninfo.start[i] == d && (hplaninfo.start_way[i] & 0xf) == 3)
	transpa (IMGPOS (heditrsc, 29 + i * 9,
			 31 + (hplaninfo.start_way[i] >> 4) * 12),
		 hedit_buffer + 306 + 99 * xbuf, 10, 9, 71);
    rectangle_copy (90, 112, 30, 27, 290, 112, &heditrsc);
    rectangle_copy (96 + c * 5, 139, 4, 4, 296 + c * 5, 112, &heditrsc);
    vsynchro (hedit_buffer);
    while (mouse12 () == 0 && key_ready () == 0);
    if (mouse1 ()) {
      x = mouse_coord_x ();
      y = mouse_coord_y ();
      if (y < 112) {
	l = 0;
	if (y > 98) {
	  l = 2;
	  y = y - 99;
	} else
	  y = y - 88;
	if (x >= 305) {
	  l++;
	  x = x - 305;
	} else
	  x = x - 293;
	if (!(x > 11 || y > 8 || x < 0 || y < 0)) {
	  flag = 0;
	  m = dir_test[y][x] & 0x3;
	  for (i = 0; i < 4; i++)
	    if (hplaninfo.start[i] == d
		&& (hplaninfo.start_way[i] & 0xf) == l && i != c)
	      flag = 1;
	  if (flag == 0) {
	    hplaninfo.start[c] = d;
	    hplaninfo.start_way[c] = (m << 4) + l;
	    draw_level_map (xplan, yplan, 0);
	  }
	  while (mouse12 () != 0);
	}
      } else {
	if (y < 116 && x >= 296 && x <= 316)
	  c = (x - 296) / 5;
	while (mouse12 () != 0);
      }
    }
  }
  while (mouse_coord_x () > 290 && (!key_ready ()) && mouse2 () == 0);
  update_left_panel ();
}

static void
write_rle (a_pixel *src, int t, FILE * fpcx)
{
  a_pixel old, new;
  int i;
  int nbr = 1;

  old = *src++;

  for (i = 1; i < t; i++) {
    new = *src++;
    if (nbr == 63 || (nbr > 1 && (new != old))) {
      putc (nbr | 192, fpcx);
      putc (old, fpcx);
      old = new;
      nbr = 1;
    } else if (old == new && nbr < 63)
      nbr++;
    else {
      if (old < 192)
	putc (old, fpcx);
      else {
	putc (193, fpcx);
	putc (old, fpcx);
      }
      old = new;
    }
  }
  if (nbr == 1) {
    if (old < 192)
      putc (old, fpcx);
    else {
      putc (193, fpcx);
      putc (old, fpcx);
    }
  } else {
    putc (nbr | 192, fpcx);
    putc (old, fpcx);
  }
}

static void
save_level_as_pcx (void)
{
  FILE *fpcx;
  a_pcx_header headpcx;
  unsigned int i1, i3, n;
  unsigned int j3;
  a_pixel *tempc;
  a_pixel *dest;
  int sdec[4];

  XMALLOC_ARRAY (tempc, hplaninfo.xt * 20 * 24);
  sdec[0] = 0;
  sdec[1] = 12;
  sdec[2] = hplaninfo.xt * 24 * 10;
  sdec[3] = hplaninfo.xt * 24 * 10 + 12;

  headpcx.signature = 10;
  headpcx.version = 5;
  headpcx.rle = 1;
  headpcx.bits_per_pixels = 8;
  headpcx.x = headpcx.y = BSWAP16 (0);
  headpcx.palette_kind = BSWAP16 (1);
  headpcx.width = BSWAP16 ((hplaninfo.xt * 24) - 1);
  headpcx.height = BSWAP16 ((hplaninfo.yt * 20) - 1);
  headpcx.bytes_per_lines = headpcx.widthdpi = BSWAP16 (hplaninfo.xt * 24);
  headpcx.heightdpi = BSWAP16 (hplaninfo.yt * 20);
  headpcx.nbrplanes = 1;
  if ((fpcx = fopen (pcxnom, "wb")) == NULL)
    return;
  fwrite ((char *) &headpcx, 1, sizeof (a_pcx_header), fpcx);

  for (i3 = 0; i3 < hplaninfo.yt; i3++) {
    j3 = hplaninfo.xt * i3;
    dest = tempc;
    for (i1 = 0; i1 < hplaninfo.xt; i1++) {
      copy_tile_pcx (level_map[i1 + j3].number, dest);
      if (sprhide == 0) {
	for (n = 0; n < 4; n++)
	  if (i1 + j3 == hplaninfo.start[n])
	    copy_square_transp_pcx (heditrsc.buffer +
				    (16 + (n << 4)) * 320 + 256 +
				    (hplaninfo.start_way[n] & 0xf0),
				    dest + sdec[hplaninfo.start_way[n] & 0xf]);
	if (level_map[i1 + j3].sprite != 0)
	  copy_tile_transp_pcx (level_map[i1 + j3].sprite, dest);
      }
      if (draw_collide_tests) {
	if (level_map[i1 + j3].type == t_outway)
	  transpac (heditrsc.buffer + 10 * 320 + 222, dest, 24, 20, 0);
	else
	  for (n = 0; n < 4; n++)
	    if (level_map[i1 + j3].collision[n] == 0xf)
	      copy_square_transp_pcx (heditrsc.buffer + 10 * 320 + 222 +
				      square_offset_320[n],
				      dest + square_offset[n]);
	    else
	      copy_square_transp_pcx (heditrsc.buffer + 10 * 320 + 30 +
				      level_map[i1 + j3].collision[n] * 12,
				      dest + square_offset[n]);
      }
      dest += 24;
    }
    write_rle (tempc, hplaninfo.xt * 20 * 24, fpcx);
  }
  free (tempc);
  putc (0xC, fpcx);
  for (i1 = 0; i1 < 768; i1++)
    putc (tile_set_img.palette.global[i1] << 2, fpcx);
  fclose (fpcx);
}

static void
display_level_map_fullscreen (void)
{
  a_keycode t;
  int x, y, xm = 128, ym = 100;

  memset (hedit_buffer, 0, xbuf * 200);
  if (xplan > (hplaninfo.xt - 13) && hplaninfo.xwrap == DONT_WRAP)
    xplan = hplaninfo.xt - 13;
  draw_level_map (xplan, yplan, 7);
  while (mouse12 () != 0);
  do {
    draw_level_map (xplan, yplan, 7);

    t = 0;
    while (key_ready () == 0 && (xm - mouse_coord_x ()) <= 1
	   && (mouse_coord_x () - xm) <= 1 && (ym - mouse_coord_y ()) <= 1
	   && (mouse_coord_y () - ym) <= 1 && mouse12 () == 0);
    if (key_ready ())
      t = get_key ();
    x = mouse_coord_x ();
    y = mouse_coord_y ();
    if (t == HK_Right || (x - xm) > 1) {
      if (xplan < (hplaninfo.xt - 13) || hplaninfo.xwrap != DONT_WRAP)
	xplan = ((xplan + 1) & hplaninfo.xwrap);
      xm = x;
    }
    if (t == HK_Left || (xm - x) > 1) {
      if (xplan > 0 || hplaninfo.xwrap != DONT_WRAP)
	xplan = ((xplan - 1) & hplaninfo.xwrap);
      xm = x;
    }
    if (t == HK_Down || (y - ym) > 1) {
      if (yplan < (hplaninfo.yt - 10) || hplaninfo.ywrap != DONT_WRAP)
	yplan = ((yplan + 1) & hplaninfo.ywrap);
      ym = y;
    }
    if (t == HK_Up || (ym - y) > 1) {
      if (yplan > 0 || hplaninfo.ywrap != DONT_WRAP)
	yplan = ((yplan - 1) & hplaninfo.ywrap);
      ym = y;
    }
  } while (t != HK_Enter && t != HK_Space && t != HK_Escape &&
	   mouse12 () == 0);
  memset (hedit_buffer, 0, xbuf * 200);
  rectangle_copy (0, 0, 30, 200, 290, 0, &heditrsc);
  draw_text (levelnomshort, 305, 29, 8, 1);
  sprintf (nombre, "%u", hplaninfo.xt);
  draw_text (nombre, 302, 43, 8, 2);
  sprintf (nombre, "%u", hplaninfo.yt);
  draw_text (nombre, 307, 43, 8, 0);
  update_middle_panel ();
  update_left_panel ();
}

static void
outwayinit (void)
{
  XMALLOC_ARRAY (outwaymap, hplaninfo.xt * hplaninfo.yt * 4);
  XMALLOC_ARRAY (hdradar, hplaninfo.xt * hplaninfo.yt * 4);
  XMALLOC_ARRAY (hdcolli, hplaninfo.xt * hplaninfo.yt * 4);
}

static void
outwayflag (void)
{
  char flag, flag2;
  int d, tmp, tmp2;
  unsigned int i, j, k, l;

  memset (outwaymap, 0, hplaninfo.xt * hplaninfo.yt * 4);
  memset (hdradar, 0, hplaninfo.xt * hplaninfo.yt * 4);
  memset (hdcolli, 0, hplaninfo.xt * hplaninfo.yt * 4);

  j = 0;
  l = 0;
  for (k = 0; k < hplaninfo.yt; k++) {
    for (i = 0; i < hplaninfo.xt; i++) {
      hdradar[j] = level_map[i + l].collision[0];
      hdradar[j + 1] = level_map[i + l].collision[1];
      hdradar[j + hplaninfo.xt * 2] = level_map[i + l].collision[2];
      hdradar[j + hplaninfo.xt * 2 + 1] = level_map[i + l].collision[3];
      j += 2;
    }
    j += hplaninfo.xt * 2;
    l += hplaninfo.xt;
  }
  if (hplaninfo.ywrap != DONT_WRAP) {
    for (i = 0; i < hplaninfo.xt * 2; i++) {
      if (hdradar[(hplaninfo.ywrap * 2 + 1) * hplaninfo.xt * 2 + i] & c_down)
	hdcolli[i] |= d_up;
      if (hdradar[i] & c_up)
	hdcolli[(hplaninfo.ywrap * 2 + 1) * hplaninfo.xt * 2 + i] |= d_down;
    }
  } else
    for (i = 0; i < hplaninfo.xt * 2; i++) {
      hdcolli[i] |= d_up;
      hdcolli[(hplaninfo.yt * 2 - 1) * hplaninfo.xt * 2 + i] |= d_down;
    };
  if (hplaninfo.xwrap != DONT_WRAP) {
    for (i = 0; i < hplaninfo.yt * 2; i++) {
      if (hdradar[i * hplaninfo.xt * 2 + hplaninfo.xwrap * 2 + 1] & c_right)
	hdcolli[i * hplaninfo.xt * 2] |= d_left;
      if (hdradar[i * hplaninfo.xt * 2] & c_left)
	hdcolli[hplaninfo.xwrap * 2 + 1 + i * hplaninfo.xt * 2] |= d_right;
    }
  } else
    for (i = 0; i < hplaninfo.yt * 2; i++) {
      hdcolli[i * hplaninfo.xt * 2] |= d_left;
      hdcolli[hplaninfo.xt * 2 - 1 + i * hplaninfo.xt * 2] |= d_right;
    };
  j = 0;
  for (k = hplaninfo.yt * 2 - 1; k != 0; k--) {
    for (i = 0; i < hplaninfo.xt * 2; i++) {
      if (hdradar[j + i] & c_down)
	hdcolli[j + i + hplaninfo.xt * 2] |= d_up;
      if (hdradar[j + i + hplaninfo.xt * 2] & c_up)
	hdcolli[j + i] |= d_down;
    }
    j += hplaninfo.xt * 2;
  }
  j = 0;
  for (k = hplaninfo.yt * 2; k != 0; k--) {
    for (i = 0; i < hplaninfo.xt * 2 - 1; i++) {
      if (hdradar[j + i] & c_right)
	hdcolli[j + i + 1] |= d_left;
      if (hdradar[j + i + 1] & c_left)
	hdcolli[j + i] |= d_right;
    }
    j += hplaninfo.xt * 2;
  }

  for (k = 0; k < 4; k++) {
    l = hplaninfo.start_way[k] & 15;
    i = (hplaninfo.start[k] % hplaninfo.xt) * 2;
    j = (hplaninfo.start[k] / hplaninfo.xt) * 2;
    if (l == 1)
      i++;
    else if (l == 2)
      j++;
    else if (l == 3) {
      i++;
      j++;
    }
    outwaymap[i + j * hplaninfo.xt * 2] = 1;
  }

  k = 0;
  l = 0;
  for (j = 0; j < hplaninfo.yt; j++) {
    for (i = 0; i < hplaninfo.xt; i++, l++)
      if (level_map[l].type == t_tunnel) {
	outwaymap[i * 2 + k] = 1;
	outwaymap[i * 2 + k + 1] = 1;
	outwaymap[i * 2 + k + hplaninfo.xt * 2] = 1;
	outwaymap[i * 2 + k + hplaninfo.xt * 2 + 1] = 1;
      }
    k += hplaninfo.xt << 2;
  }

  do {
    flag = 0;
    for (k = 0, j = 0; k < 2 * hplaninfo.yt; k++, j += 2 * hplaninfo.xt)
      for (i = 0; i < 2 * hplaninfo.xt; i++) {
	d = j + i;
	if (outwaymap[d] == 1) {
	  flag2 = 0;
	  if (!(hdcolli[d] & d_up)) {
	    tmp = (k - 1) & ((hplaninfo.ywrap << 1) + 1);
	    tmp2 = i + tmp * hplaninfo.xt * 2;
	    if (!outwaymap[tmp2]) {
	      outwaymap[tmp2] = 1;
	      flag2 = 1;
	    }
	  }
	  if (!(hdcolli[d] & d_down)) {
	    tmp = (k + 1) & ((hplaninfo.ywrap << 1) + 1);
	    tmp2 = i + tmp * hplaninfo.xt * 2;
	    if (!outwaymap[tmp2]) {
	      outwaymap[tmp2] = 1;
	      flag2 = 1;
	    }
	  }
	  if (!(hdcolli[d] & d_right)) {
	    tmp = (i + 1) & ((hplaninfo.xwrap << 1) + 1);
	    tmp2 = tmp + k * hplaninfo.xt * 2;
	    if (!outwaymap[tmp2]) {
	      outwaymap[tmp2] = 1;
	      flag2 = 1;
	    }
	  }
	  if (!(hdcolli[d] & d_left)) {
	    tmp = (i - 1) & ((hplaninfo.xwrap << 1) + 1);
	    tmp2 = tmp + k * hplaninfo.xt * 2;
	    if (!outwaymap[tmp2]) {
	      outwaymap[tmp2] = 1;
	      flag2 = 1;
	    }
	  }
	  if (flag2 == 0)
	    outwaymap[d] = 2;
	  else
	    flag = 1;
	}
      }
  } while (flag);

  k = 0;
  l = 0;
  for (j = 0; j < hplaninfo.yt; j++) {
    for (i = 0; i < hplaninfo.xt; i++, l++) {
      if (!(outwaymap[i * 2 + k]))
	level_map[l].collision[0] = 15;
      if (!(outwaymap[i * 2 + k + 1]))
	level_map[l].collision[1] = 15;
      if (!(outwaymap[(i + hplaninfo.xt) * 2 + k]))
	level_map[l].collision[2] = 15;
      if (!(outwaymap[(i + hplaninfo.xt) * 2 + k + 1]))
	level_map[l].collision[3] = 15;
      if (!(outwaymap[i * 2 + k] || outwaymap[i * 2 + 1 + k]
	    || outwaymap[i * 2 + hplaninfo.xt * 2 + k]
	    || outwaymap[i * 2 + hplaninfo.xt * 2 + 1 + k])) {
	if (level_map[l].type != t_anim)
	  level_map[l].type = t_outway;
	level_map[l].collision[0] = 15;
	level_map[l].collision[1] = 15;
	level_map[l].collision[2] = 15;
	level_map[l].collision[3] = 15;
      }
    }
    k += hplaninfo.xt << 2;
  }
}

static void
outwayclose (void)
{
  free (outwaymap);
  free (hdradar);
  free (hdcolli);
}

static void
display_level_map_animated (void)
{
  unsigned int i, m;
  int k, l, j, n;
  unsigned int tmp;
  a_pixel *dest;
  int xx, yy, t = 0;

  do {
    dest = hedit_buffer;
    yy = 7;
    vsynchro (hedit_buffer);
    for (k = yplan, l = 10; l > 0; l--, k = ((k + 1) & hplaninfo.ywrap)) {
      m = k * hplaninfo.xt;
      xx = 12;
      for (i = xplan, j = 6; j > 0; j--, i = ((i + 1) & hplaninfo.xwrap)) {
	if (level_map[i + m].type == t_anim ||
	    ((level_map[i + m].type == t_speed ||
	      level_map[i + m].type == t_boom ||
	      level_map[i + m].type == t_stop ||
	      level_map[i + m].type == t_ice ||
	      level_map[i + m].type == t_outway ||
	      level_map[i + m].type == t_dust)
	     && (level_map[i + m].info.param[4] & 0xf0) != 0)) {
	  if (level_map[i + m].type == t_anim)
	    copy_tile (level_map[i + m].number +
		       24 * ((t / (level_map[i + m].info.anim.speed + 1)) %
			     (level_map[i + m].info.anim.frame_nbr + 1)),
		       dest);
	  else {
	    tmp =
	      ((t / ((level_map[i + m].info.param[4] & 15) + 1)) %
	       ((level_map[i + m].info.param[4] >> 4) * 2));
	    if (tmp >
		((unsigned int) level_map[i + m].
		 info.param[4] >> 4))tmp =
		((level_map[i + m].info.param[4] >> 4) * 2) - tmp;
	    copy_tile (level_map[i + m].number + 24 * tmp, dest);
	  }
	  if (sprhide == 0) {
	    for (n = 0; n < 4; n++)
	      if (i + m == hplaninfo.start[n])
		copy_square_transp (heditrsc.buffer + (16 + (n << 4)) * 320 +
				    256 + (hplaninfo.start_way[n] & 0xf0),
				    dest +
				    square_offset[hplaninfo.start_way[n]
						 & 0xf]);
	    if (level_map[i + m].sprite != 0)
	      copy_tile_transp (level_map[i + m].sprite, dest);
	  }
	  if (draw_collide_tests)
	    for (n = 0; n < 4; n++)
	      copy_square_transp (heditrsc.buffer + 10 * 320 + 30 +
				  level_map[i + m].collision[n] * 12,
				  dest + square_offset[n]);
	  if (i + m == tempd)
	    frame_dashed (xx - 12, yy - 7, 24, 20, 8, 15);
	}
	dest += 24;
	xx += 24;
      }
      dest += 20 * xbuf - 24 * 6;
      yy += 20;
    }
    t++;
  }
  while (mouse12 () == 0 && key_ready () == 0);

  update_left_panel ();
  update_middle_panel ();
}

static void
gestclav (a_keycode i, a_keycode mod)
{
  char t;
  int j, k;
  if (i == HK_Home) {
    if (mod & HK_MOD_Ctrl) {
      xdalles = 0;
      update_middle_panel ();
    }
  } else if (i == HK_End) {
    if (mod & HK_MOD_Ctrl) {
      xdalles = (tile_set_img.width / 24) * 24 - 144;
      update_middle_panel ();
    }
  } else if (i == HK_PageDown) {
    if (mod & HK_MOD_Ctrl) {
      if (ydalles < 180) {
	ydalles += 20;
	update_middle_panel ();
      } else if (xdallesdec < 120) {
	xdallesdec += 24;
	update_middle_panel ();
      }
    }
  } else if (i == HK_PageUp) {
    if (mod & HK_MOD_Ctrl) {
      if (ydalles > 0) {
	ydalles -= 20;
	update_middle_panel ();
      } else if (xdallesdec > 0) {
	xdallesdec -= 24;
	update_middle_panel ();
      }
    }
  } else if (i == HK_Right) {
    if (mod & HK_MOD_Ctrl) {
      if (xdalles + 168U < tile_set_img.width) {
	xdalles += 24;
	update_middle_panel ();
      } else if (xdallesdec < 120) {
	xdallesdec += 24;
	update_middle_panel ();
      }
    } else if (mod & HK_MOD_Shift) {
      if (xplandec < 120)
	xplandec += 24;
      else if (xplan < (hplaninfo.xt - 6) || hplaninfo.xwrap != DONT_WRAP) {
	xplan = ((xplan + 1) & hplaninfo.xwrap);
      }
      update_left_panel ();
    } else {
      if (xplan < (hplaninfo.xt - 6) || hplaninfo.xwrap != DONT_WRAP) {
	xplan = ((xplan + 1) & hplaninfo.xwrap);
	update_left_panel ();
      } else
	gestclav (HK_Right, HK_MOD_Shift);
    }
  } else if (i == HK_Left) {
    if (mod & HK_MOD_Ctrl) {
      if (xdalles > 0) {
	xdalles -= 24;
	update_middle_panel ();
      } else if (xdallesdec > 0) {
	xdallesdec -= 24;
	update_middle_panel ();
      }
    } else if (mod & HK_MOD_Shift) {
      if (xplandec > 0)
	xplandec -= 24;
      else if (xplan > 0 || hplaninfo.xwrap != DONT_WRAP) {
	xplan = ((xplan - 1) & hplaninfo.xwrap);
      }
      update_left_panel ();
    } else {
      if (xplan > 0 || hplaninfo.xwrap != DONT_WRAP) {
	xplan = ((xplan - 1) & hplaninfo.xwrap);
	update_left_panel ();
      } else
	gestclav (HK_Left, HK_MOD_Shift);
    }
  } else if (i == HK_Down) {
    if (mod & HK_MOD_Ctrl) {
      gestclav (HK_PageDown, HK_MOD_Ctrl);
    } else if (mod & HK_MOD_Shift) {
      if (yplandec < 180)
	yplandec += 20;
      else if (yplan < (hplaninfo.yt - 10) || hplaninfo.ywrap != DONT_WRAP) {
	yplan = ((yplan + 1) & hplaninfo.ywrap);
      }
      update_left_panel ();
    } else {
      if (yplan < (hplaninfo.yt - 10) || hplaninfo.ywrap != DONT_WRAP) {
	yplan = ((yplan + 1) & hplaninfo.ywrap);
	update_left_panel ();
      } else
	gestclav (HK_Down, HK_MOD_Shift);
    }
  } else if (i == HK_Up) {
    if (mod & HK_MOD_Ctrl) {
      gestclav (HK_PageUp, HK_MOD_Ctrl);
    } else if (mod & HK_MOD_Shift) {
      if (yplandec > 0)
	yplandec -= 20;
      else if (yplan > 0 || hplaninfo.ywrap != DONT_WRAP) {
	yplan = ((yplan - 1) & hplaninfo.ywrap);
      }
      update_left_panel ();
    } else {
      if (yplan > 0 || hplaninfo.ywrap != DONT_WRAP) {
	yplan = ((yplan - 1) & hplaninfo.ywrap);
	update_left_panel ();
      } else
	gestclav (HK_Up, HK_MOD_Shift);
    }
  } else if (i == HK_Enter) {
    if (mod & HK_MOD_Ctrl) {
      display_level_map_animated ();
    } else {
      display_level_map_fullscreen ();
    }
  } else if (i == HK_Space) {
    j = curdallep ();
    level_map[j].number =
      xdalles + xdallesdec + ydalles * (tile_set_img.width);
    level_map[j].type = ddef[curdalled ()].type;
    level_map[j].info = ddef[curdalled ()].info;
    gestclav (HK_i, HK_MOD_None);
    gestclav (HK_O, HK_MOD_None);
    update_left_panel ();
  } else if (i == HK_I || i ==HK_i) {
    for (j = hplaninfo.xt * hplaninfo.yt - 1; j >= 0; j--) {
      level_map[j].collision[0] = 0;
      level_map[j].collision[1] = 0;
      level_map[j].collision[2] = 0;
      level_map[j].collision[3] = 0;
    }
    for (j = hplaninfo.xt * hplaninfo.yt - 1; j >= 0; j--) {
      k = (((level_map[j].number % tile_set_img.width) / 24) +
	   (level_map[j].number / (tile_set_img.width * 20)) *
	   (tile_set_img.width / 24));
      level_map[j].type = ddef[k].type;
      if (level_map[j].type != t_tunnel) {
	if (level_map[j].type == t_anim || i == 0x1749)
	  level_map[j].info = ddef[k].info;
	else {
	  level_map[j].info.param[0] = ddef[k].info.param[0];
	  level_map[j].info.param[1] = ddef[k].info.param[1];
	  level_map[j].info.param[2] = ddef[k].info.param[2];
	  level_map[j].info.param[3] = ddef[k].info.param[3];
	}
      } else
	level_map[j].info.tunnel.direction = ddef[k].info.tunnel.direction;
      level_map[j].collision[0] &= (c_left | c_up);
      level_map[j].collision[1] &= (c_right | c_up);
      level_map[j].collision[2] &= (c_left | c_down);
      level_map[j].collision[3] &= (c_right | c_down);
      level_map[j].collision[0] |= ddef[k].collision[0];
      level_map[j].collision[1] |= ddef[k].collision[1];
      level_map[j].collision[2] |= ddef[k].collision[2];
      level_map[j].collision[3] |= ddef[k].collision[3];
      if (curdallepg (j) >= 0) {
	if (ddef[k].collision[0] & c_left)
	  level_map[curdallepg (j)].collision[1] |= c_right;
	if (ddef[k].collision[2] & c_left)
	  level_map[curdallepg (j)].collision[3] |= c_right;
      }
      if (curdalleph (j) >= 0) {
	if (ddef[k].collision[0] & c_up)
	  level_map[curdalleph (j)].collision[2] |= c_down;
	if (ddef[k].collision[1] & c_up)
	  level_map[curdalleph (j)].collision[3] |= c_down;
      }
      if (curdallepd (j) >= 0) {
	if (ddef[k].collision[1] & c_right)
	  level_map[curdallepd (j)].collision[0] |= c_left;
	if (ddef[k].collision[3] & c_right)
	  level_map[curdallepd (j)].collision[2] |= c_left;
      }
      if (curdallepb (j) >= 0) {
	if (ddef[k].collision[2] & c_down)
	  level_map[curdallepb (j)].collision[0] |= c_up;
	if (ddef[k].collision[3] & c_down)
	  level_map[curdallepb (j)].collision[1] |= c_up;
      }
    }
    update_left_panel();
  } else if (i == HK_f || i == HK_F) {
    if (mod & HK_MOD_Ctrl) {
      for (j = hplaninfo.xt * hplaninfo.yt - 1; j >= 0; j--) {
	level_map[j].number = xdalles + xdallesdec +
	  ydalles * (tile_set_img.width);
	level_map[j].type = ddef[curdalled ()].type;
	level_map[j].info = ddef[curdalled ()].info;
      }
      update_left_panel ();
    } else if (mod & (HK_MOD_Alt | HK_MOD_Meta)) {
      for (j = hplaninfo.xt * hplaninfo.yt - 1; j >= 0; j--)

	level_map[j].number =
	  (((j % hplaninfo.xt) + (j / hplaninfo.xt)) & 1) * 20 *
	  tile_set_img.width;
      update_left_panel ();
    }
  } else if (i == HK_F3) {
    sprhide ^= 1;
    update_left_panel ();
  } else if (i == HK_F6) {
    draw_collide_tests ^= 1;
    update_left_panel ();
  } else if (i == HK_s || i == HK_S) {
    if (level_map[curdallep ()].sprite == 0)

      level_map[curdallep ()].sprite =
	(short int) (xdalles + xdallesdec + ydalles * (tile_set_img.width));
    else
      level_map[curdallep ()].sprite = 0;
    update_left_panel ();
  } else if (i == HK_d || i == HK_D) {
    t = level_map[curdallep ()].type;
    if (t != t_boom && t != t_anim && t != t_outway)
      departfix ();
  } else if (i == HK_t || i == HK_T) {
    if (tempd == curdallep ())
      tempd = DONT_WRAP;
    else
      tempd = curdallep ();
    update_left_panel ();
  } else if (i == HK_o || i == HK_O) {
    outwayflag ();
    update_left_panel();
  } else if (i == HK_p || i == HK_P) {
    save_level_as_pcx ();
    update_left_panel ();
  } else if (i == HK_0) {
    i = 0;
    goto handle_numbers;
  } else if (i == HK_1) {
    i = 1;
    goto handle_numbers;
  } else if (i == HK_2) {
    i = 2;
    goto handle_numbers;
  } else if (i == HK_3) {
    i = 3;
    goto handle_numbers;
  } else if (i == HK_4) {
    i = 4;
    goto handle_numbers;
  } else if (i == HK_5) {
    i = 5;
    goto handle_numbers;
  } else if (i == HK_6) {
    i = 6;
    goto handle_numbers;
  } else if (i == HK_7) {
    i = 7;
    goto handle_numbers;
  } else if (i == HK_8) {
    i = 8;
  handle_numbers:
    level_map[curdallep ()].type = i;
    level_map[curdallep ()].info.tunnel.output = 0;
    level_map[curdallep ()].info.param[4] = 0;
    update_left_panel ();
  } else {
    /* Unknown key.  */
  }
}

static void
gestsrs1 (void)
{
  int x = mouse_coord_x (), y = mouse_coord_y (), i, x2, y2;
  i = curdallep ();

  if (x >= 290) {
    if (y < 19)
      gestclav (HK_Enter, HK_MOD_None);
    if (y >= 144 && y <= 166) {
      if (x > 305)
	gestclav (HK_Right, HK_MOD_Ctrl);
      else
	gestclav (HK_Left, HK_MOD_Ctrl);
    }
    if (y >= 86 && y <= 137) {
      (*fnptr[level_map[i].type]) (i, x, y);
      update_left_panel ();
    }
    if (y >= 168) {
      if ((level_map[i].type == t_speed ||
	   level_map[i].type == t_ice ||
	   level_map[i].type == t_stop ||
	   level_map[i].type == t_dust ||
	   level_map[i].type == t_outway || level_map[i].type == t_boom)) {
	anim_mod_bcl (i, x, y);
	update_left_panel ();
      }
    }
  }

  if (x < 144) {
    xplandec = (x / 24) * 24;
    yplandec = (y / 20) * 20;
    update_left_panel ();
    do {
      x2 = mouse_coord_x ();
      y2 = mouse_coord_y ();
      if (x - x2 > 3) {
	x = x2;
	gestclav (HK_Right, HK_MOD_None);
      } else if (x2 - x > 3) {
	x = x2;
	gestclav (HK_Left, HK_MOD_None);
      }
      if (y - y2 > 3) {
	y = y2;
	gestclav (HK_Down, HK_MOD_None);
      } else if (y2 - y > 3) {
	y = y2;
	gestclav (HK_Up, HK_MOD_None);
      }
    } while (mouse12 () != 0);
  }
  if (x > 144 && x < 289) {
    xdallesdec = ((x - 145) / 24) * 24;
    ydalles = (y / 20) * 20;
    update_middle_panel ();
    do {
      x2 = mouse_coord_x ();
      y2 = mouse_coord_y ();
      if (x - x2 > 3) {
	x = x2;
	gestclav (HK_Right, HK_MOD_Ctrl);
      } else if (x2 - x > 3) {
	x = x2;
	gestclav (HK_Left, HK_MOD_Ctrl);
      }
      if (y - y2 > 3) {
	y = y2;
	gestclav (HK_PageDown, HK_MOD_Ctrl);
      } else if (y2 - y > 3) {
	y = y2;
	gestclav (HK_PageUp, HK_MOD_Ctrl);
      }
    } while (mouse12 () != 0);
  }

}

static void
gestsrs2 (void)
{
  int x = mouse_coord_x (), y = mouse_coord_y ();

  if (x >= 290) {
    if (y >= 144 && y <= 166) {
      if (x > 305)
	gestclav (HK_End, HK_MOD_Ctrl);
      else
	gestclav (HK_Home, HK_MOD_Ctrl);
    }
  }
  if (x < 144) {
    xplandec = (x / 24) * 24;
    yplandec = (y / 20) * 20;
    gestclav (HK_Space, HK_MOD_None);
  }
}

static void
gestsrs3 (void)
{
  int x = mouse_coord_x (), y = mouse_coord_y ();
  int a, b;

  if (x < 144) {
    a = xplandec;
    b = yplandec;
    xplandec = (x / 24) * 24;
    yplandec = (y / 20) * 20;
    gestclav (HK_t, HK_MOD_None);
    xplandec = a;
    yplandec = b;
    update_left_panel ();
  }
}

char* levels_output_dir = 0;

int
create_levels_output_dir (void)
{
  int error;

  if (!levels_output_dir) {
    levels_output_dir = get_non_null_rsc_file ("hedlite-output-dir");
    strip_trailing_slashes (levels_output_dir);
  }

  error = exists_dir (levels_output_dir);
  if (error < 0)
    return 1;
  if (error == 0)
    if (mkdir (levels_output_dir, 0755)) {
      dperror (levels_output_dir);
      wmsg (_("Cannot create directory `%s'."), levels_output_dir);
      return 1;
    }
  return 0;
}

void
free_levels_output_dir (void)
{
  dmsg (D_MISC, "free levels output dir");
  XFREE0 (levels_output_dir);
}

int
hmain (const char* lname, const char* tset_name,
       a_u32 xsize, a_u32 ysize, a_u32 xwrap, a_u32 ywrap)
{
  char* lvl_name;

  if (create_levels_output_dir ())
    return 1;

  tile_set_name = get_non_null_rsc_file ("tiles-sets-dir");
  dallepie = get_non_null_rsc_file ("tiles-sets-dir");

  xdalles = 0;
  ydalles = 0;
  xdallesdec = 0;
  xplan = 0;
  yplan = 0;
  tempd = DONT_WRAP;
  xplandec = 0;
  yplandec = 0;
  sprhide = 0;
  draw_collide_tests = 0;

  strcpy (levelnomshort, lname);
  strlwr (levelnomshort);
  if (tset_name) {
    strcat (strcpy (hplaninfo.tile_set_name, "level"), tset_name);
    strcat (strcpy (hplaninfo.soundtrack_name, "heroes"), tset_name);
  }
  hplaninfo.xt = xsize;
  hplaninfo.yt = ysize;
  hplaninfo.xwrap = xwrap;
  hplaninfo.ywrap = ywrap;

  strcat (strcpy (pcxnom, levelnomshort), ".pcx");

  XMALLOC_ARRAY (lvl_name, (strlen (levels_output_dir) + 1
			    + strlen (levelnomshort) + 4 + 1));
  sprintf (lvl_name, "%s/%s.lvl", levels_output_dir, levelnomshort);

  /* initialize level_map */

  if (!((ftmp = fopen (lvl_name, "rb")) == NULL)) {
    if (fread (&hplaninfo, sizeof (a_level_header), 1, ftmp) != 1)
      emsg (_("%s: invalid level file"), lvl_name);
    /* convert hplaninfo to local endianess */
    bswap_level_header (&hplaninfo);

    XMALLOC_ARRAY (level_map, hplaninfo.xt * hplaninfo.yt);
    if (fread (level_map, sizeof (a_tile), hplaninfo.xt * hplaninfo.yt, ftmp)
	!= (hplaninfo.xt * hplaninfo.yt))
      emsg (_("%s: invalid level file"), lvl_name);
    fclose (ftmp);
    /* convert level_map to local endianess */
    bswap_level_tiles (&hplaninfo, level_map);
  } else {
    XCALLOC_ARRAY (level_map, hplaninfo.xt * hplaninfo.yt);
    hplaninfo.start[0] = 0;
    hplaninfo.start[1] = 0;
    hplaninfo.start[2] = 0;
    hplaninfo.start[3] = 0;
    hplaninfo.start_way[0] = 0x00;
    hplaninfo.start_way[1] = 0x11;
    hplaninfo.start_way[2] = 0x32;
    hplaninfo.start_way[3] = 0x23;
  }

  strlwr (hplaninfo.tile_set_name);

  tile_set_name = strappend (strappend (tile_set_name,
					hplaninfo.tile_set_name), ".pcx");
  dallepie = strappend (strappend (dallepie,
				   hplaninfo.tile_set_name), ".pie");

  pcx_load_from_rsc ("editor-img", &heditrsc);
  pcx_load (tile_set_name, &tile_set_img);

  /* read the tileset definition */
  XCALLOC_ARRAY (ddef, (tile_set_img.width / 24) * 10);
  if (!((ftmp = fopen (dallepie, "rb")) == NULL))
    fread (ddef, sizeof (a_tile_info), (tile_set_img.width / 24) * 10, ftmp);
  fclose (ftmp);

  outwayinit ();
  XCALLOC_ARRAY (hedit_buffer, xbuf * 200);
  set_pal (tile_set_img.palette.global, 0, 256 * 3);
  rectangle_copy (0, 0, 30, 200, 290, 0, &heditrsc);
  strupr (levelnomshort);
  draw_text (levelnomshort, 305, 29, 8, 1);
  sprintf (nombre, "%u", hplaninfo.xt);
  draw_text (nombre, 302, 43, 8, 2);
  sprintf (nombre, "%u", hplaninfo.yt);
  draw_text (nombre, 307, 43, 8, 0);

  update_middle_panel ();
  update_left_panel ();
  while (key_ready ())
    get_key ();
  mouse_show ();

  {
    a_keycode k = 0;
    do {
      while (key_ready () == 0 && mouse12 () == 0);
      if (key_ready ()) {
	k = get_key ();
	gestclav (k, keyboard_modifiers);
      } else {
	if (mouse1 ()) {
	  gestsrs1 ();
	} else if (mouse2 ()) {
	  gestsrs2 ();
	} else if (mouse3 ()) {
	  gestsrs3 ();
	}
	while (mouse12 () != 0);
      }
    } while (k != HK_Escape);
  }
  mouse_hide ();
  outwayclose ();
  free (hedit_buffer);
  {
    if (!((ftmp = fopen (lvl_name, "wb")) == NULL)) {
      /* convert hplaninfo to disk endianess */
      bswap_level_header (&hplaninfo);
      fwrite (&hplaninfo, sizeof (a_level_header), 1, ftmp);
      /* convert level_map to file endianess */
      bswap_level_tiles (&hplaninfo, level_map);
      fwrite (level_map, sizeof (a_tile), hplaninfo.xt * hplaninfo.yt, ftmp);
      fclose (ftmp);
    }
    free (ddef);
    free (level_map);
    img_free (&heditrsc);
    img_free (&tile_set_img);
  }

  free (dallepie);
  free (tile_set_name);
  free (lvl_name);
  return 0;
}
