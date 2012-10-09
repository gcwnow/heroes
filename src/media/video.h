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

#ifndef HEROES__VIDEO__H
#define HEROES__VIDEO__H

/* the three macro below configure the rendering buffers used in the game */
#define xbuf 512		/* large width to suppress any clipping need,
				   power of two of easier multiplications */
#define ybuf 300		/* height */
#define sbuf (50*xbuf)		/* keep sbuf empty line on top of the buffers
				   (to avoid clipping) */


typedef a_u8 a_pixel;

void set_display_params (const char *str);
void set_full_screen_mode (void);

void init_video (void);
void uninit_video (void);

void set_pal_entry (unsigned char c,
		    unsigned char r, unsigned char g, unsigned char b);
void set_pal (const unsigned char *ptr, int p, int n);

/* dump the supplied buffer (assumed xbuf-wide) to the screen */
void vsynchro (const a_pixel *s);
/* likewise, but construct the screen from two buffers (used
   in two player mode). */
void vsynchro2 (const a_pixel *s1, const a_pixel *s2);

#endif /* HEROES__VIDEO__H */
