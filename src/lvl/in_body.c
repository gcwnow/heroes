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
#include "lvl_priv.h"

void
initialize_level_body (a_level *lvl)
{
  a_square_coord y, x;
  a_square_index idx;

  /* Allocate various buffer.  */
  XCALLOC_ARRAY (lvl->private->tile, lvl->tile_count);
  XCALLOC_ARRAY (lvl->square_type, lvl->square_count);
  XCALLOC_ARRAY (lvl->square_walls_out, lvl->square_count);
  XMALLOC_ARRAY (lvl->square_direction, lvl->square_count);
  XMALLOC_ARRAY (lvl->square_move[D_UP], lvl->square_count);
  XMALLOC_ARRAY (lvl->square_move[D_RIGHT], lvl->square_count);
  XMALLOC_ARRAY (lvl->square_move[D_DOWN], lvl->square_count);
  XMALLOC_ARRAY (lvl->square_move[D_LEFT], lvl->square_count);

  /* Assume any square is connected to its neighbors.  */
  for (y = 0, idx = 0; y < lvl->square_height; ++y)
    for (x = 0; x < lvl->square_width; ++x, ++idx) {
      lvl->square_move[D_UP][idx] =
	SQR_COORDS_TO_INDEX (lvl, SQR_COORD_UP (lvl, y), x);
      lvl->square_move[D_RIGHT][idx] =
	SQR_COORDS_TO_INDEX (lvl, y, SQR_COORD_RIGHT (lvl, x));
      lvl->square_move[D_DOWN][idx] =
	SQR_COORDS_TO_INDEX (lvl, SQR_COORD_DOWN (lvl, y), x);
      lvl->square_move[D_LEFT][idx] =
	SQR_COORDS_TO_INDEX (lvl, y, SQR_COORD_LEFT (lvl, x));
    }

  /* Prevent anybody to run out of the map by marking outgoing
     directions as invalid.  Of course this makes sense only for non
     wrapped levels.  */

  if (lvl->square_width_wrap == DONT_WRAP)
    for (y = 0; y < lvl->square_height; ++y) {
      lvl->square_move[D_LEFT][SQR_COORDS_TO_INDEX (lvl, y, 0)]
	= INVALID_INDEX;
      lvl->square_move[D_RIGHT][SQR_COORDS_TO_INDEX (lvl, y,
						     lvl->square_width - 1)]
	= INVALID_INDEX;
    }

  if (lvl->square_height_wrap == DONT_WRAP)
    for (x = 0; x < lvl->square_width; ++x) {
      lvl->square_move[D_UP][SQR_COORDS_TO_INDEX (lvl, 0, x)]
	= INVALID_INDEX;
      lvl->square_move[D_DOWN][SQR_COORDS_TO_INDEX (lvl,
						    lvl->square_height - 1, x)]
	= INVALID_INDEX;
    }
}

void
initialize_empty_level_body (a_level *lvl)
{
  lvl->private->tile = 0;
  lvl->square_type = 0;
  lvl->square_walls_out = 0;
  lvl->square_direction = 0;
  lvl->square_move[D_UP] = 0;
  lvl->square_move[D_RIGHT] = 0;
  lvl->square_move[D_DOWN] = 0;
  lvl->square_move[D_LEFT] = 0;
}
