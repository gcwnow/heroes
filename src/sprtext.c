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

#include "system.h"
#include "sprtext.h"
#include "sprprogwav.h"
#include "sprrle.h"
#include "sprunish.h"
#include "const.h"
#include "parafmt.h"

/*
 * Generate a a_sprite that display a text, using a given font.
 *
 * Currently, an S_RLE sprite is computed for each character and chained
 * together in a S_PROG sprite.  This can be enhanced: unless waving,
 * there is no point in computing a S_RLE for each separate character,
 * it may be better to compute *one* S_RLE for the *whole* string.
 */

a_sprite *
compile_sprtext (const a_fontdata *font, const char *text,
		 enum text_option topt, unsigned int maxwidth,
		 int offset)
{
  unsigned int nspaces;
  unsigned int text_width = 0;

  check_message_is_drawable (font, text);

  if (topt & T_FLUSHED_LEFT) {	/* FLUSHED_LEFT or JUSTIFIED */
    if (topt & T_FLUSHED_RIGHT) { /* JUSTIFIED */
      text_width = compute_text_width (font, text, &nspaces);
      if (nspaces == 0) {	/* we cannot justify a text without spaces */
	topt &= ~T_JUSTIFIED;
	topt |= T_FLUSHED_LEFT;
      }
    }
  } else {			/* FLUSHED_RIGHT or CENTERED */
    text_width = compute_text_width (font, text, 0);

    if (topt & T_FLUSHED_RIGHT)	/* FLUSHED_RIGHT */
      offset -= (int) text_width;
    else			/* CENTERED */
      offset -= (int) text_width/2;
  }

  new_sprprog ();

  for (; *text; ++text) {
    if (*text == ' ') {
      if ((topt & T_JUSTIFIED) == T_JUSTIFIED) {
	unsigned int off = (maxwidth - text_width) / nspaces;
	offset += off;
	text_width += off;
	--nspaces;
      } else
	offset += font->width[' '];
    } else {
      /* ensure that the letter actually exists before drawing it */
      if (font->width[UCHAR (*text)])
	add_sprprog (compile_sprrle (font->upper_left[UCHAR (*text)], 0,
				     font->height, font->width[UCHAR (*text)],
				     font->line_size, xbuf),
		     offset);
      offset += font->width[UCHAR (*text)];
    }
  }

  if (topt & T_WAVING)
    return end_sprprogwav ();
  else
    return end_sprprog ();
}


a_sprite *
compile_sprtext_color (const a_fontdata *font, const char *text,
		       enum text_option topt, unsigned int maxwidth,
		       int offset)
{
  unsigned int nspaces;
  unsigned int text_width = 0;
  unsigned int colors[] = { 255, 111, 127, 143, 159, 16 };
  unsigned int c = colors[0];

  check_message_is_drawable (font, text);

  if (topt & T_FLUSHED_LEFT) {	/* FLUSHED_LEFT or JUSTIFIED */
    if (topt & T_FLUSHED_RIGHT) { /* JUSTIFIED */
      text_width = compute_text_width (font, text, &nspaces);
      if (nspaces == 0) {	/* we cannot justify a text without spaces */
	topt &= ~T_JUSTIFIED;
	topt |= T_FLUSHED_LEFT;
      }
    }
  } else {			/* FLUSHED_RIGHT or CENTERED */
    text_width = compute_text_width (font, text, 0);

    if (topt & T_FLUSHED_RIGHT)	/* FLUSHED_RIGHT */
      offset -= (int) text_width;
    else			/* CENTERED */
      offset -= (int) text_width/2;
  }

  new_sprprog ();

  for (; *text; ++text) {
    if (*text == ' ') {
      if ((topt & T_JUSTIFIED) == T_JUSTIFIED) {
	unsigned int off = (maxwidth - text_width) / nspaces;
	offset += off;
	text_width += off;
	--nspaces;
      } else
	offset += font->width[' '];
    } else if (*text == '%') {
      ++text;
      switch (*text) {
      case 'w':
	c = colors[0];
	break;
      case 'p':
	c = colors[1];
	break;
      case 'y':
	c = colors[2];
	break;
      case 'b':
	c = colors[3];
	break;
      case 'g':
	c = colors[4];
	break;
      case 'r':
	c = colors[5];
	break;
      default:
	goto compile_letter;
      }
    } else {
    compile_letter:
      /* ensure that the letter actually exists before drawing it */
      if (font->width[UCHAR (*text)])
	add_sprprog (compile_sprunish (font->upper_left[UCHAR (*text)], 0,
				       82, glenz[0], c,
				       font->height,
				       font->width[UCHAR (*text)],
				       font->line_size, xbuf),
		     offset);
      offset += font->width[UCHAR (*text)];
    }
  }

  if (topt & T_WAVING)
    return end_sprprogwav ();
  else
    return end_sprprog ();
}


a_sprite *
compile_sprpara (const a_fontdata *font, const char *text,
		 enum text_option topt, unsigned int maxwidth,
		 int offset)
{
  char **p = parafmt (text, font->width, maxwidth, font->min_space_width);
  char **l = p;
  unsigned voffset = 0;

  new_sprprog ();
  while (*l) {
    add_sprprog (compile_sprtext (font, *l, topt, maxwidth, offset), voffset);
    voffset += xbuf * (font->line_skip + font->height);
    ++l;
  }
  free_pararray (p);
  return end_sprprog ();
}

a_sprite*
compile_menu_text (const char *text, enum text_option topt,
		   int row, int col)
{
  return compile_sprtext (menu_font, text, topt, 0, row * xbuf + col);
}

a_sprite *
compile_menu_para (const char *text, enum text_option topt,
		   int row, int col, int maxwidth)
{
  return compile_sprpara (menu_font, text, topt, maxwidth , row * xbuf + col);
}
a_sprite*
compile_deck_text (const char *text, enum text_option topt,
		   int row, int col)
{
  return compile_sprtext (deck_font, text, topt, 0, row * xbuf + col);
}

a_sprite *
compile_bonus_text (const char *text, enum text_option topt,
		    int row, int col)
{
  return compile_sprtext (bonus_font, text, topt, 0, row * xbuf + col);
}
