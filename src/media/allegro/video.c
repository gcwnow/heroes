/*------------------------------------------------------------------.
| Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
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
#include <allegro.h>
#include "video_low.h"
#include "debugmsg.h"
#include "errors.h"
#include "argv.h"

static int scr_w, scr_h;	/* rendering buffer width and height */
static int scr_stretch;

BITMAP *screen_rv;	/* Rendering buffer.  */

void
set_display_params (const char *str)
{
  (void) str;
}

void
set_full_screen_mode (void)
{
}

static void
init_allegro (void)
{
  dmsg (D_SYSTEM, "initialize allegro");
  allegro_init ();
  install_keyboard ();
  install_mouse ();
}


void heroes_init_allegro (void);
void
heroes_init_allegro (void)
{
  static bool done = false;
  if (done)
    return;
  done = true;
  init_allegro ();
}

void
init_video_low (int stretch_, int *pitch)
{
  int gfxret;

  heroes_init_allegro ();

  scr_stretch = stretch_;
  scr_w = 320 * scr_stretch;
  scr_h = 200 * scr_stretch;

  dmsg (D_VIDEO, "setup video mode");

  set_color_depth (8);
  gfxret = set_gfx_mode (GFX_AUTODETECT, scr_w, scr_h, 0, 0);

  if (gfxret < 0)
    emsg ("failed to setup video mode\n%s", allegro_error);

  *pitch = VIRTUAL_W;

  dmsg (D_VIDEO, "video mode is %dx%d",	SCREEN_W, SCREEN_H);

  screen_rv = create_bitmap (SCREEN_W, SCREEN_H);
}

void
uninit_video_low (void)
{
  dmsg (D_VIDEO, "setup text mode");
  set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
  destroy_bitmap (screen_rv);
}

void
set_pal_entry (unsigned char c,
	       unsigned char r, unsigned char g, unsigned char b)
{
  RGB p = { r, g, b, 0 };
  dmsg (D_VIDEO, "set color %d=(%d,%d,%d)", c, r, g, b);
  set_color (c, &p);
}

void
set_pal (const unsigned char *ptr, int p, int n)
{
  PALETTE pal;
  int i;
  for (i = 0; i < 256; ++i) {
    pal[i].r = *ptr++;
    pal[i].g = *ptr++;
    pal[i].b = *ptr++;
  }
  dmsg (D_VIDEO, "set %d colors (%d - %d)", n/3, p/3, p/3 + n/3 - 1);
  set_palette_range (pal, p/3, p/3 + n/3 - 1, 0);
}

void
vsynchro_low (const a_pixel *s, a_copy_function f)
{
  acquire_screen ();
  bmp_select (screen_rv);
  f (s, screen_rv->line[0], 320);
  vsync ();
  blit (screen_rv, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
  release_screen ();
}

void
vsynchro2_low (const a_pixel *s1, const a_pixel *s2, a_copy_function f)
{
  acquire_screen ();
  bmp_select (screen_rv);
  f (s1, screen_rv->line[0], 160);
  f (s2, screen_rv->line[0] + 160 * scr_stretch, 160);
  vsync ();
  blit (screen_rv, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
  release_screen ();
}
