/*------------------------------------------------------------------.
| Copyright 2002  Alexandre Duret-Lutz <duret_g@epita.fr>           |
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
#include "access.h"

void
encode_level_header (a_u8 *data, const a_level *lvl)
{
  /* Structure of the header:

     Offset  Size  Repeat  What
     --------------------------------------------------------------------
      0      4             width of the level in tiles
      4      4             height of the level in tiles
      8      4             horizontal wrapping AND-mask
     12      4             vertical wrapping AND-mask
     16      4     4       starting tile index for each player
     32      1     4       starting direction (upper 4 bits) and starting
                           square (bottom 4 bits) for each player
     36      9             tile sprite map basename (zero-terminated)
     45      9             sound track alias (zero-terminated)
     54      1     10      unused bytes.
     ==
     64 bytes.
  */
  int player;

  WRITE_U32 (data, lvl->tile_width);
  WRITE_U32 (data, lvl->tile_height);
  WRITE_U32 (data, lvl->tile_width_wrap);
  WRITE_U32 (data, lvl->tile_height_wrap);

  for (player = 0; player < 4; ++player) {
    a_u32 starting_square_index = SQR_COORDS_TO_INDEX (lvl,
      lvl->private->start_pos[player].y, lvl->private->start_pos[player].x);
    WRITE_U32 (data,
      SQR_INDEX_TO_TILE_INDEX (lvl, starting_square_index));
  }
  for (player = 0; player < 4; ++player) {
    a_u8 starting_dir;

    starting_dir = lvl->private->start_dir[player] << 4;
    if (lvl->private->start_pos[player].x & 1)
      starting_dir |= 1;
    if (lvl->private->start_pos[player].y & 1)
      starting_dir |= 2;

    WRITE_U8 (data, starting_dir);
  }

  strncpy (data, lvl->private->tile_sprite_map_basename, 8);
  data[8] = 0;
  data += 9;
  strncpy (data, lvl->private->sound_track_alias, 8);
  data[8] = 0;
  data += 9;

  /* Clear unused bytes so we don't save garbage.  */
  memset (data, 0, 10);
}
