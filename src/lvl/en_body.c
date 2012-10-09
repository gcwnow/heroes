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
#include "format.h"

/* Reverse the walls: Check all neighbour squares if it's allowed to enter
   the current square from there. */
static void
en_reverse_walls (const a_level *lvl, a_dir_mask8 *square_walls_in)
{
  a_square_index idx;

  for (idx = 0; idx < lvl->square_count; ++idx) {
    a_square_coord this_x = SQR_INDEX_TO_COORD_X (lvl, idx);
    a_square_coord this_y = SQR_INDEX_TO_COORD_Y (lvl, idx);
    {
      a_square_coord dest_y = SQR_COORD_UP (lvl, this_y);
      if (SQR_COORD_Y_VALID (lvl, dest_y)) {
	a_square_index dest = SQR_COORDS_TO_INDEX (lvl, dest_y, this_x);
	if ((lvl->square_walls_out[dest] & DM_DOWN)
	    || ((lvl->square_type[dest] == T_TUNNEL)
		&& (lvl->square_direction[dest] == D_DOWN)))
	  square_walls_in[idx] |= DM_DOWN;
      }
    }
    {
      a_square_coord dest_x = SQR_COORD_RIGHT (lvl, this_x);
      if (SQR_COORD_X_VALID (lvl, dest_x)) {
	a_square_index dest = SQR_COORDS_TO_INDEX (lvl, this_y, dest_x);
	if ((lvl->square_walls_out[dest] & DM_LEFT)
	    || ((lvl->square_type[dest] == T_TUNNEL)
		&& (lvl->square_direction[dest] == D_LEFT)))
	  square_walls_in[idx] |= DM_LEFT;
      }
    }
    {
      a_square_coord dest_y = SQR_COORD_DOWN (lvl, this_y);
      if (SQR_COORD_Y_VALID (lvl, dest_y)) {
	a_square_index dest = SQR_COORDS_TO_INDEX (lvl, dest_y, this_x);
	if ((lvl->square_walls_out[dest] & DM_UP)
	    || ((lvl->square_type[dest] == T_TUNNEL)
		&& (lvl->square_direction[dest] == D_UP)))
	  square_walls_in[idx] |= DM_UP;
      }
    }
    {
      a_square_coord dest_x = SQR_COORD_LEFT (lvl, this_x);
      if (SQR_COORD_X_VALID (lvl, dest_x)) {
	a_square_index dest = SQR_COORDS_TO_INDEX (lvl, this_y, dest_x);
	if ((lvl->square_walls_out[dest] & DM_RIGHT)
	    || ((lvl->square_type[dest] == T_TUNNEL)
		&& (lvl->square_direction[dest] == D_RIGHT)))
	  square_walls_in[idx] |= DM_RIGHT;
      }
    }
  }
}

/* Encode the level body into the preallocated buffer data */
void
encode_level_body (a_u8 *data, const a_level *lvl)
{
  a_tile_index ti;		/* Current tile index. */
  a_square_index si;		/* Current square index. */
  a_tile_index tcount;		/* Total tile count to write.  */
  a_dir_mask8 *square_walls_in;	/* Walls forbiding to *enter* a tile.  */

  tcount = lvl->tile_count;
  XCALLOC_ARRAY (square_walls_in, lvl->square_count);
  /* T_OUTWAY squares cannot be entered from any direction */
  for (si = 0; si < lvl->square_count; ++si)
    if (lvl->square_type[si] == T_OUTWAY)
      square_walls_in[si] = DM_ALL;
  en_reverse_walls (lvl, square_walls_in);

  /* Write each tile to buffer. */
  for (ti = 0; ti < tcount; ++ti, data += LVL_RECORD_SIZE) {
    a_tile_type tt;		/* Tile type.  */

    si = TILE_INDEX_TO_SQR_INDEX (lvl, ti);

    /* Store tile type.  */
    tt = lvl->private->tile[ti].type;
    SET_TILE_TYPE (data, tt);

    /* Store inside walls. */
    SET_TILE_WALLS (data, 0, square_walls_in[SQR0 (lvl, si)]);
    SET_TILE_WALLS (data, 1, square_walls_in[SQR1 (lvl, si)]);
    SET_TILE_WALLS (data, 2, square_walls_in[SQR2 (lvl, si)]);
    SET_TILE_WALLS (data, 3, square_walls_in[SQR3 (lvl, si)]);

    /* Store sprites.  */
    SET_TILE_SPRITE (data, lvl->private->tile[ti].sprite_offset);
    SET_TILE_OVERLAY (data, lvl->private->tile[ti].sprite_overlay_offset);

    switch (tt) {
    case T_TUNNEL:
      {
	int s;
	a_dir td;
	a_square_index tsi;
	a_tile_index dti;

	/* find a square that's part of the tunnel */
	s = 0;
	while (lvl->square_type[SQRX (lvl, si, s)] != T_TUNNEL) ++s;
	tsi = SQRX (lvl, si, s);

	td = lvl->square_direction[tsi];
	dti = SQR_INDEX_TO_TILE_INDEX (lvl, lvl->square_move[td][tsi]);
	SET_TUNNEL_OUTPUT (data, dti);
	SET_TUNNEL_DIR (data, DIR_TO_DIRMASK (td));
      }
      break;
    case T_ANIM:
      SET_ANIM_FRAME_COUNT (data, lvl->private->tile[ti].frame_count);
      SET_ANIM_FRAME_DELAY (data, lvl->private->tile[ti].frame_delay - 1);
      break;
    case T_SPEED:
      {
	int x;
	for (x = 0; x < 4; ++x)
	  if (lvl->square_type[SQRX (lvl, si, x)] != T_NONE)
	    SPEED_DIR (data)[x] |=
	      DIR_TO_DIRMASK (lvl->square_direction[SQRX (lvl, si, x)]);
      }
      goto decode_small_anim;
    case T_STOP:
    case T_BOOM:
    case T_ICE:
    case T_DUST:
      {
	int x;
	/* Set EFFECT_SET=non_zero for all squares that have
	   some special effect enabled  */
	for (x = 0; x < 4; ++x)
	  if (lvl->square_type [SQRX (lvl, si, x)] == T_NONE)
	    SET_EFFECT_SET (data, x, 0);
	  else
	    SET_EFFECT_SET (data, x, 1);
      }
      /* Fall through.  */
    decode_small_anim:
    case T_OUTWAY:
    case T_NONE:
      SET_SANIM_FRAME_COUNT_DELAY (data,
				   (lvl->private->tile[ti].frame_count << 4) |
				   (lvl->private->tile[ti].frame_delay - 1));
      break;
    default:
      assert (0);
    }
  }
  free (square_walls_in);
}
