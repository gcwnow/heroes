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
#include "keyb_low.h"

int mouse_pos_x = 0;
int mouse_pos_y = 0;
char mouse_button_left = 0;
char mouse_button_right = 0;
char mouse_button_middle = 0;

int
init_mouse (void)
{
  return 0;
}

void
mouse_show (void)
{
}

void
mouse_hide (void)
{
}

void
update_mouse_state (void)
{
}

void
process_input_events (void)
{
}

a_keycode
get_key (void)
{
  return 0;
}

int
key_ready (void)
{
  return 0;
}

const char *
keys_pref_group (void)
{
  return "none";
}

int
keycode_to_ascii (a_keycode a)
{
  return a;
}
