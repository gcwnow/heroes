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

#ifndef HEROES__SPRITES__H
#define HEROES__SPRITES__H

/*-------------------------------------------------------------------.
| This file define an abstraction for sprites, i.e., small blocks of |
| pixels that can be pasted in memory (but you knew that).           |
`-------------------------------------------------------------------*/

#include "video.h"

/* Different kind of sprites, they are not all used yet.  These values
   should be used *internally* by the sprite handling functions.  */
enum sprite_kind {
  S_OPAQUE,			/* opaque (yes!) */
  S_RLE,			/* transparant */
  S_RLE_SHADE,			/* transparant, using one line of glenz */
  S_RLE_ZCOL,			/* transparant, using a kind of
  				   color-based z-buffer, for explosions */
  S_RLE_GLENZ,			/* transparant,
				   using uniform glenz for the rest */
  S_RLE_UNIC_SHADE,		/* transparant, using a UNIque opaque Color,
				   and one line of glenz */
  S_RLE_GLENZ_AUTO,		/* transparant, using glenz for some
				   known colors (this is slow) */
  S_PROG,			/* a list of sprites, to draw all at once */
  S_PROG_WAV			/* like S_PROG but also wave */
};

typedef union sprite_s a_sprite;

typedef void (*sprite_draw_func) (const a_sprite *sprite, a_pixel *dest);

/*-------------------------------------------------------------------.
| Because the various flavor of sprites are not drawn the same way,  |
| there exist various drawing functions.  There is one drawing       |
| function for each kind of sprite.                                  |
|                                                                    |
| Each sprite contains a pointer to its drawing function.  Therefore |
| the DRAW_SPRITE macro below is the generic way to draw a sprite.   |
| It is more efficient, however, to call the right drawing function  |
| if you know it at compile time.                                    |
`-------------------------------------------------------------------*/

#define DRAW_SPRITE(s,dest) ((s)->draw ((s), (dest)))

#define SPRITE_FIRST_MEMBER			\
  sprite_draw_func	draw


#define SPRITE_COMMON_MEMBERS				\
  SPRITE_FIRST_MEMBER;					\
  enum sprite_kind	kind	/* kind of sprite */

struct sprite_common_s {
  SPRITE_COMMON_MEMBERS;
};

typedef struct sprite_prog_list_s a_sprite_prog_list; /* see sprprog.c */

struct sprite_prog_s {
  SPRITE_COMMON_MEMBERS;
  a_sprite_prog_list *list;
};

#define SPRITE_RLE_MEMBERS						\
  SPRITE_COMMON_MEMBERS;						\
									\
  /* The code of an rle-sprite is a sequence of				\
       1 a_u8: number m of transparent pixels to skip			\
       1 a_u8: number n of bytes to write				\
       n a_u8: actual bytes to write					\
     The end of a line can be announced using m=0 and n=0,		\
     and the code MUST terminate by m=0 and n=0.			\
  */									\
  a_u8*		code;							\
									\
  /* a pointer the byte right after the end of code,			\
     in order to known where to stop. */				\
  a_u8*		end_code;						\
									\
  /* the number of byte to skip to from the end of a line to the	\
     beginning of the next one */					\
  unsigned int	line_skip

struct sprite_rle_s {
  SPRITE_RLE_MEMBERS;
};

struct sprite_rle_shade_s {
  /* shaded sprites reuse the member of RLE, but the code used
     is extended.  The code of a shaded sprite is a sequence of
       1 a_u8: number m of transparent pixels to skip
       1 a_u8: number n of bytes to write
       n a_u8: actual bytes to write
       1 a_u8: number s of glenz pixels to draw right after
     The end of a line can be announced using m=0, n=0, and s=0,
     and the code MUST terminate by m=0, n=0, and s=0.
  */
  SPRITE_RLE_MEMBERS;
  a_pixel*	glenz;
};

struct sprite_rle_glenz_s {
  /* glenz sprites reuse the member of RLE, but the code used
     is extended.  The code of a shaded sprite is a sequence of
       1 a_u8: number m of transparent pixels to skip
       1 a_u8: number n of glenz pixels to write
     The end of a line can be announced using m=0, and n=0,
     and the code MUST terminate by m=0, and n=0.
  */
  SPRITE_RLE_MEMBERS;
  a_pixel*	glenz;
};

struct sprite_rle_unic_shade_s {
  /* shaded sprites reuse the member of RLE, but the code used
     is extended.  The code of a shaded sprite is a sequence of
       1 a_u8: number m of transparent pixels to skip
       1 a_u8: number n of bytes to write
       1 a_u8: number s of glenz pixels to draw right after
     The end of a line can be announced using m=0, n=0, and s=0,
     and the code MUST terminate by m=0, n=0, and s=0.
  */
  SPRITE_RLE_MEMBERS;
  a_pixel*	glenz;
  a_pixel	color;		/* used to draw opaque pixels */
};

struct sprite_opaque_s {
  SPRITE_COMMON_MEMBERS;
  a_pixel*	data;		/* data to write (concatenated) */
  a_pixel*	end_data;	/* pointer the pixel right after the last
				   in data */
  int		width;		/* number of data to write per line */
  int		line_skip;	/* byte to skip at the end of a line, to
				   jump to the next one. */
};

union sprite_s {
  SPRITE_FIRST_MEMBER;
  struct sprite_common_s	all;
  struct sprite_prog_s		prog;
  struct sprite_rle_s		rle;
  struct sprite_rle_shade_s	shade;
  struct sprite_rle_glenz_s	glenz;
  struct sprite_opaque_s	opaq;
  struct sprite_rle_unic_shade_s unish;
};

/* generic sprite freeing function, this will dispatch to the right
   freeing function */
void free_sprite (a_sprite *sprite);

#define FREE_SPRITE0(x)				\
  do {						\
    if (x) {					\
      free_sprite (x);				\
      x = 0;					\
    }						\
  } while (0)

#endif /* HEROES__SPRITES__H */
