/*------------------------------------------------------------------------.
| Copyright 2000, 2001, 2002  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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
#include "readmake.h"
#include "parafmt.h"
#include "fontdata.h"
#include "const.h"
#include "sprtext.h"
#include "sprprog.h"
#include "sprrle.h"
#include "sprglenz.h"
#include "sprglauto.h"
#include "pcx.h"
#include "errors.h"

struct private_read_data_s {
  a_sprite *sprite;
  int line;
  a_private_read_data *next;
};

static a_read_data *
insert_read_data (a_read_data *rd, a_sprite *spr, int line, bool bg)
{
  a_private_read_data *p, *o, **q;
  NEW (a_private_read_data, res);

  if (!rd) {
    NEW (a_read_data, rhd);
    rhd->data_bg = 0;
    rhd->data = 0;
    rhd->max = 0;
    rd = rhd;
  }

  if (rd->max < line)
    rd->max = line;

  res->sprite = spr;
  res->line = line;

  if (bg)
    q = &rd->data_bg;
  else
    q = &rd->data;
  p = *q;

  if (!p || p->line >= line) {
    res->next = p;
    *q = res;
    return rd;
  }

  do {
    o = p;
    p = p->next;
  } while (p && p->line < line);
  o->next = res;
  res->next = p;
  return rd;
}

/* return the head of a string, cut on character ' ' (or '\0' or '}')
   and advance the input pointer to the character after that delimiter
   (unless '\0' is encountered in which case *STR is set to 0). */
static char *
readtok (char **src)
{
  int size;
  char *_src = *src;
  for (size = 0; _src[size] && _src[size] != ' ' && _src[size] != '}'; ++size)
    /* NOP */;
  if (_src[size]) {
    _src[size] = 0;
    *src = _src + size + 1;
  } else
    *src = 0;
  ++size;
  return _src;
}

/* default margins */
#define DEF_LM	5
#define DEF_RM	315

typedef struct a_margin_list_item a_margin_list_item;
struct a_margin_list_item {
  int lm;			    /* left margin */
  int rm;			    /* right margin */
  a_margin_list_item *next;	    /* pointer to next item in list */
};

typedef struct a_margin_list a_margin_list;
struct a_margin_list {
  int glm;			    /* global left margin */
  int lines;			    /* Number of lines we track margins for */
  a_margin_list_item *first, *last;  /* pointers to first and last list item */
};

/* Inititalize margin list and default item.  */
static void
init_margins (a_margin_list *margins)
{
  XMALLOC_VAR (margins->first);
  margins->last = margins->first;
  margins->first->next = 0;
  margins->first->lm = DEF_LM;
  margins->first->rm = DEF_RM;
  margins->lines = 1;
}

/* add (lm, rm) to the list of line margins.  */
static a_margin_list_item*
add_margin (a_margin_list *margins, int lm, int rm)
{
  XMALLOC_VAR (margins->last->next);
  margins->last = margins->last->next;
  margins->last->next = 0;
  margins->last->lm = lm;
  margins->last->rm = rm;
  margins->lines++;
  return margins->last;
}

/* Remove margin list from memory.  */
static void
free_margins (a_margin_list *margins)
{
  while (margins->first) {
    margins->last = margins->first;
    margins->first = margins->first->next;
    free (margins->last);
  }
  margins->first = margins->last = 0;
  margins->lines = 0;
}

/* Set wid with the computed width of each line in margins.  */
static void
compute_widths (a_margin_list *margins, a_width *wid)
{
  a_margin_list_item *m;
  a_width *w;
  if (!margins->first)
    init_margins (margins);
  m = margins->first;
  w = wid;
  while (m) {
    *w++ = m->rm - m->lm - margins->glm;
    m = m->next;
  }
  *w = 0;	/* end of array marker */
}

static void	/* progress one line */
shift_margins (a_margin_list *margins)
{
  a_margin_list_item *m = margins->first;
  margins->first = margins->first->next;
  free (m);
  margins->lines--;
  /* The while loops in compile_reader_data needs this
     for their abort conditions. */
  if (!margins->first)
    init_margins (margins);
}

static void
flag_error (int flag, const char *cmd)
{
  emsg (_("Unknown option '%c' for command '%s'."), flag, cmd);
}

a_read_data *
compile_reader_data (a_read_data *head, const char *str)
{
  a_margin_list margins;
  a_width *wid = 0;		/* array of linewidths (= rm - lm) */
  char *curstr;
  char *curstr_allocated;
  int voffset = 0;
  a_pcx_image help_pics_img;

  pcx_load_from_rsc ("help-pictures-img", &help_pics_img);
  init_margins (&margins);

  while (*str) {
    const char *start;
    enum text_option topt = T_JUSTIFIED;

    margins.glm = 0;

    /* get a line */
    start = str;
    while (*str && *str != '\n')
      ++str;
    if (*str) {
      ++str;
      curstr = CCLONE (start, str - start);
      curstr[str - start - 1] = 0;
    } else
      curstr = xstrdup (start);
    curstr_allocated = curstr;

    /* scan for initial markup */
    while (curstr[0] == '%' && curstr[1] == '{') {
      char *cmd;
      curstr += 2;
      cmd = readtok (&curstr);
      if (!strcmp (cmd, "img")) {
	char *flags = readtok (&curstr);
	int y = atoi (readtok (&curstr));
	int x = atoi (readtok (&curstr));
	int h = atoi (readtok (&curstr));
	int w = atoi (readtok (&curstr));
	int cl = 0;
	bool bg = false;
	bool gl = false;
	bool redgl = false;
	int offset = 0;
	a_margin_list_item *m;
	int i;
	for (; *flags; ++flags) {
	  switch (*flags) {
	  case 'B':
	    bg = true;
	    break;
	  case 'G':
	    gl = true;
	    break;
	  case 'r' :
	    redgl = true;
	    break;
	  case 'C':		/* centered */
	    m = margins.first;
	    offset = margins.glm + m->lm + (m->rm-margins.glm-m->lm-w) / 2;
	    break;
	  case 'R':		/* right */
	    offset = margins.first->rm - w;
	    m = margins.first;
	    if (!bg)
	      for (i = h; i > 0; i -= 10) {
		m->rm = offset;
		if (!(m = m->next))
		  m = add_margin (&margins, DEF_LM, DEF_RM);
	      }
	    break;
	  case 'L':		/* left */
	    offset = margins.glm + margins.first->lm;
	    m = margins.first;
	    if (!bg)
	      for (i = h; i > 0; i -= 10) {
		m->lm = offset - margins.glm + w;
		if (!(m = m->next))
		  m = add_margin (&margins, DEF_LM, DEF_RM);
	      }
	    break;
	  case 'V':
	    offset -= (h - 10) / 2 * xbuf;
	    break;
	  default:
	    flag_error (*flags, "image");
	  }
	}

	while (h>0) {
	  /* split the sprite in strip of 10 pixels heights */
	  a_sprite *s;
	  if (redgl) {
	    s = compile_sprglenz (IMGPOS (help_pics_img, y, x), 0,
				  glenz[6], 10, w, help_pics_img.width, xbuf);
	  } else {
	    s = (gl ? compile_sprglauto : compile_sprrle)
	      (IMGPOS (help_pics_img, y, x), 0,
	       10, w, help_pics_img.width, xbuf);
	  }
	  head = insert_read_data (head, s, offset + voffset + xbuf * cl * 10,
				   bg);
	  h -= 10;
	  y += 10;
	  ++cl;
	}
	/* FIXME: handle glenz images */
      } else if (!strcmp (cmd, "flushleft")) {
	topt = T_FLUSHED_LEFT;
      } else if (!strcmp (cmd, "flushright")) {
	topt = T_FLUSHED_RIGHT;
      } else if (!strcmp (cmd, "center")) {
	topt = T_CENTERED;
      } else if (!strcmp (cmd, "head")) {
	int color = atoi (readtok (&curstr));
	head = insert_read_data (head,
				 compile_sprtext_color (help_font, curstr,
							T_FLUSHED_LEFT
							| T_WAVING, 0, 25),
				 voffset,
				 false);
	head = insert_read_data
	  (head, compile_sprrle (IMGPOS (help_pics_img,
					 69 + color * 12, 0), 0,
				 10, 12, help_pics_img.width, xbuf),
	   voffset + 5, true);
	head = insert_read_data
	  (head, compile_sprglenz (IMGPOS (help_pics_img, 81, 12), 0,
				   glenz[((color - 1) ^ 1) + 2],
				   10, 310 - 12, help_pics_img.width, xbuf),
	   voffset + 5 + 12, true);
	voffset += xbuf * (help_font->line_skip + help_font->height);
	goto next_line;
      } else if (!strcmp (cmd, "textat")) {
	char *flags = readtok (&curstr);
	int y = atoi (readtok (&curstr));
	int x = atoi (readtok (&curstr));
	int pos = voffset + y * xbuf;
	int align = 0;
	for (; *flags; ++flags) {
	  switch (*flags) {
	  case 'L':		/* y,x is the bottom left of text */
	    align |= T_FLUSHED_LEFT;
	    break;
	  case 'R':		/* y,x is the bottom right of text */
	    align |= T_FLUSHED_RIGHT;
	    break;
	  case 'C':		/* y,x is the center of the text */
	    align |= T_CENTERED;
	    break;
	  case 'W':		/* waving text */
	    align |= T_WAVING;
	    break;
	  case 'c':		/* x is relative to the middle of the screen */
	    pos += 320 / 2;
	    break;
	  default:
	    flag_error (*flags, "textat");
	  }
	}
	head = insert_read_data (head,
				 compile_sprtext_color (help_font, curstr,
							align, 0, x),
				 pos, false);
	goto next_line;
      } else {
	if (cmd[0] == '>') {
	  margins.glm += 5 * strlen (cmd);
	}
      }
    }

    /* no text to format? */
    if (!*curstr) {
      do {
	voffset += xbuf * (help_font->line_skip + help_font->height);
	shift_margins (&margins);
      } while (margins.first->lm != DEF_LM || margins.first->rm != DEF_RM);
      goto next_line;
    }

    /* format the paragraph */
    XMALLOC_ARRAY (wid, margins.lines + 1);
    compute_widths (&margins, wid);
    {
      char **p = parafmt_var (curstr, help_font->width, wid,
			      help_font->min_space_width);
      char **l = p;

      while (*l) {
	int offset;
	compute_widths (&margins, wid);

	if (!l[1] && (topt & T_JUSTIFIED) == T_JUSTIFIED)
	  topt = T_FLUSHED_LEFT;

	switch (topt) {
	case T_FLUSHED_LEFT:
	case T_JUSTIFIED:
	  offset = margins.glm + margins.first->lm;
	  break;
	case T_FLUSHED_RIGHT:
	  offset = margins.first->rm;
	  break;
	case T_CENTERED:
	  offset = margins.glm + margins.first->lm +
	    (margins.first->rm - margins.glm - margins.first->lm) / 2;
	  break;
	default:
	  assert (0);
	}

	head = insert_read_data (head,
				 compile_sprtext_color (help_font, *l,
							topt, *wid, offset),
				 voffset, false);
	voffset += xbuf * (help_font->line_skip + help_font->height);
	++l;
	shift_margins (&margins);
      }
      free (wid);
      free_pararray (p);

      while (margins.first->lm != DEF_LM || margins.first->rm != DEF_RM) {
	voffset += xbuf * (help_font->line_skip + help_font->height);
	shift_margins (&margins);
      }
    }
  next_line:
    free (curstr_allocated);
  }
  free_margins (&margins);
  img_free (&help_pics_img);
  return head;
}

void
free_reader_data (a_read_data *rd)
{
  a_private_read_data *p = rd->data;
  while (p) {
    a_private_read_data *n = p->next;
    free_sprite (p->sprite);
    free (p);
    p = n;
  }
  p = rd->data_bg;
  while (p) {
    a_private_read_data *n = p->next;
    free_sprite (p->sprite);
    free (p);
    p = n;
  }
  free (rd);
}

void
draw_reader_data (const a_read_data *rd, a_pixel *dest, int min, int max)
{
  const a_private_read_data *p;
  min *= xbuf;
  max *= xbuf;
  /* draw the background */
  p = rd->data_bg;
  while (p && p->line < min)
    p = p->next;
  while (p && p->line <= max) {
    DRAW_SPRITE (p->sprite, dest + p->line - min);
    p = p->next;
  }
  /* draw the foreground */
  p = rd->data;
  while (p && p->line < min)
    p = p->next;
  while (p && p->line <= max) {
    DRAW_SPRITE (p->sprite, dest + p->line - min);
    p = p->next;
  }

}
