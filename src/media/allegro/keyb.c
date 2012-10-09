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
#include <assert.h>
#include <allegro.h>
#include "keyb_low.h"
#include "errors.h"

int mouse_pos_x = 0;
int mouse_pos_y = 0;
char mouse_button_left = 0;
char mouse_button_right = 0;
char mouse_button_middle = 0;

int
init_mouse (void)
{
  install_mouse ();
  return 0;
}

void
mouse_show (void)
{
  show_mouse (screen);
}

void
mouse_hide (void)
{
  show_mouse (0);
}

void
update_mouse_state (void)
{
  poll_mouse ();
  mouse_pos_x = mouse_x;
  mouse_pos_y = mouse_y;
  mouse_button_left = mouse_b & 1;
  mouse_button_right = mouse_b & 2;
  mouse_button_middle = mouse_b & 4;
}

void
process_input_events (void)
{
  int i;

  poll_keyboard ();

  /* KEY_MAX is from Allegro, HKEY_MAX is from Heroes.  */
  for (i = 0; i < KEY_MAX; ++i)
    keyboard_map[i] = key[i];

  keyboard_modifiers = key_shifts;

  /* Apparently we have to clear the keybuf here, otherwise the next
     call to get_key() will still see the event polled here.  (At
     least that's what I can see under X.)  */
  clear_keybuf ();
}

a_keycode
get_key (void)
{
  return readkey () >> 8;
}

int
key_ready (void)
{
  return keypressed ();
}

const char *
keys_pref_group (void)
{
  return "allegro";
}

int
keycode_to_ascii (a_keycode a)
{
  return scancode_to_ascii (a);
}
