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

#ifndef HEROES__FORMAT__H
#define HEROES__FORMAT__H

#include "access.h"

/* The body is a succession of tile_width*tile_height records.
   One for each tile.

   Structure of a record:

   Offset  Size  Repeat  What
   --------------------------------------------------------------------
    0      4             sprite offset of the tile in the tile sprite map
    4      1     4       walls for each sub square: if D_UP is set, you
                         cannot *enter* the square from the *bottom* edge.
                         Note this is not the expected content of
                         square_walls_out.  Actually if these bytes
                         set the bit D_LEFT for the square (Y,X), then
			   the same bit should be set in square_walls_out
			   for the square (Y,X+1) (i.e. the square on the
			   right).
    8      2             sprite offset of the overlay in the tile sprite map,
                         unless nul
   10      5             parameters (see interpretation below)
   15      1             tile type
   ==
   16 bytes.
*/
#define GET_TILE_WALLS(p) ((const a_u8 *) ((p) + 4))
#define SET_TILE_WALLS(p, idx, val) SET_U8 ((p) + 4 + (idx), (val))

#define GET_TILE_SPRITE(p) ((a_u32) (GET_U32 ((p) + 0)))
#define SET_TILE_SPRITE(p, val) SET_U32 ((p) + 0, (val))

#define GET_TILE_OVERLAY(p) ((a_u16) (GET_U16 ((p) + 8)))
#define SET_TILE_OVERLAY(p, val) SET_U16 ((p) + 8, (val))

#define GET_TILE_TYPE(p) ((a_tile_type) (GET_U8 ((p) + 15)))
#define SET_TILE_TYPE(p, val) SET_U8 ((p) + 15, (val))

/* Parameters are used differently for each type of tile.

   Tunnels:

   Offset  Size  What
   ------------------------
    0      4     output tile index
    4      1     direction (4 lower bits), delay (4 upper bits)
                 delay was never used with tunnels.  The original
                 purpose was to delay the player underground (for the time
                 given by delay, computed by the level editor from the
                 length of the tunnel).  Its has been abandoned because
                 handling of vehicles "out of the map" would complexify
                 the game internals.
   ==
    5 bytes.
*/
#define GET_TUNNEL_OUTPUT(p) ((a_tile_index) (GET_U32 ((p) + 10)))
#define SET_TUNNEL_OUTPUT(p, val) SET_U32 ((p) + 10, (val))

#define GET_TUNNEL_DIR(p) (dir_mask_to_dir ((a_dir_mask) \
                                            ((GET_U8 ((p) + 14)) & 15)))
#define SET_TUNNEL_DIR(p, val) SET_U8((p) + 14, (val))

/* Animations:

   Offset  Size  What
   ------------------------
    0      1     frame count
    1      1     delay between each frame (in 70th of sec.)
                 0 is 1/70s, 1 is 2/70s, etc.
    2      3     unused
   ==
    5 bytes.
*/
#define GET_ANIM_FRAME_COUNT(p) (GET_U8 ((p) + 10))
#define SET_ANIM_FRAME_COUNT(p, val) SET_U8 ((p) + 10, (val))

#define GET_ANIM_FRAME_DELAY(p) (GET_U8 ((p) + 11) + 1)
#define SET_ANIM_FRAME_DELAY(p, val) SET_U8 ((p) + 11, (val))

/* Stops, Booms, Ices, Dusts, Outway:

   Offset  Size  Repeat  What
   ---------------------------------
    0      1     4       For each square, zero means no effect, non-zero
                         means effect (stop, boom, ice, dust).
    4      1     1       animation: frame count (upper 4 bits), and
                         delay (lower 4 bits, where 0 means 1 and 1 is 16).
   ==
    5 bytes.
*/
#define GET_EFFECT_SET(p) ((const a_u8*) ((p) + 10))
#define SET_EFFECT_SET(p, idx, val) SET_U8 ((p) + 10 + (idx), (val))

#define GET_SANIM_FRAME_COUNT(p) ((GET_U8 ((p) + 14)) >> 4)
#define GET_SANIM_FRAME_DELAY(p) (((GET_U8 ((p) + 14)) & 15) + 1)
#define SET_SANIM_FRAME_COUNT_DELAY(p, val) SET_U8 ((p) + 14, (val))

/* Speeds:

   Offset  Size  Repeat  What
   ---------------------------------
    0      1     4       For each square, the direction*s* of the effect.
    4      1     1       animation: frame count (upper 4 bits), and
                         delay (lower 4 bits, where 0 means 1 and 1 is 16).
   ==
    5 bytes.
*/
#define GET_SPEED_DIR(p) ((const a_dir_mask8 *) ((p) + 10))
#define SPEED_DIR(p) ((a_u8 *)((p) + 10))

#endif /* HEROES__FORMAT__H */
