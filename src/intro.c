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

/* the intro animation */

#include "system.h"
#include "video.h"
#include "pcx.h"
#include "timer.h"
#include "fastmem.h"

#include "const.h"
#include "strack.h"

#include "intro.h"
#include "debugmsg.h"
#include "fader.h"

#include "scrtools.h"

/******* data of the intro *******/
a_palette fade_pal;
a_pcx_image intro_img;
a_pixel **erase_data;
a_pixel **erase_data_cur;
int color_nbr[256 + 1];
a_pixel **(erase_color_ptr[256]);
int errori;
a_timer intro_frame_htimer;
a_timer intro_global_htimer;
static a_pixel *intro_buffer;
/********************************/


static void
copy_vehicle_1 (int x)
{
  int y;
  int dx = 248;
  if (x >= 320)
    dx -= x - 320;
  if (x < 248) {
    dx = x;
    for (y = 31; y >= 0; y--)
      memcpy (intro_buffer + (58 + y) * xbuf,
	      intro_img.buffer + (58 + y) * 320 + 306 - dx, dx);
  } else
    for (y = 31; y >= 0; y--)
      memcpy (intro_buffer + (58 + y) * xbuf + x - 248,
	      intro_img.buffer + (58 + y) * 320 + 58, dx);
}

static void
copy_vehicle_2 (int x)
{
  int y;
  int dx = 248;
  if (x >= 320)
    dx -= x - 320;
  if (x < 248) {
    dx = x;
    for (y = 31; y >= 0; y--)
      memcpy (intro_buffer + (110 + y) * xbuf,
	      intro_img.buffer + (110 + y) * 320 + 262 - dx, dx);
  } else
    for (y = 31; y >= 0; y--)
      memcpy (intro_buffer + (110 + y) * xbuf + x - 248,
	      intro_img.buffer + (110 + y) * 320 + 14, dx);
}

static void
compute_erase_data (void)
{
  a_pixel *dest = intro_buffer;
  a_pixel *src = intro_img.buffer;
  int i, j;
  for (i = 320 * 200; i > 0; i--)
    color_nbr[*src++]++;
  erase_color_ptr[0] = erase_data;
  for (i = 1; i <= 255; i++)
    erase_color_ptr[i] = erase_color_ptr[i - 1] + color_nbr[i - 1];
  src = intro_img.buffer;
  for (i = 200; i > 0; i--) {
    for (j = 320; j > 0; j--) {
      *erase_color_ptr[*src]++ = dest;
      src++;
      dest++;
    }
    dest += xbuf - 320;
  }
}

static a_pixel **
erase (a_pixel **src, int j)
{
  int nbr = color_nbr[j];
  color_nbr[j + 1] += nbr & 1;
  nbr >>= 1;
  while (nbr) {
    a_pixel *a = src[0];
    a_pixel *b = src[1];
    *a = 0;
    src += 2;
    --nbr;
    *b = 0;
  }
  return src;
}

static void
antialias (a_pixel *src, int height)
{
  do {
    unsigned int a, b, c, d;
    unsigned int width;
    a = src[-1];
    b = src[0];
    for (width = 320 / 2; width; --width) {
      c = src[1];
      ++src;
      a += c;
      d = src[1];
      a >>= 1;
      b += d;
      src[-1] = a;
      b >>= 1;
      a = c;
      src[0] = b;
      b = d;
      ++src;
    }
    src += xbuf - 320;
    --height;
  } while (height);
}

static char
show_intro (void)
{
  a_palette pal;
  a_fader_status fade_stat;
  int i;

  load_soundtrack_from_alias ("INTRO");
  XMALLOC_ARRAY (erase_data, 320 * 200);
  erase_data_cur = erase_data;
  pcx_load_from_rsc ("intro-logos-img", &intro_img);

  play_soundtrack ();
  memset (color_nbr, 0, 256 * sizeof(*color_nbr));
  set_pal_entry (255, 0, 0, 0);
  memset (intro_buffer, 255, xbuf * 100);
  memset (intro_buffer + xbuf * 100, 0, xbuf * 100);
  reset_htimer (intro_frame_htimer);
  reset_htimer (intro_global_htimer);
  for (i = 0; i <= 63; i += read_htimer (intro_frame_htimer)) {
    set_pal_entry (255, i, i, i);
    fade_pal.indiv[255].r = i;
    fade_pal.indiv[255].g = i;
    fade_pal.indiv[255].b = i;
    flush_display (intro_buffer);
    if (key_or_joy_ready ()) {
      img_free (&intro_img);
      return (1);
    }
  }
  for (i = 767; i >= 0; i--)
    fade_pal.global[i] = pal.global[i] = ((i >= 384) ? 63 : 0);

  set_pal (pal.global, 0, 768);
  copy_image_to_scr_area (&intro_img, intro_buffer);
  while (read_htimer (intro_global_htimer) < 2) {
    flush_display (intro_buffer);
    if (key_or_joy_ready ()) {
      img_free (&intro_img);
      return (1);
    }
  }

  /* fade-in to the intro_img */
  std_palette_fade (&pal, &intro_img.palette);
  fader_status_flagback (&fade_stat);
  do {
    flush_display (intro_buffer);
    if (key_or_joy_ready ()) {
      img_free (&intro_img);
      return (1);
    }
  } while (fade_stat != F_FINISHED);

  img_free (&intro_img);
  pcx_load_from_rsc ("intro-vehicles-img", &intro_img);
  while (read_htimer (intro_global_htimer) < 9) {
    flush_display (intro_buffer);
    if (key_or_joy_ready ()) {
      img_free (&intro_img);
      return (1);
    }
  }

  std_palette_fade (&intro_img.palette, &pal);
  fader_status_flagback (&fade_stat);
  fader_delay (64);
  do {
    antialias (intro_buffer + 85 * xbuf, 13);
    antialias (intro_buffer + 103 * xbuf, 13);
    flush_display (intro_buffer);
    if (key_or_joy_ready ()) {
      img_free (&intro_img);
      return (1);
    }
  } while (fade_stat != F_FINISHED);

  memset (intro_buffer, 255, xbuf * 100);
  memset (intro_buffer + xbuf * 100, 0, xbuf * 100);
  set_pal (intro_img.palette.global, 0, 768);

  while (read_htimer (intro_global_htimer) < 14) {
    flush_display (intro_buffer);
    if (key_or_joy_ready ()) {
      img_free (&intro_img);
      return (1);
    }
  }

  /* For this sequence, slices will be 4 times shorter */
  intro_frame_htimer->slice_duration /= 4;
  reset_htimer (intro_frame_htimer);
  for (i = 0; i < 568; i += read_htimer (intro_frame_htimer)) {
    copy_vehicle_1 (i);
    copy_vehicle_2 (567 - i);
    flush_display (intro_buffer);
    if (key_or_joy_ready ()) {
      img_free (&intro_img);
      return (1);
    }
  }
  intro_frame_htimer->slice_duration *= 4; /* revert old speed. */

  img_free (&intro_img);
  pcx_load_from_rsc ("intro-splash-img", &intro_img);
  copy_image_to_scr_area (&intro_img, intro_buffer);
  intro_img.palette.indiv[254].r = 0;
  intro_img.palette.indiv[254].g = 0;
  intro_img.palette.indiv[254].b = 0;
  img_free (&intro_img);	/* will free the picture, not the palette */

  std_white_fadein (&intro_img.palette);
  fader_status_flagback (&fade_stat);
  do {
    flush_display (intro_buffer);
    if (key_or_joy_ready ())
      return (1);
  } while (fade_stat != F_FINISHED);

  pcx_load_from_rsc ("intro-erase-img", &intro_img);
  compute_erase_data ();
  img_free (&intro_img);

  while (read_htimer (intro_global_htimer) < 27) {
    flush_display (intro_buffer);
    if (key_or_joy_ready ())
      return (1);
  }

  erase_data_cur = erase_data;
  reset_htimer (intro_frame_htimer);
  for (i = 0; i <= 255; i += read_htimer (intro_frame_htimer)) {
    flush_display (intro_buffer);
    erase_data_cur = erase (erase_data_cur, i);
    if (key_or_joy_ready ())
      return (1);
  }
  return (0);
}

void
play_intro (void)
{
  a_palette pal;
  dmsg (D_SECTION, "-- game introduction --");

  intro_frame_htimer = new_htimer (T_LOCAL|T_BLOCKING, HZ (70));
  intro_global_htimer = new_htimer (T_GLOBAL, HZ (2));
  XMALLOC_ARRAY (intro_buffer, xbuf * 200);
  show_intro ();
  free (intro_buffer);
  memset (pal.global, 0, 768);
  set_pal (pal.global, 0, 768);
  free (erase_data);
  unload_soundtrack ();
  while (key_or_joy_ready ())
    get_key_or_joy ();

  free_htimer (intro_frame_htimer);
  free_htimer (intro_global_htimer);
}
