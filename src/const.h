/*------------------------------------------------------------------.
| Copyright 1997, 1998, 2000, 2001  Alexandre Duret-Lutz            |
|                                    <duret_g@epita.fr>             |
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

#ifndef HEROES__CONST__H
#define HEROES__CONST__H

/* this file carries too many unrelated stuffs */

#include "pcx.h"
#include "structs.h"
#include "gameid.h"
#include "lvl.h"

extern int rounds_nbr_values[16];

extern a_pixel *(render_buffer[2]);	/* xbuf * ybuf */

char key_or_joy_ready (void);
a_keycode get_key_or_joy (void);

extern char kbjoy[6];
extern char kbjoyold[6];
extern char in_jokebox;
extern char in_menu;
extern char in_demo;

extern char demo_ready;

extern a_pixel glenz[8][256];
void draw_glenz_box (a_pixel *dest, int c, int xt, int yt);

extern a_pcx_image main_font_img, vehicles_img;
extern a_pcx_image bonus_font_img;
extern a_pcx_image tile_set_img, font_deck_img;

extern signed char minisinus[32];
extern bool two_players;

#define maxq 128

extern a_player player[4];
extern int trail_pos[4][maxq];
extern a_dir8_pair trail_way[4][maxq];
extern int trail_offset[4];
extern char trail_size[4];	/* size of trails, minus one */

extern int col2plr[4];
extern int plr2col[4];

extern a_level lvl;

extern unsigned char *square_occupied;
extern a_dir8_pair *square_way;
extern a_tile_index *square_tile;
extern a_square_corrd_pair *square_coord;
extern signed char *square_object;
#define lemmings_per_players 50
#define lemmings_total (lemmings_per_players*4)
extern a_lemming **square_lemmings_list;
extern a_lemming **square_dead_lemmings_list;
extern a_lemming lemmings_support[lemmings_total];
extern int objects_nbr;

extern int game_mode;
extern a_gameid game_id;

extern int lemmings_anim_offset;
extern int lemmings_move_offset;

#endif /* HEROES__CONST__H */
