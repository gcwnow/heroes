/*------------------------------------------------------------------------.
| Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
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

#ifndef HEROES__FONTDATA__H
#define HEROES__FONTDATA__H

#include "video.h"

typedef struct {
  unsigned int	width[256];	/* width of each character */
  unsigned int	height;		/* height common to each character */
  a_pixel*	upper_left[256]; /* upper left pixel of each character.
				    DO NOT use this pointer if the
				    corresponding width is null. */
  unsigned int	line_size;	/* size of a line in the font buffer */
  unsigned int	line_skip;	/* number of pixel to skip verticaly
				   between lines, height is added. */
  unsigned int	min_space_width; /* minimum width allowed for space */
} a_fontdata;

extern a_fontdata *edit_font;
extern a_fontdata *menu_font;
extern a_fontdata *help_font;
extern a_fontdata *deck_font;
extern a_fontdata *bonus_font;

void init_fonts (void);
void uninit_fonts (void);

/* Compute the width needed by `text' using font `font'.
   If nspaces is non null spaces are not accounted in
   the returned width but *NSPACES is set to the number of
   space encountered. */
unsigned int compute_text_width (const a_fontdata *font, const char *text,
				 unsigned int *nspaces);

#if DEBUG
void check_message_is_drawable (const a_fontdata *font, const char *text);
#else
# define check_message_is_drawable(x, y)  ((void *) 0)
#endif

#endif /* HEROES__FONTDATA__H */
