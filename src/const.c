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

/* Heroes constants */

#include "system.h"
#include "keyb.h"
#include "keyvalues.h"
#include "joystick.h"
#include "pcx.h"
#include "sfx.h"
#include "structs.h"
#include "prefs.h"
#include "video.h"
#include "const.h"
#include "timer.h"
#include "heroes.h"
#include "argv.h"

/* available round numbers */
int rounds_nbr_values[16] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 30, 50, 100
};

char in_menu = 1;
char in_demo = 0;
/* int error; */
char in_jokebox;

char kbjoy[6] = { 0, 0, 0, 0, 0, 0 };
char kbjoyold[6] = { 0, 0, 0, 0, 0, 0 };

a_pixel *(render_buffer[2]);		/* xbuf * ybuf */

a_pixel glenz[8][256];		/* glenz lines */

a_pcx_image main_font_img, vehicles_img;
a_pcx_image bonus_font_img;
a_pcx_image tile_set_img, font_deck_img;

signed char minisinus[32];
bool two_players = false;

a_player player[4];
int trail_pos[4][maxq];
a_dir8_pair trail_way[4][maxq];
int trail_offset[4];
char trail_size[4];		/* trail size, minus one */

/* mapping color <-> player */
int col2plr[4];
int plr2col[4];

a_level lvl;

unsigned char *square_occupied;
a_dir8_pair *square_way;
a_tile_index *square_tile;
a_square_corrd_pair *square_coord;
signed char *square_object;
a_lemming **square_lemmings_list;
a_lemming **square_dead_lemmings_list;
a_lemming lemmings_support[lemmings_total];
int objects_nbr;

int game_mode = 0;
a_gameid game_id;
int lemmings_anim_offset;
int lemmings_move_offset;

char demo_ready = 0;

char
key_or_joy_ready (void)
{
  int i;
  int ct = read_htimer (demo_trigger_htimer);

  if ((in_jokebox == 0)) {
    if (ct >= 30 || check_what == check_demo)
      demo_ready = 1;
    else
      demo_ready = 0;
    if (ct >= 60) {
      if (in_menu)
	event_sfx (118 + (rand () & 1));
      reset_htimer (demo_trigger_htimer);
    }
  }

  if (key_ready ()) {
    reset_htimer (demo_trigger_htimer);
    return (1);
  }
  if ((joystick_detected & 1) && (opt.ctrl_one == 1 || opt.ctrl_two == 1)) {
    for (i = 0; i < 6; i++)
      kbjoyold[i] = kbjoy[i];
    get_joystick_state ();
    kbjoy[0] = is_joystick_up (0);
    kbjoy[1] = is_joystick_right (0);
    kbjoy[2] = is_joystick_down (0);
    kbjoy[3] = is_joystick_left (0);
    kbjoy[4] = is_joystick_button_a (0);
    kbjoy[5] = is_joystick_button_b (0);
    if ((kbjoy[0] && !kbjoyold[0])
	|| (kbjoy[1] && !kbjoyold[1])
	|| (kbjoy[2] && !kbjoyold[2])
	|| (kbjoy[3] && !kbjoyold[3])
	|| (kbjoy[4] && !kbjoyold[4])
	|| (kbjoy[5] && !kbjoyold[5])) {
      reset_htimer (demo_trigger_htimer);
      return (1);
    }
  }
  return (0);
}

a_keycode
get_key_or_joy (void)
{
  if (key_ready ())
    return (get_key ());
  if (kbjoy[0] && !kbjoyold[0])
    return (HK_Up);
  if (kbjoy[1] && !kbjoyold[1])
    return (HK_Right);
  if (kbjoy[2] && !kbjoyold[2])
    return (HK_Down);
  if (kbjoy[3] && !kbjoyold[3])
    return (HK_Left);
  if (kbjoy[4] && !kbjoyold[4])
    return (HK_Enter);
  if (kbjoy[5] && !kbjoyold[5])
    return (HK_Escape);
  printf ("get_key_or_joy(): no event.");
  return (0);
}

void
draw_glenz_box (a_pixel *dest, int c, int xt, int yt)
{
  a_pixel *glenzligne = glenz[c];
  int xt2;

  for (; yt != 0; yt--) {
    for (xt2 = xt; xt2 != 0; --xt2, ++dest)
      *dest = glenzligne[*dest];
    dest += xbuf - xt;
  }

}
