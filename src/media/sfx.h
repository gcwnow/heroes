/*------------------------------------------------------------------------.
| Copyright 1997, 1998, 2000  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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

#ifndef HEROES__SOUNDEFFECT__H
#define HEROES__SOUNDEFFECT__H

char read_sfx_conf (void);
void close_sfx_handle (void);
void load_sfx_mode (signed char mode);
void free_all_sfx (void);
void event_sfx (int event);

void *load_sfx_low (char *file);
void free_sfx_low (void *sfx);
void play_sfx_low (void *sfx);

#endif /* HEROES__SOUNDEFFECT__H */
