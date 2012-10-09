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

/*----------------------------------------------------------.
| This file defines data used by the renderering function.  |
`----------------------------------------------------------*/

#ifndef HEROES__RENDERDATA__H
#define HEROES__RENDERDATA__H

#include "video.h"
#include "pcx.h"
#include "sprite.h"
#include "lvl.h"

/* data for background tiles */

typedef struct {
  const a_pixel *source;	/* address of the tile's image */
  an_anim_kind kind;		/* kind of animation */
  int anim_speed;		/* speed for animated tiles */
  int anim_frames;		/* number of frames in an animation */
} a_bg_data;

extern a_bg_data *bg_data;

/* data for foreground sprites (trees...) and bonuses, these are kept
   in the same struct because they are drawn in the same loop */

typedef struct {
  a_sprite *sprite;		/* sprite to superpose if non nil */
  a_sprite **bonus;		/* bonus line, if non nil */
  char big_dollar;		/* 1 if a big dollar must be drawn */
} a_fg_data;

extern a_fg_data *fg_data;

extern void init_render_data (void);
extern void uninit_render_data (void);

#endif /* HEROES__RENDERDATA__H */
