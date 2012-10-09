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

#ifndef HEROES__EXTRAS__H
#define HEROES__EXTRAS__H

typedef	char*	a_filename;

typedef struct {
  char*		level_name;
  a_filename	full_name;
  char		is_in_user_dir;
} an_extra_level;

extern an_extra_level *extra_list;
extern char *extra_selected_list;
extern unsigned int extra_nbr;
extern unsigned int extra_user_nbr;

void browse_extra_directories (void);
void add_extra_directory (a_filename fn);
void add_default_extra_directories (void);
void free_extra_list (void);
void free_extra_directories (void);

#endif /* HEROES__EXTRAS__H */
