/*------------------------------------------------------------------------.
| Copyright 2000  Alexandre Duret-Lutz <duret_g@epita.fr>                 |
|                                                                         |
| This file is part of Heroes.                                            |
|                                                                         |
| Heroes is free software; you can redistribute it and/or modify it under |
| the terms of the GNU General Public License version 2 as published by   |
| the Free Software Foundation.                                           |
|                                                                         |
| Heroes is distributed in the hope that it will be useful, but WITHOUT   |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   |
| for more details.                                                       |
|                                                                         |
| You should have received a copy of the GNU General Public License along |
| with this program; if not, write to the Free Software Foundation, Inc., |
| 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA                   |
`------------------------------------------------------------------------*/

#include "system.h"
#include "renderdata.h"
#include "errors.h"
#include "debugmsg.h"
#include "structs.h"
#include "const.h"
#include "sprrle.h"

a_bg_data *bg_data = 0;
a_fg_data *fg_data = 0;

int tile_set_size = 0;		/* number of tiles in the tile set */
a_sprite **tile_transp;		/* An array as wide as the tile set,
				   which might contains pointer to
				   the a_sprite for a tile, when
				   that tile is used as a transparant
				   sprite. */
static void
init_tile_sprites (void)
{
  tile_set_size = (tile_set_img.width / 24) * 10;
  XCALLOC_ARRAY (tile_transp, tile_set_size);
}

static void
uninit_tile_sprites (void)
{
  while (tile_set_size--)
    XFREE0 (tile_transp[tile_set_size]);
  XFREE0 (tile_transp);
}

static a_sprite *
get_tile_sprite (unsigned int offset)
{
  /* convert an offset-in-image, into a tile-number */
  int tile_row = offset / (tile_set_img.width * 20);
  int tile_col = (offset % (tile_set_img.width * 20)) / 24;
  int tile_pos = tile_row * (tile_set_img.width / 24) + tile_col;

  /* don't recompile the sprite if it's already done */
  if (!tile_transp[tile_pos])
    tile_transp[tile_pos] =
      compile_sprrle (tile_set_img.buffer + offset, 0, 20, 24,
		      tile_set_img.width, xbuf);

  return tile_transp[tile_pos];
}

void
uninit_render_data (void)
{
  dmsg (D_SECTION, "Uninitialize rendering data");

  uninit_tile_sprites ();
  XFREE0 (bg_data);
  XFREE0 (fg_data);
}

void
init_render_data (void)
{
  a_tile_index pos;
  a_tile_index max_pos = lvl.tile_count;

  dmsg (D_SECTION, "Initialize rendering data");

  XMALLOC_ARRAY (bg_data, max_pos);
  XMALLOC_ARRAY (fg_data, max_pos);
  init_tile_sprites ();

  /* initialize background tile information */

  for (pos = 0; pos < max_pos; ++pos) {

    /* background data */

    bg_data[pos].source =
      lvl_tile_sprite_offset (&lvl, pos) + tile_set_img.buffer;

    lvl_animation_info (&lvl, pos, &bg_data[pos].anim_frames,
			&bg_data[pos].anim_speed,
			&bg_data[pos].kind);

    /* foreground data */
    {
      unsigned int ovlay = lvl_tile_sprite_overlay_offset (&lvl, pos);

      if (ovlay)
	fg_data[pos].sprite = get_tile_sprite (ovlay);
      else
	fg_data[pos].sprite = 0;
    }
    fg_data[pos].bonus = 0;
    fg_data[pos].big_dollar = 0;
  }
}
