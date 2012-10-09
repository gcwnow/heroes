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

#ifndef HEROES__SPRGLENZ__H
#define HEROES__SPRGLENZ__H

/*------------------------------------------------------------------.
| transparent sprites whose non-transparent pixel are drawn using a |
| glenz line.  This is used for trails, amongst other things.       |
`------------------------------------------------------------------*/

#include "sprrle.h"

void draw_sprglenz (const a_sprite *sprite, a_pixel *dest);

/* Same as above, but allow to choose the glenz line used */
void draw_sprglenz_custom (const a_sprite *sprite, a_pixel *dest,
			   const a_pixel *glenz_line);

a_sprite *compile_sprglenz (const a_pixel *src, a_pixel transp_color,
			    a_pixel *glenz_line,
			    unsigned int block_height,
			    unsigned int block_width,
			    unsigned int src_width, unsigned int dest_width);

void free_sprglenz (a_sprite *prog);

#endif /* HEROES__SPRGLENZ__H */
