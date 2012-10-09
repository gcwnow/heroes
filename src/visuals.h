/*------------------------------------------------------------------------.
| Copyright 1997, 1998, 2000  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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

#ifndef HEROES__VISUALS__H
#define HEROES__VISUALS__H

extern int rotosinus[256];
extern int rotocosinus[256];
extern int rotosinus2[256];
extern int rotocosinus2[256];
extern signed char moyensinus[512];
extern int mulxbuf[200];
extern int angle;
extern a_pixel *srcroto;

void rotozoom_buffer (void);	/* rotozoom */
void rotozoom_half_buffer (int c);
void wave_buffer (void);	/* mushroom */
void wave_half_buffer (int c);
void flip_buffer (int p2);	/* flipping the screen horizontaly */
int corner_buffer (void);	/* "removing" the screen from the corner */

void compute_lut (void);	/* init tables sin/cos */

#endif /* HEROES__VISUALS__H */
