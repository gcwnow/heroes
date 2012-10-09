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

#ifndef HEROES__SCRTOOLS__H
#define HEROES__SCRTOOLS__H

#include "pcx.h"

void set_pal_with_luminance (const a_palette* palsrc);
void force_palette (const unsigned char *palette);

void flush_display (const a_pixel *src);
void flush_display2 (const a_pixel *src1, const a_pixel *src2);
void flush_display_moving (int x);
void flush_display2_moving (int x);

void shade_scr_area (const a_pixel *src, a_pixel *dest);
void copy_scr_area (const a_pixel *src, a_pixel *dest);
void copy_image_to_scr_area (const a_pcx_image *src, a_pixel *dest);
void clear_scr_area (a_pixel *dest);

#endif /* HEROES__SCRTOOLS__H */
