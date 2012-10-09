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
#include "const.h"
#include "debugmsg.h"
#include "fontdata.h"
#include "errors.h"

a_fontdata *edit_font = 0;
a_fontdata *menu_font = 0;
a_fontdata *help_font = 0;
a_fontdata *deck_font = 0;
a_fontdata *bonus_font = 0;
static a_pcx_image help_font_img;
static a_pcx_image big_font_img;

static void
initialize_menu_font (void)
{
  a_pixel *upl;			/* upper left pixel of the character */
  int ch;			/* current character */

  pcx_load_from_rsc ("big-font", &big_font_img);

  XCALLOC_VAR (menu_font);
  menu_font->height = 10;
  menu_font->line_skip = 2;
  menu_font->line_size = big_font_img.width;

  for (ch = ' '; ch <= 255; ++ch) {
    unsigned int width, act_width;
    unsigned int height;

    upl = big_font_img.buffer +
      ((int) (ch - ' ') % 16) * 22 +
      ((int) (ch - ' ') / 16) * big_font_img.width * menu_font->height;

    /* detect the width of a character */
    for (act_width = width = 0; width < 22; ++width)
      for (height = 0; height < menu_font->height; ++height) {
	if (upl[width + height * big_font_img.width] != 0) {
	  act_width = width + 1;
	  break;
	}
      }
    menu_font->upper_left[ch] = upl;
    menu_font->width[ch] = act_width;
  }
  menu_font->width[' '] = 5;
  menu_font->min_space_width = 3;
}

static void
initialize_deck_font (void)
{
  a_pixel *upl;			/* upper left pixel of the character */
  int ch;			/* current character */

  XCALLOC_VAR (deck_font);
  deck_font->height = 5;
  deck_font->line_skip = 1;
  deck_font->line_size = font_deck_img.width;

  for (ch = ' '; ch <= '^'; ++ch) {
    unsigned int width, act_width;
    unsigned int height;

    upl = font_deck_img.buffer + 2 * font_deck_img.width +
      ((int) (ch - ' ') % 32) * 8 +
      ((int) (ch - ' ') / 32) * font_deck_img.width * 8;

    /* detect the width of a character */
    for (act_width = width = 0; width < 8; ++width)
      for (height = 0; height < deck_font->height; ++height) {
	if (upl[width + height * font_deck_img.width] != 0) {
	  act_width = width + 2;
	  break;
	}
      }
    deck_font->upper_left[ch] = upl;
    deck_font->width[ch] = act_width;
  }

  deck_font->width[' '] = 4;
  deck_font->min_space_width = 2;

  /* Lower case letters are not supported, but since this font is used
     to draw user-supplied string, it's better to convert lower case
     characters to upper.  */
  for (ch = 'a'; ch <= 'z'; ++ch)
    if (deck_font->width[ch] == 0)
      {
	deck_font->upper_left[ch] = deck_font->upper_left[TOUPPER(ch)];
	deck_font->width[ch] = deck_font->width[TOUPPER(ch)];
      }
}

static void
initialize_bonus_font (void)
{
  a_pixel *upl;			/* upper left pixel of the character */
  int ch;			/* current character */

  XCALLOC_VAR (bonus_font);
  bonus_font->height = 12;
  bonus_font->line_skip = 2;
  bonus_font->line_size = bonus_font_img.width;

  for (ch = ' '; ch <= 'd'; ++ch) {
    unsigned int width, act_width;
    unsigned int height;

    upl = bonus_font_img.buffer +
      ((int) (ch - ' ') % 26) * 12 +
      ((int) (ch - ' ') / 26) * bonus_font_img.width * bonus_font->height;

    /* detect the width of a character */
    for (act_width = width = 0; width < 12; ++width)
      for (height = 0; height < bonus_font->height; ++height) {
	if (upl[width + height * bonus_font_img.width] != 0) {
	  act_width = width + 1;
	  break;
	}
      }
    bonus_font->upper_left[ch] = upl;
    bonus_font->width[ch] = act_width;
  }
  bonus_font->width[' '] = 5;
  bonus_font->min_space_width = 2;
}

static void
initialize_help_font (void)
{
  a_pixel *upl;			/* upper left pixel of the character */
  int ch;			/* current character */

  pcx_load_from_rsc ("help-font", &help_font_img);

  XCALLOC_VAR (help_font);
  help_font->height = 9;
  help_font->line_skip = 1;
  help_font->line_size = help_font_img.width;

  for (ch = ' '; ch <= 255; ++ch) {
    unsigned int width, act_width;
    unsigned int height;

    upl = help_font_img.buffer +
      ((int) (ch - ' ') % 32) * 8 +
      ((int) (ch - ' ') / 32) * help_font_img.width * help_font->height;

    /* detect the width of a character */
    for (act_width = width = 0; width < 8; ++width)
      for (height = 0; height < help_font->height; ++height) {
	if (upl[width + height * help_font_img.width] != 0) {
	  act_width = width + 1;
	  break;
	}
      }
    help_font->upper_left[ch] = upl;
    help_font->width[ch] = act_width;
  }
  help_font->width[' '] = 4;
  help_font->min_space_width = 2;
}

void
init_fonts (void)
{
  dmsg (D_MISC, "initializing font data");

  initialize_menu_font ();
  edit_font = 0;
  initialize_help_font ();
  initialize_deck_font ();
  initialize_bonus_font ();
}

void
uninit_fonts (void)
{
  dmsg (D_MISC, "uninitializing font data");

  XFREE0 (menu_font);
  XFREE0 (edit_font);
  XFREE0 (help_font);
  XFREE0 (deck_font);
  XFREE0 (bonus_font);
  img_free (&help_font_img);
  img_free (&big_font_img);
}

unsigned int
compute_text_width (const a_fontdata *font, const char *text,
		    unsigned int *nspaces)
{
  unsigned int width = 0;
  unsigned int ns = 0;

  for (; *text; ++text) {
    if (*text == '%') {
      ++text;
      if (*text != '%')
	continue;
    }
    if (!nspaces || *text != ' ')
      width += font->width[UCHAR (*text)];
    else
      ++ns;
  }
  if (nspaces)
    *nspaces = ns;
  return width;
}


#if DEBUG
void
check_message_is_drawable (const a_fontdata *font, const char *text)
{
  const char *text_orig = text;

  for (; *text; ++text) {
    if (font->width[UCHAR (*text)] == 0) {
      wmsg (_("character '%c' is not drawable (in message '%s')"),
	    *text, text_orig);
      break;
    }
  }
}
#endif
