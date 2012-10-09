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
lvl_free (a_level *lvl)
{
  XFREE0 (lvl->private->tile);
  XFREE0 (lvl->private->sound_track_alias);
  XFREE0 (lvl->private->tile_sprite_map_basename);
  XFREE0 (lvl->private);
  XFREE0 (lvl->square_type);
  XFREE0 (lvl->square_walls_out);
  XFREE0 (lvl->square_direction);
  XFREE0 (lvl->square_move[D_UP]);
  XFREE0 (lvl->square_move[D_RIGHT]);
  XFREE0 (lvl->square_move[D_DOWN]);
  XFREE0 (lvl->square_move[D_LEFT]);
}
