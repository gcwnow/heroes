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

#include "system.h"
#include "keyb_low.h"
#include "argv.h"
#include "errors.h"

unsigned int keyboard_modifiers;
unsigned char keyboard_map[HKEY_MAX + 1];

void
init_keyboard_map (void)
{
  int i;

  for (i = HKEY_MAX; i >= 0; i--)
    keyboard_map[i] = 0;
  keyboard_modifiers = 0;
}

void
uninit_keyboard_map (void)
{
}

int
mouse_coord_x (void)
{
  update_mouse_state ();
  return mouse_pos_x / stretch;
}

int
mouse_coord_y (void)
{
  update_mouse_state ();
  return mouse_pos_y / stretch;
}

char
mouse1 (void)
{
  update_mouse_state ();
  return mouse_button_left;
}

char
mouse2 (void)
{
  update_mouse_state ();
  return mouse_button_right;
}

char
mouse3 (void)
{
  update_mouse_state ();
  return mouse_button_middle;
}

/* FIXME: this should be renamed */
char
mouse12 (void)
{
  update_mouse_state ();
  return mouse_button_left || mouse_button_right || mouse_button_middle;
}
