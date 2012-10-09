/*------------------------------------------------------------------.
| Copyright 2001, 2002  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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

#ifndef HEROES__LVL_PRIV__H
#define HEROES__LVL_PRIV__H

/*-------------------------------------------------------------------.
| NOTE: This is a private header, for use in the level loading code  |
| only.  Clients of the `lvl' module should only rely on the `lvl.h' |
| interface.                                                         |
`-------------------------------------------------------------------*/

/* We don't want to see constant attribute from here.  */
#define LVL_MUTABLE
#include "lvl.h"

typedef struct a_tile_data a_tile_data;
struct a_tile_data {
  a_tile_type type;
  /* Animation data.  */
  unsigned int frame_count;
  unsigned int frame_delay;
  an_anim_kind anim;
  /* Tunnel data.  */
  /* a_dir tunnel_direction;  FIXME: Useless ? */
  /* Sprite data.  */
  unsigned int sprite_offset;
  unsigned int sprite_overlay_offset;
};

/* Number of players handled by level format.  */
#define LVL_PLAYER_COUNT 4

/* Size in bytes of the level header on disk.  */
#define LVL_HEADER_SIZE 64
/* The body of the level is made of tile_count records of
   LVL_RECORD_SIZE bytes.  */
#define LVL_RECORD_SIZE 16

/* The following is declared, but not defined in lvl.h.  */
struct a_level_bits {
  /* Starting position for each player.  */
  a_square_corrd_pair start_pos[LVL_PLAYER_COUNT];
  /* Starting direction for each player.  */
  a_dir start_dir[LVL_PLAYER_COUNT];
  /* Alias for the soundtrack, to be resolved to a true filename by
     someone else.  */
  char *sound_track_alias;
  /* Basename for the tile sprite map, to be expanded as a real
     filename by someone else.  */
  char *tile_sprite_map_basename;

  /* Information for each tile (present only if the body is loaded).  */
  a_tile_data *tile;
};

void decode_level_header (const a_u8 *data, a_level *lvl);
void decode_level_body (const a_u8 *data, a_level *lvl);
void initialize_level_body (a_level *out);
void initialize_empty_level_body (a_level *lvl);
void encode_level_header (a_u8 *data, const a_level *lvl);
void encode_level_body (a_u8 *data, const a_level *lvl);

#endif /* HEROES__LVL_PRIV__H */
