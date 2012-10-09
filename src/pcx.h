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


#ifndef HEROES__PCX__H
#define HEROES__PCX__H

#include "video.h"

typedef struct pcx_header_type
{
  a_u8 signature;
  a_u8 version;
  a_u8 rle;
  char bits_per_pixels;
  a_u16 x, y;
  a_u16 width, height;
  a_u16 widthdpi, heightdpi;
  a_u8 egapal[48];
  a_u8 inutil;
  a_u8 nbrplanes;
  a_u16 bytes_per_lines;
  a_u16 palette_kind;
  a_u8 rien[58];
}
a_pcx_header ATTRIBUTE_PACKED;

typedef struct
{
  a_u8 r, g, b;
}
an_rvb_color;

typedef union
{
  an_rvb_color indiv[256];
  a_u8 global[256 * 3];
}
a_palette;

typedef struct img_type
{
  a_pcx_header header;
  a_palette palette;
  unsigned int width, height;
  unsigned int size;
  a_pixel *buffer;
}
a_pcx_image;

void img_free (a_pcx_image * image);
char pcx_load (const char *file, a_pcx_image * image);
char pcx_load_from_rsc (const char *rsc, a_pcx_image * image);

#define IMGPOS(img,row,col) ((img).buffer + (row) * (img).width + (col))

#endif /* HEROES__PCX__H */
