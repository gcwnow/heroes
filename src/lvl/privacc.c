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

const char *
lvl_sound_track (const a_level *lvl)
{
  return lvl->private->sound_track_alias;
}

const char *
lvl_tile_sprite_map_basename (const a_level *lvl)
{
  return lvl->private->tile_sprite_map_basename;
}

void
lvl_start_position (const a_level *lvl, unsigned int player,
		    a_square_corrd_pair *coord, a_dir *dir)
{
  if (coord)
    *coord = lvl->private->start_pos[player];
  if (dir)
    *dir = lvl->private->start_dir[player];
}

a_tile_type
lvl_tile_type (const a_level *lvl, a_tile_index tile)
{
  return lvl->private->tile[tile].type;
}

unsigned int
lvl_tile_sprite_offset (const a_level *lvl, a_tile_index tile)
{
  return lvl->private->tile[tile].sprite_offset;
}

unsigned int
lvl_tile_sprite_overlay_offset (const a_level *lvl, a_tile_index tile)
{
  return lvl->private->tile[tile].sprite_overlay_offset;
}

void
lvl_animation_info (const a_level *lvl, a_tile_index tile,
		    unsigned int *frame_count, unsigned int *delay,
		    an_anim_kind *kind)
{
  *kind = lvl->private->tile[tile].anim;
  *frame_count = lvl->private->tile[tile].frame_count;
  *delay = lvl->private->tile[tile].frame_delay;
}
