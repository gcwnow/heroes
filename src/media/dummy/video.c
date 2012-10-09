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
#include "video_low.h"

/* This is a dummy driver.  The functions do absolutely nothing.  */

void
set_display_params (const char *str)
{
  (void) str;
}

void
set_full_screen_mode (void)
{
}

void
init_video_low (int stretch, int *pitch)
{
  (void) stretch;
  (void) pitch;
}

void
uninit_video_low (void)
{
}

void
set_pal_entry (unsigned char c,
	       unsigned char r, unsigned char g, unsigned char b)
{
  (void) c;
  (void) r;
  (void) g;
  (void) b;
}

void
set_pal (const unsigned char *ptr, int p, int n)
{
  (void) ptr;
  (void) p;
  (void) n;
}

void
vsynchro_low (const a_pixel *s, a_copy_function f)
{
  (void) s;
  (void) f;
}

void
vsynchro2_low (const a_pixel *s1, const a_pixel *s2, a_copy_function f)
{
  (void) s1;
  (void) s2;
  (void) f;
}
