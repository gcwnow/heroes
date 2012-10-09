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

#ifndef HEROES__CAMERA__H
#define HEROES__CAMERA__H

#include "video.h"

extern unsigned int corner_x[2]; /* in pixels */
extern unsigned int corner_y[2]; /* idem */
extern a_pixel *(corner[2]);	/* topleft of the camera vision */

extern unsigned int corner_dx[2]; /* coordinate of the first tile */
extern unsigned int corner_dy[2]; /* idem */

extern unsigned int nbr_tiles_cols; /* # of tiles columns to display */
extern unsigned int nbr_tiles_rows; /*        ... rows ... */

/* Update the position of camera for buffer P as if N frames had elapsed.
   This also update the corner* varibles, hence the name.  */
void compute_corner (int p, int n);

/* Compute the (target) position of all cameras according to the
   positions of players.  */
void position_camera (void);

/* Set initial camera position, on level startup.  */
void init_camera (void);

/* This handle the moving background of menu.  The camera follow
   some stupid lisajou curve.  Pos is the position on this curve.  */
void lisajou_camera (int pos);

#endif /* HEROES__CAMERA__H */
