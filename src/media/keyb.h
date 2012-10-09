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

#ifndef HEROES__KEYB__H
#define HEROES__KEYB__H

#define HKEY_MAX 0xffff
#define HKEYCODE_MAX	U16_MAX
extern unsigned char keyboard_map[HKEY_MAX + 1];
extern unsigned int keyboard_modifiers;

void init_keyboard_map (void);
void uninit_keyboard_map (void);
int mouse_coord_x (void);
int mouse_coord_y (void);
char mouse1 (void);
char mouse2 (void);
char mouse3 (void);
char mouse12 (void);

void process_input_events (void);
a_keycode get_key (void);
int keycode_to_ascii (a_keycode k);
int key_ready (void);

int init_mouse (void);
void mouse_show (void);
void mouse_hide (void);

const char *keys_pref_group (void);

#endif /* HEROES__KEYB__H */
