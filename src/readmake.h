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

#ifndef HEROES__READMAKE__H
#define HEROES__READMAKE__H

#include "sprite.h"


typedef struct private_read_data_s a_private_read_data;

typedef struct read_data_s a_read_data;
struct read_data_s {
  a_private_read_data *data;
  a_private_read_data *data_bg;
  int max;
};


/* Prepare STR for the reader rendering function, and a
   append it to the already cooked data in HEAD.  If HEAD is NULL,
   a new structure is allocated. */
a_read_data *compile_reader_data (a_read_data *head, const char *str);

/* Free any data allocated by compile_reader_data for RD. */
void free_reader_data (a_read_data *rd);

/* draw RD form line MIN to line MAX on DEST on DEST. */
void draw_reader_data (const a_read_data *rd, a_pixel *dest, int min, int max);

#endif /* HEROES__READMAKE__H */
