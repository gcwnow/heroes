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

/*-------------------------------------------------------------------.
|   This is a "smart" paragraph formating function.  The algorithm   |
| used here is roughly the same as in the command `fmt' from the GNU |
| fileutils except that here we have to handle spaces of variable    |
| width, letters that does not all have the same width, and lines of |
| differents width.                                                  |
`-------------------------------------------------------------------*/

#include "system.h"
#include "parafmt.h"

typedef struct word_s		a_word;
typedef struct paragraph_s	a_paragraph;
typedef a_u32			a_cost;
#define MAX_COST		U32_MAX

struct word_s {
  char		*letters;	/* positions in the string copy */
  char		*letters_end;	/* end of the word in the string copy */
  a_width	width;		/* width of the word */
  unsigned int	spaces;		/* number of spaces after the word */
  bool		is_punct;	/* if the word it is ended by a
				   punctuation mark */
  const a_word	**next_break;	/* Next word to break after, if
				   this one is broken after.  This is an array
				   because we compute the next_break for
				   any width of line possible. */
  a_cost	*next_break_cost; /* Cost for this next break, an array for
				     the very same reason. */
};

struct paragraph_s {
  a_word	*words;		/* The list of words for the paragraph. */
  unsigned int	nwords;		/* The number of words. */
  unsigned int	indent;		/* Initial spaces, before the first word. */
  const a_width	*max_widths;	/* like line_widths but sorted in ascending
				   order, duplicate removed */
  unsigned int	nmax_widths;	/* size of max_width (= number of
				   expected lines).  If there is more
				   line than nmax_widths, the last value
				   of max_width (namely
				   max_width[nmax_width - 1]) will be used. */
  a_width	std_space_width; /* standard space width */
  a_width	min_space_width; /* minimun space_width */
  const a_word	*first_break;	/* first word to break after */
  char		*data;		/* a copy of the original string */
  const a_word	**break_data;	/* a buffer allocated once for

			   break pointers of all words */
  a_cost	*break_cost_data; /* a buffer allocated once for
				     break costs of all words */
};

/* count the number of words in STR.
   Words are sequences of non-space characters */
static unsigned int
count_words (const char *str)
{
  unsigned int count = 0;

  for (;;) {
    /* skip initial spaces */
    while (*str && ISSPACE (*str))
      ++str;
    /* at end of string? */
    if (!*str)
      break;
    ++count;
    /* skip the word */
    while (*str && !ISSPACE (*str))
      ++str;
  }
  return count;
}

/* Split STR in an array of words.  Return this as a a_paragraph. */
static a_paragraph *
split_words (const char *input_str, const a_width *wa)
{
  unsigned int nwords;
  a_word *words;
  unsigned int i;
  char *str;
  NEW (a_paragraph, p);

  /* count initial spaces */
  for (i = 0; *input_str && ISSPACE (*input_str); ++input_str)
    ++i;
  p->indent = i;

  /* duplicate the string, without the leading spaces */
  p->data = str = xstrdup (input_str);

  /* allocate the words array */
  nwords = count_words (str);
  XMALLOC_ARRAY (words, nwords);
  p->nwords = nwords;
  p->words = words;

  /* fill the words array */
  for (i = 0; i < nwords; ++i) {
    words[i].letters = str;

    /* compute the a_width of the word
       (previous spaces have been skipped, and *str!=0 because i<nwords) */
    {
      a_width w = 0;
      do {
	/* skip possible %x or %{xxx} */
	if (*str == '%') {
	  if (str[1] == '%')
	    ++str;
	  else /* if (str[1] == '{') {
	    str = strchr (str, '}');
	    assert (str);
	    ++str;
	  } else */
	    str += 2;
	  if (!*str || ISSPACE(*str))
	    break;
	}

	w += wa[UCHAR (*str)];
	++str;
      } while (*str && !ISSPACE (*str));
      words[i].width = w;
    }

    /* record the end */
    words[i].letters_end = str;

    /* mark the word if it is ended by punctuatuion.
       The do/while above has run at least once so str[-1] is ok. */
    words[i].is_punct = !!ISPUNCT (str[-1]);

    /* count the spaces following current word */
    {
      int j;
      for (j = 0; *str && ISSPACE (*str); ++str)
	++j;
      words[i].spaces = j;
    }
  }

  return p;
}

/* initialize data related to line widths */
static a_paragraph *
initialize_width_data (a_paragraph *p, const a_width *line_widths)
{
  unsigned int nwidths = 0;
  p->max_widths = line_widths;

  /* compute the muber of line widths given */
  for (; *line_widths; ++line_widths)
    ++nwidths;
  p->nmax_widths = nwidths;

  /* allocate the break buffers */
  XMALLOC_ARRAY (p->break_data, p->nmax_widths * p->nwords);
  XMALLOC_ARRAY (p->break_cost_data, p->nmax_widths * p->nwords);
  /* distribute amounts of these buffers to all words */
  {
    unsigned int n;
    const a_word **w = p->break_data;
    a_cost *c = p->break_cost_data;
    for (n = 0; n < p->nwords; ++n) {
      p->words[n].next_break = w;
      p->words[n].next_break_cost = c;
      w += p->nmax_widths;
      c += p->nmax_widths;
    }
  }

  return p;
}

static a_cost
wdist (a_width l, a_width r)
{
  a_cost c = l > r ? l - r : r - l;
  return c * c;
}

/* Give the cost for breaking after word WN on a line
   which is WIDTH large and has SPACES spaces.
   You should play with the formulas here to give
   a better looking to the paragraph formating.
   WIDX is tha width index of the current line.
*/
static a_cost
compute_break_cost (a_paragraph *p, unsigned int wn,
		    a_width width, a_width spaces, unsigned widx)
{
  a_cost cost = 0;
  a_width space_width, std_space_width;

  /* width for spaces */
  space_width = p->max_widths[widx] - width;

  /* prefered width for spaces */
  std_space_width = p->std_space_width * spaces;

  /* prefer the standard space width */
  cost += wdist (space_width, std_space_width);

  /* prefer breaking on punctuation */
  if (!p->words[wn].is_punct)
    cost += p->std_space_width * 3;

  /* account for the cost of following breaks */
  if (widx + 1 < p->nmax_widths)
    cost += p->words[wn].next_break_cost[widx + 1];
  else
    cost += p->words[wn].next_break_cost[p->nmax_widths - 1];
  return cost;
}

/* Find the best break for a line starting on word wn, and with width
   max_widths[widx].  Update res->next_break and res->next_break_cost
   accordingly.  WARNING: this does not handle words larger than
   p->max_width */
static const a_word *
compute_best_break (a_paragraph *p, unsigned int wn, a_word *res,
		    unsigned int widx)
{
  unsigned int nwords = p->nwords;
  const a_word *w = p->words;
  unsigned int spaces = 0;
  a_width width = 0;

  a_cost best_cost = MAX_COST;
  const a_word *best_break = 0;

  /* find the best break position on this line */
  while (wn < nwords) {
    a_cost cost;

    /* Add a new word */
    width += w[wn].width;

    /* don't check more words than what can fit on this line */
    if ((width + spaces * p->min_space_width) > p->max_widths[widx])
      break;

    /* consider breaking after this word */
    cost = compute_break_cost (p, wn, width, spaces, widx);
    if (cost < best_cost) {
      best_cost = cost;
      best_break = w + wn;
    }

    /* advance to next word */
    spaces += w[wn].spaces;
    ++wn;
  }

  /* update res accordingly */
  if (res) {
    res->next_break[widx] = best_break;
    res->next_break_cost[widx] = (wn >= nwords) ? 0 : best_cost;
  }
  return best_break;
}

/* compute breaking paths for the paragraph P */
static void
compute_breaking_path (a_paragraph *p)
{
  a_word *w = p->words;
  unsigned int wn;
  unsigned int widx;

  for (wn = p->nwords; wn > 0; --wn) {
    /* consider breaking right before w[wn] */
    for (widx = 0; widx < p->nmax_widths; ++widx)
      compute_best_break (p, wn, w + wn - 1, widx);
  }

  /* finally, compute the best break for the first line */
  /* FIXME: p->indent should be dealed with somewhere*/
  p->first_break = compute_best_break (p, 0, 0, 0);
}

/* follow the (already computed) breaking path,
   and build and array of strings (one per line) */
static char **
convert_paragraph_to_array (a_paragraph *p)
{
  unsigned int nlines;
  const a_word *fw;		/* first word of the current line */
  const a_word *lw;		/* last word of the current line */
  char **result;
  unsigned int curline;

  /* count the number of lines */
  lw = p->first_break;
  nlines = 0;
  while (lw) {
    ++nlines;
    lw = lw->next_break[nlines < p->nmax_widths ? nlines : p->nmax_widths - 1];
  }

  XMALLOC_ARRAY (result, nlines + 1);
  result[nlines] = 0;		/* mark the end of the array */

  fw = p->words;
  lw = p->first_break;
  curline = 0;
  while (lw) {
    /* copy the words for the current line */
    *lw->letters_end = 0;	/* split the string */
    result[curline] = fw->letters;
    /* jump to next line */
    ++curline;
    fw = lw + 1;
    lw = lw->next_break[curline < p->nmax_widths
		       ? curline : p->nmax_widths - 1];
  }
  assert (curline == nlines);
  return result;
}

char **
parafmt_var (const char *str, const a_width *wa,
	     const a_width *max_widths, a_width min_space_width)
{
  char **result;
  a_paragraph *p = split_words (str, wa);
  /* FIXME: make sure no words are larger than any max_width or
     compute_breaking_path may fail. */
  initialize_width_data (p, max_widths);
  p->min_space_width = min_space_width;
  p->std_space_width = wa[' '];
  compute_breaking_path (p);
  result = convert_paragraph_to_array (p);
  free (p->words);
  free (p->break_data);
  free (p->break_cost_data);
  free (p);
  return result;
}

char **
parafmt (const char *str, const a_width *wa,
	 a_width max_width, a_width min_space_width)
{
  a_width mw[2] = {max_width, 0};
  return parafmt_var (str, wa, mw, min_space_width);
}

void
free_pararray (char **p)
{
  free (p[0]);			/* this free ALL lines (p[0] is
				   the ->data member of the original
				   paragraph). */
  free (p);
}

#ifdef TEST
/*-----------------------------------------------------------------.
| Run "make parafmt-check" in the src/ directory to compile this.  |
`-----------------------------------------------------------------*/

static char **
print_array (char **array, a_width width)
{
  char spec[30];
  char **a = array;
  sprintf (spec, "%%-%us|\n", width);
  for (; *a; ++a)
    printf (spec, *a);
  return array;
}

static char **
print_array_var (char **array, const a_width *widths)
{
  char spec[30];
  char **a = array;
  a_width width = *widths;
  for (; *a; ++a) {
    sprintf (spec, "%%-%us|\n", width);
    printf (spec, *a);
    if (widths[1])
      width = *++widths;
  }
  return array;
}

static void
check_parafmt (a_width width)
{
  unsigned int test_widths[256];
  const char *test_str = "\
This is a quit longish string, used to test whether parafmt format \
it correctly.  Blah blah, blah blah.  You can check that break on \
punctiation (non alaphanumeric, non spaces) are sometime prefered.";
  int i;
  for (i = 0; i < 256; ++i)
    test_widths[i] = 1;
  free_pararray (print_array (parafmt (test_str, test_widths, width, 1),
			      width));
}

static void
check_parafmt_var (a_width width)
{
  unsigned int test_widths[256];
  const char *test_str = "\
This is another testing string.  But this time we check the \
parafmt_var function.  It is used to format paragraphs whose lines \
do not always have the same width.  Blah blah, blah blah.";
  const a_width w[] = { width, width + 2, width + 4, width + 6, 0 };
  int i;
  for (i = 0; i < 256; ++i)
    test_widths[i] = 1;
  free_pararray (print_array_var (parafmt_var (test_str, test_widths, w, 1),
				  w));
}

char *program_name;

int
main (void)
{
  int i;
  for (i = 20; i < 80; ++i)
    check_parafmt (i);
  for (i = 20; i < 70; ++i)
    check_parafmt_var (i);
  return 0;
}
#endif
