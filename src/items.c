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
#include "items.h"
#include "const.h"
#include "sprshade.h"
#include "sprglenz.h"
#include "sprunish.h"

a_sprite *big_dollar = 0;
a_sprite *small_dollar = 0;
a_sprite *clocks[NBR_CLOCK_FRAMES];
a_sprite *pyramids[NBR_PYRAMIDS];
a_sprite *trails[16][12];
a_sprite *red_cross[4] = { 0, 0, 0, 0 };
a_sprite *tutorial_arrow[NBR_ARROW_FRAMES] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int trail_row[16] = {
  110, 0, -1, 30, 100, 20, 70, -1, -1, 10, 80, 40, 90, -1, 60, 50 };


void
init_items (void)
{
  int i, j;
  a_pcx_image trailimg, arrow_img;

  big_dollar = compile_sprshade (IMGPOS (main_font_img, 81, 0),
				 0, 1, glenz[0],
				 17, 17, main_font_img.width, xbuf);
  small_dollar = compile_sprshade (IMGPOS (main_font_img, 81, 18),
				   0, 1, glenz[0],
				   10, 10, main_font_img.width, xbuf);
  for (i = 0; i < NBR_CLOCK_FRAMES; ++i)
    clocks[i] = compile_sprshade (IMGPOS (main_font_img, 81, 52 + i * 10),
				  0, 1, glenz[0],
				  10, 10, main_font_img.width, xbuf);
  for (i = 0; i < NBR_PYRAMIDS; ++i)
    pyramids[i] = compile_sprshade (IMGPOS (main_font_img, 64, i * 16),
				    0, 1, glenz[0],
				    7, 9, main_font_img.width, xbuf);

  pcx_load_from_rsc ("arrow-img", &arrow_img);
  for (i = 0; i < 3; ++i)
    for (j = 0; j < 4; ++j)
      tutorial_arrow[i * 4 + j] =
	compile_sprunish (IMGPOS (arrow_img, i * 26, j * 26),
			  0, 200, glenz[0], 10, 26, 26, arrow_img.width, xbuf);
  img_free (&arrow_img);

  pcx_load_from_rsc ("trails-img", &trailimg);
  for (i = 0; i < 16; ++i) {
    if (trail_row[i] < 0)
      for (j = 0 ; j < 12; ++j)
	trails[i][j] = 0;
    else
      for (j = 0 ; j < 12; ++j)
	trails[i][j] = compile_sprglenz (IMGPOS (trailimg,
						 trail_row[i], j << 4),
					 0, glenz[0],
					 10, 12, trailimg.width, xbuf);
  }
  for (i = 0; i < 4; ++i)
    red_cross[i] = compile_sprglenz (IMGPOS (main_font_img, 119, i << 6),
				     0, glenz[6],
				     22, 40, main_font_img.width, xbuf);
  img_free (&trailimg);
}

void
uninit_items (void)
{
  int i;
  FREE_SPRITE0 (big_dollar);
  FREE_SPRITE0 (small_dollar);
  for (i = 0; i < NBR_CLOCK_FRAMES; ++i)
    FREE_SPRITE0 (clocks[i]);
  for (i = 0; i < NBR_PYRAMIDS; ++i)
    FREE_SPRITE0 (pyramids[i]);
  for (i = 0; i < NBR_ARROW_FRAMES; ++i)
    FREE_SPRITE0 (tutorial_arrow[i]);
  for (i = 0; i < 16; ++i) {
    int j;
    for (j = 0 ; j < 12; ++j)
      FREE_SPRITE0 (trails[i][j]);
  }
  for (i = 0; i < 4; ++i)
    FREE_SPRITE0 (red_cross[i]);
}
