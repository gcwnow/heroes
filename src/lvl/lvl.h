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

#ifndef HEROES__LVL__H
#define HEROES__LVL__H

/* Tiles are units of 24x20 pixels, squares are units of 12x10 pixels.
   There is 4 squares per tiles.

   Functions can use squares or tiles, depending on the resolution
   with which entities are stored.  For instance the bonuses are
   stored in a tile-sized array, while the vehicules are stored in an
   squares-sized array.

   Those types are defined for better readability.  The a_*_coord
   types are used to designate a coordinate (either X, or Y), while
   the a_*_index are used for square numbers (indices in one
   dimensional arrays: index = y_coord * width + x_coord).  */
typedef unsigned int a_square_coord;
typedef unsigned int a_square_index;
typedef unsigned int a_tile_coord;
typedef unsigned int a_tile_index;
#define INVALID_INDEX (~0u)

typedef struct a_square_corrd_pair a_square_corrd_pair;
struct a_square_corrd_pair {
  a_square_coord y, x;
};

typedef struct a_tile_coord_pair a_tile_coord_pair;
struct a_tile_coord_pair {
  a_tile_coord y, x;
};

/* Direction constants.  */
typedef enum a_dir a_dir;
enum a_dir { D_UP = 0,
	     D_RIGHT = 1,
	     D_DOWN = 2,
	     D_LEFT = 3 };
typedef a_u8 a_dir8;		/* Hold the same values, on 8bits. */
#define REVERSE_DIR(d) ((d) ^ 2)

/* Two direction on one byte (used for trails).
   Don't change the encoding, this the resulting value is used
   to lookup sprites to draw the trail.  */
typedef a_u8 a_dir8_pair;
#define DIR8_TAIL(X) ((X) >> 2)
#define DIR8_HEAD(X) ((X) & 0x3)
#define DIR8_PAIR(H, T) (((T) << 2) | (H))


/* Number of directions,
   unless Heroes goes 3D this is unlikely to change :) */
#define DIR_MAX 4

/* Similar constants, but OR-able, to indicate multiple directions.  */
typedef enum a_dir_mask a_dir_mask;
enum a_dir_mask { DM_UP = 1,
		  DM_RIGHT = 2,
		  DM_DOWN = 4,
		  DM_LEFT = 8 };
typedef a_u8 a_dir_mask8;	/* Hold the same values, on 8bits. */
#define DM_ALL (DM_UP | DM_RIGHT | DM_DOWN | DM_LEFT)
#define DIR_TO_DIRMASK(dir) (1 << (dir))

/* Tile type constants.  */
typedef enum a_tile_type a_tile_type;
enum a_tile_type { T_NONE = 0,
		   T_STOP = 1,
		   T_SPEED = 2,
		   T_TUNNEL = 3,
		   T_BOOM = 4,
		   T_ANIM = 5,
		   T_ICE = 6,
		   T_DUST = 7,
		   T_OUTWAY = 8,
		   T_MAXTYPE	/* Keep this upper bound last in the enum. */};
typedef a_u8 a_tile_type8;	/* Hold the same values, on 8bits. */


typedef struct a_level a_level;	/* An Heroes level.  */
typedef struct a_level_bits a_level_bits; /* Hiden details about levels.  */


/* The pointers in the a_level structure should point to constant
   data.  But from the level loading code the data need to be mutable.
   So we allow const to be removed by defining LVL_MUTABLE.  */

#ifndef LVL_MUTABLE
# define LVL_MUTABLE const
#endif

struct a_level {
  /*-------------.
  | Dimensions.  |
  `-------------*/

  /* We store the dimensions in those two units, so that they are
     directly available.  */
  a_tile_coord tile_height;
  a_tile_coord tile_width;
  a_square_coord square_height;
  a_square_coord square_width;

  /* Some levels are wrapped: when the vehicle reach the right edge of
     the level, they continue as if they were entering the level from
     the left edge, and vice-versa.  Of course this can also be set
     for the top and bottom.

     The _wrap members should be ANDed with a coordinate to wrap it.
     For instance a wrapped level with a width of 32 will have
     `tile_width_wrap' set to 31.  That way an y coordinate of 35 will
     be mapped to 3 (== 35 & 31).  Non-wrapped level set this value to
     `~0u' (== DONT_WRAP) so that ANDing is no-op.  */
  a_tile_coord tile_height_wrap;
  a_tile_coord tile_width_wrap;
  a_square_coord square_height_wrap;
  a_square_coord square_width_wrap;

  /* Most maps are allocated as one-dimensional arrays.  So we define
     to more constants used for the size of the arrays.  */
  a_tile_index tile_count;	/* == tile_width * tile_height */
  a_square_index square_count;	/* == square_width * square_height */

  /*----------------------------------------------------------------.
  | The following arrays are initialized only if the level body has |
  | been loaded.  See the load_body argument of lvl_load_file.      |
  `----------------------------------------------------------------*/

  /* Type of each square.  */
  LVL_MUTABLE a_tile_type8 *square_type;
  /* Set of blocked directions for each square.  If the entry is set
     to DM_UP|DM_LEFT, it means a vehicle cannot *leave* the square
     by the top or left edges.  */
  LVL_MUTABLE a_dir_mask8 *square_walls_out;
  /* Directions for squares for which it matters (i.e., T_SPEED, and
     T_TUNNEL).  */
  LVL_MUTABLE a_dir8 *square_direction;
  /* square_move[D_UP][SQR] is the square that a vehicle shoud go into
     when it leave SQR by the top edge.  */
  LVL_MUTABLE a_square_index *square_move[DIR_MAX];

  /*-------------------------------------------------------------.
  | Opaque structure to hide the remaining data.  Use one of the |
  | following functions to access anything relevant in there.    |
  `-------------------------------------------------------------*/

  LVL_MUTABLE a_level_bits *private;
};

#ifndef DONT_WRAP
# define DONT_WRAP (~0u)	/* Used for non wrapping directions.  */
#endif

/* Load a level from FILENAME to OUT.  Return 0 on success, !0
   otherwise.  If LOAD_BODY is true, parse the body, and compute the
   various constant square_* maps declared in a_level.  If LOAD_BODY
   is false, only the header of the level will be loaded.  */
int lvl_load_file (const char *filename, a_level *out, bool load_body);
int lvl_save_file (const char *filename, const a_level *out);
/* Free any data associated to LVL.  */
void lvl_free (a_level *lvl);

/* These two functions return a pointer to a string allocated for LVL,
   and freed by lvl_free().  You should not free them yourself, and
   you should not use these results after a call to lvl_free().  */
const char *lvl_sound_track (const a_level *lvl);
const char *lvl_tile_sprite_map_basename (const a_level *lvl);

/* Initialize COORD, and DIR, with the starting position for PLAYER.  */
void lvl_start_position (const a_level *lvl, unsigned int player,
			 a_square_corrd_pair *coord, a_dir *dir);

/*--------------------------------------------------------------------.
| NOTE: The following function will fail if the body of the level     |
| has not been loaded (see the load_body argument of lvl_load_file).  |
`--------------------------------------------------------------------*/

/* Return the type of tile TILE on level LVL.  */
a_tile_type lvl_tile_type (const a_level *lvl, a_tile_index tile);

/* Return the output direction of a tunnel on tile TILE.  This function
   should only be called on tiles of type T_TUNNEL.  */
a_dir lvl_tunnel_output_dir (const a_level *lvl, a_tile_index tile);

/* Return the offset (to the first pixel) of the sprite used to draw TILE.  */
unsigned int lvl_tile_sprite_offset (const a_level *lvl,
				     a_tile_index tile);
/* Return the offset (to the first pixel) of the overlay sprite to
   draw on TILE.  */
unsigned int lvl_tile_sprite_overlay_offset (const a_level *lvl,
					     a_tile_index tile);

/* Kind of animation.  */
typedef enum an_anim_kind an_anim_kind;
enum an_anim_kind { A_NONE, A_LOOP, A_PINGPONG };

/* Return information about a possible animation on tile TILE.
   FRAME_COUNT is the number of frames to display, and DELAY a number
   of 1/70sec to wait between each frame.  Actually, FRAME_COUNT is
   the number of sprites to display: FRAME_COUNT sprites starting at
   the sprite index for the current tile should be displayed.

   There is no animation if FRAME_COUNT is set to 0.  */
void lvl_animation_info (const a_level *lvl, a_tile_index tile,
			 unsigned int *frame_count, unsigned int *delay,
			 an_anim_kind *kind);

/*----------------.
| Useful macros.  |
`----------------*/

/* Moving to neighbor squares.  */
#define SQR_COORD_LEFT(lvl_ptr, sqr_x_coord) \
  (((sqr_x_coord) - 1) & (lvl_ptr)->square_width_wrap)
#define SQR_COORD_RIGHT(lvl_ptr, sqr_x_coord) \
  (((sqr_x_coord) + 1) & (lvl_ptr)->square_width_wrap)
#define SQR_COORD_UP(lvl_ptr, sqr_y_coord) \
  (((sqr_y_coord) - 1) & (lvl_ptr)->square_height_wrap)
#define SQR_COORD_DOWN(lvl_ptr, sqr_y_coord) \
  (((sqr_y_coord) + 1) & (lvl_ptr)->square_height_wrap)

/* Checking square coordinates.  */
#define SQR_COORD_X_VALID(lvl_ptr, sqr_coord_x) \
  (((sqr_coord_x) < (lvl_ptr)->square_width))
#define SQR_COORD_Y_VALID(lvl_ptr, sqr_coord_y) \
  (((sqr_coord_y) < (lvl_ptr)->square_height))
#define SQR_COORD_VALID(lvl_ptr, sqr_coord_y, sqr_coord_x) \
  (SQR_COORD_Y_VALID (lvl_ptr, sqr_coord_y) \
   && SQR_COORD_X_VALID (lvl_ptr, sqr_coord_x))

/* Index to coordinate conversions.  */
#define SQR_INDEX_TO_COORD_X(lvl_ptr, sqr_index) \
  ((sqr_index) % (lvl_ptr)->square_width)
#define SQR_INDEX_TO_COORD_Y(lvl_ptr, sqr_index) \
  ((sqr_index) / (lvl_ptr)->square_width)
#define SQR_COORDS_TO_INDEX(lvl_ptr, sqr_y, sqr_x) \
  ((lvl_ptr)->square_width * (sqr_y) + (sqr_x))

#define TILE_INDEX_TO_COORD_X(lvl_ptr, tile_index) \
  ((tile_index) % (lvl_ptr)->tile_width)
#define TILE_INDEX_TO_COORD_Y(lvl_ptr, tile_index) \
  ((tile_index) / (lvl_ptr)->tile_width)
#define TILE_COORDS_TO_INDEX(lvl_ptr, tile_y, tile_x) \
  ((lvl_ptr)->tile_width * (tile_y) + (tile_x))


/* Tile indexes to square indexes conversions.  */
#define TILE_INDEX_TO_SQR_INDEX(lvl_ptr, tile_index)		\
  (TILE_INDEX_TO_COORD_Y ((lvl_ptr), (tile_index)) * 2		\
   * (lvl_ptr)->square_width					\
   + TILE_INDEX_TO_COORD_X ((lvl_ptr), (tile_index)) * 2)

#define SQR_INDEX_TO_TILE_INDEX(lvl_ptr, sqr_index)		\
  ((SQR_INDEX_TO_COORD_Y ((lvl_ptr), (sqr_index)) / 2)		\
   * (lvl_ptr)->tile_width					\
   + (SQR_INDEX_TO_COORD_X ((lvl_ptr), (sqr_index)) / 2))

/* Given the index of the top left square of a tile,
   compute the index for each square in the tile.  */
#define SQR0(lvl_ptr, sqr0) (sqr0)
#define SQR1(lvl_ptr, sqr0) ((sqr0) + 1)
#define SQR2(lvl_ptr, sqr0) ((sqr0) + (lvl_ptr)->square_width)
#define SQR3(lvl_ptr, sqr0) ((sqr0) + (lvl_ptr)->square_width + 1)
#define SQRX(lvl_ptr, sqr0, x) \
  ((sqr0) + (((x) & 2) ? (lvl_ptr)->square_width : 0) + ((x) & 1))


#endif /* HEROES__LVL__H */
