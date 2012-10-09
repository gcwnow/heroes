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
#include "format.h"

/* Simple helper function to transform masked direction into
   directions.  */
static a_dir
dir_mask_to_dir (a_dir_mask dm)
{
  switch (dm) {
  case DM_UP:
    return D_UP;
  case DM_RIGHT:
    return D_RIGHT;
  case DM_DOWN:
    return D_DOWN;
  case DM_LEFT:
    return D_LEFT;
  default:
    /* FIXME: This is not smart.  */
    assert (1);
    return 0;
  }
}



/* Each tunnel has two input/output squares, indiced 0 and 1.
   tunnel_square_io[][] is used to build the map of square links as it
   helps to locate the square used to exit from a tunnel, given the
   direction of the output tunnel (first index), and the  square
   input number (second index).
   For instance, if tile A is a tunnel oriented up, linked to
   tile B which is a tunnel oriented right, we need to:
    link  A's square number `tunnel_square_io[w_up][0]'
      to  B's square number `tunnel_square_io[w_right][1]'
   and link  A's square number `tunnel_square_io[w_up][1]'
         to  B's square number `tunnel_square_io[w_right][0]'
*/
int tunnel_square_io[4][2] = { {0, 1}, {1, 3}, {3, 2}, {2, 0} };


/* Reverse the walls.

   square_walls_in indicates directions which a square cannot be
   *entered* with, but the game uses square_walls_out, which indicates
   directions for which *exiting* a square is not allowed.

   So we have to set square_walls_out by `reversing' the content of
   square_walls_in.
*/
static void
reverse_walls (a_level *lvl, const a_dir_mask8 *square_walls_in)
{
  a_square_index idx;

  memset (lvl->square_walls_out, 0, lvl->square_count);

  for (idx = 0; idx < lvl->square_count; ++idx) {
    a_square_index *dest;
    a_dir_mask8 *swo = lvl->square_walls_out + idx;
    bool is_tunnel = (lvl->square_type[idx] == T_TUNNEL);
    a_dir tunnel_dir = lvl->square_direction[idx];

    /* For each output direction of the square, we check whether that
       direction is marked as a inside-wall in the destination tile
       (meaning that it's not allowed to enter the destination wall by
       this edge).  If so, we mark the direction as invalid for the
       current square.

       This does not apply if the current square is a tunnel and the
       direction considered is the entrance of the tunnel.  */

    if (!is_tunnel || tunnel_dir != D_UP) {
      dest = &lvl->square_move[D_UP][idx];
      if ((*dest == INVALID_INDEX) || (square_walls_in[*dest] & DM_UP)) {
	*swo |= DM_UP;
	*dest = INVALID_INDEX;
      }
    }

    if (!is_tunnel || tunnel_dir != D_RIGHT) {
      dest = &lvl->square_move[D_RIGHT][idx];
      if ((*dest == INVALID_INDEX) || (square_walls_in[*dest] & DM_RIGHT)) {
	*swo |= DM_RIGHT;
	*dest = INVALID_INDEX;
      }
    }

    if (!is_tunnel || tunnel_dir != D_DOWN) {
      dest = &lvl->square_move[D_DOWN][idx];
      if ((*dest == INVALID_INDEX) || (square_walls_in[*dest] & DM_DOWN)) {
	*swo |= DM_DOWN;
	*dest = INVALID_INDEX;
      }
    }

    if (!is_tunnel || tunnel_dir != D_LEFT) {
      dest = &lvl->square_move[D_LEFT][idx];
      if ((*dest == INVALID_INDEX) || (square_walls_in[*dest] & DM_LEFT)) {
	*swo |= DM_LEFT;
	*dest = INVALID_INDEX;
      }
    }
  }
}

/* Load the level body, and initialize most of the LVL data.  */
void
decode_level_body (const a_u8 *data, a_level *lvl)
{
  const a_u8 *base_data = data;
  a_tile_index ti;		/* Current tile index. */
  a_square_index si;		/* Current square index. */
  a_tile_index tcount;		/* Total tile count to read.  */
  a_dir_mask8 *square_walls_in;	/* Walls forbiding to *enter* a tile.  */

  tcount = lvl->tile_count;
  square_walls_in = xmalloc (lvl->square_count);

  /* Initialize the default direction for all squares.  Although for
     square where it matters, the direction is initialized below, we need
     a default direction for tools like 'heroeslvl' who attempt to
     dump the entire 'square_direction' array.  */
  memset (lvl->square_direction, D_UP,
	  lvl->square_count * sizeof *lvl->square_direction);

  /* Read each tile.  */
  for (ti = 0; ti < tcount; ++ti, data += LVL_RECORD_SIZE) {
    a_tile_type tt;		/* Tile type.  */

    si = TILE_INDEX_TO_SQR_INDEX (lvl, ti);

    /* Store tile type.  */
    tt = GET_TILE_TYPE (data);
    lvl->private->tile[ti].type = tt;
    lvl->square_type[SQR0 (lvl, si)] = tt;
    lvl->square_type[SQR1 (lvl, si)] = tt;
    lvl->square_type[SQR2 (lvl, si)] = tt;
    lvl->square_type[SQR3 (lvl, si)] = tt;

    /* Store inside walls.  This will be used latter to initialize
       outside walls (only the latter are useful in the game).  */
    square_walls_in[SQR0 (lvl, si)] = GET_TILE_WALLS (data)[0];
    square_walls_in[SQR1 (lvl, si)] = GET_TILE_WALLS (data)[1];
    square_walls_in[SQR2 (lvl, si)] = GET_TILE_WALLS (data)[2];
    square_walls_in[SQR3 (lvl, si)] = GET_TILE_WALLS (data)[3];

    /* Store sprite.  */
    lvl->private->tile[ti].sprite_offset = GET_TILE_SPRITE (data);
    lvl->private->tile[ti].sprite_overlay_offset = GET_TILE_OVERLAY (data);

    switch (tt) {
    case T_TUNNEL:
      {
	a_tile_index dti;	/* Destination tile index.  */
	a_square_index dsi;	/* Destination square index.  */
	a_dir td;		/* Tunnel direction.  */
	a_dir dtd;		/* Tunnel direction for destination tile.  */

	dti = GET_TUNNEL_OUTPUT (data);
	dsi = TILE_INDEX_TO_SQR_INDEX (lvl, dti);
	td = GET_TUNNEL_DIR (data);

	/* In order to link theses squares (inputs of the tunnel) to
	   the output squares, we need to know the direction used for
	   the output.  This direction is encoded in the output tile's
	   record unless it's not a T_TUNNEL, in this latter case
	   we'll just assume the output direction is the reverse of
	   the input direction.  */
	if (GET_TILE_TYPE (base_data + dti * LVL_RECORD_SIZE) == T_TUNNEL)
	  dtd = GET_TUNNEL_DIR (base_data + dti * LVL_RECORD_SIZE);
	else
	  dtd = REVERSE_DIR (td);

	/* Adjust some output link of the relevant squares on this
	   tile, so that leaving the square in the tunnel direction
	   will lead to the tunnel destination square instead of the
	   neighbor square.  See also the description of
	   tunnel_square_io.  */
	lvl->square_move[td][SQRX (lvl, si, tunnel_square_io[td][0])] =
	  SQRX (lvl, dsi, tunnel_square_io[dtd][1]);
	lvl->square_move[td][SQRX (lvl, si, tunnel_square_io[td][1])] =
	  SQRX (lvl, dsi, tunnel_square_io[dtd][0]);
	/* Adjust directions to indicate the output.  */
	lvl->square_direction[SQRX (lvl, si, tunnel_square_io[td][0])] = td;
	lvl->square_direction[SQRX (lvl, si, tunnel_square_io[td][1])] = td;

	/* If tunnel_square_io[td][0] and tunnel_square_io[td][1] are
	   tunnel departures squares, that means
	   tunnel_square_io[REVERSE_DIR (td)][0] and
	   tunnel_square_io[REVERSE_DIR (td)][1] are not tunnels.  */
	lvl->square_type[SQRX (lvl, si,
			       tunnel_square_io[REVERSE_DIR (td)][0])]
	  = T_NONE;
	lvl->square_type[SQRX (lvl, si,
			       tunnel_square_io[REVERSE_DIR (td)][1])]
	  = T_NONE;
      }
      break;
    case T_ANIM:
      lvl->private->tile[ti].frame_count = GET_ANIM_FRAME_COUNT (data);
      lvl->private->tile[ti].frame_delay = GET_ANIM_FRAME_DELAY (data);
      lvl->private->tile[ti].anim = A_LOOP;
      break;
    case T_SPEED:
      {
	int x;
	for (x = 0; x < 4; ++x) {
	  a_dir_mask dm = GET_SPEED_DIR (data)[x];
	  if (dm)
	    lvl->square_direction[SQRX (lvl, si, x)] = dir_mask_to_dir (dm);
	  else
	    lvl->square_type[SQRX (lvl, si, x)] = T_NONE;
	}
      }
      goto decode_small_anim;
    case T_STOP:
    case T_BOOM:
    case T_ICE:
    case T_DUST:
      {
	int x;
	/* The type of each square is already set, but we don't want
	   it on the squares where effects are not enabled.  */
	for (x = 0; x < 4; ++x)
	  if (! GET_EFFECT_SET (data)[x])
	    lvl->square_type [SQRX (lvl, si, x)] = T_NONE;
      }
      /* Fall through.  */
    decode_small_anim:
    case T_OUTWAY:
    case T_NONE:
      lvl->private->tile[ti].frame_count = GET_SANIM_FRAME_COUNT (data);
      lvl->private->tile[ti].frame_delay = GET_SANIM_FRAME_DELAY (data);
      if (lvl->private->tile[ti].frame_count)
	lvl->private->tile[ti].anim = A_PINGPONG;
      else
	lvl->private->tile[ti].anim = A_NONE;
      break;
    default:
      assert (0);
    }

  }

  /* Initialize lvl->square_walls_out from square_walls_in.  */
  reverse_walls (lvl, square_walls_in);

  /* Mark all inaccessible squares as T_OUTWAY.  */
  for (si = 0; si < lvl->square_count; ++si)
    if (square_walls_in[si] == DM_ALL)
      lvl->square_type[si] = T_OUTWAY;

  free (square_walls_in);
}
