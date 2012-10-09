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
#include "pcx.h"
#include "video.h"
#include "const.h"
#include "keyb.h"
#include "fastmem.h"
#include "endscroll.h"
#include "sprtext.h"
#include "scrtools.h"
#include "timer.h"
#include "heroes.h"
#include "fader.h"
#include "camera.h"

#define XBUF 128
#define YBUF 324
static a_pixel *scroll_buffer;

a_pcx_image dummy_background_img;

/* This is an approximation of sin, using a Lagrange polynomial.
   Just to try. */
#define LPI (1<<14)
#define LS(x) ( (x) * (LPI-(x)) >> 20 )
static signed long int
ls (signed long int x)
{
  x &= (LPI << 1) - 1;
  return (((x & LPI) ? (-LS (x & (LPI - 1))) : LS (x)) * 7 >> 3);
}

static void
copy_background (void)
{
  int i;
  a_pixel *dest = scroll_buffer;
  const a_pixel *src = dummy_background_img.buffer;

  for (i = 108; i != 0; i--) {
    fastmem4 (src, dest, 128 / 4);
    fastmem4 (src, dest + 108 * XBUF, 128 / 4);
    fastmem4 (src, dest + 216 * XBUF, 128 / 4);
    dest += XBUF;
    src += 128;
  }
}

static void
draw_background (int x, int y)
{
  int i;
  a_pixel *dest = corner[0];
  const a_pixel *src = scroll_buffer + x + y * XBUF;

  for (i = 200; i != 0; i--) {
    fastmem4 (src, dest, 320 / 4);
    dest += xbuf;
    src += XBUF;
  }

}

void
dummy_moving_background_render (void)
{
  static int frame = 0;
  draw_background ((XBUF / 2) - 160 + ls (2 * frame / 3),
		   (YBUF / 2) - 100 + ls (frame));
  frame += read_htimer (background_htimer) << 8;
}

void
dummy_moving_background_init (void)
{
  XMALLOC_ARRAY (scroll_buffer, XBUF * YBUF);

  pcx_load_from_rsc ("end-scroller-bg-img", &dummy_background_img);
  copy_background ();
  img_free (&dummy_background_img); /* only free the buffer, not the palette */
}

void
dummy_moving_background_uninit (void)
{
  free (scroll_buffer);
}

void
end_scroll (void)
{
  a_sprite *theend;
  theend = compile_menu_text (_("THE END"), T_CENTERED | T_WAVING, 95, 159);
  corner[0] = render_buffer[0] + 10 * xbuf;

  std_white_fadein (&dummy_background_img.palette);
  do {
    dummy_moving_background_render ();
    DRAW_SPRITE (theend, corner[0]);
    flush_display (corner[0]);
  } while (!key_or_joy_ready ());
  get_key ();
  free_sprite (theend);
}
