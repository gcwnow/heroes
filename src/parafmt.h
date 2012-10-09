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

/*-------------------------------------------------.
| This is a "smart" paragraph formating function.  |
`-------------------------------------------------*/

#ifndef HEROES__PARAFMT__H
#define HEROES__PARAFMT__H

/* The width of a string, or of a character */
typedef unsigned int a_width;

/* Split the input string STR into an array of strings the sizes of
   which are never longer than MAX_WIDTH.  Size of strings are
   computed according to WA, and array of widths for wach of the 256
   characters.  The returned array is mallocated and
   should be freed by the caller.

   Strings are broke on spaces, with a preference for spaces which follow
   punctuation.  paramfmt will consider using spaces greater or equal
   to min_space_width, trying to use the value of wa[' '] ad much as
   possible.
*/
char **parafmt (const char *str, const a_width *wa,
		a_width max_width, a_width min_space_width);

/* Likewise, but the width may be different on each line.
   max_widths is a 0 terminated array giving the width of each line.
   The last size given by max_width is used for the remaining lines,
   should there be more lines than widths given.  */
char **parafmt_var (const char *str, const a_width *wa,
		    const a_width *max_widths, a_width min_space_width);

/* free an array of strings, as returned by parafmt */
void free_pararray (char **p);

#endif /* HEROES__PARAFMT__H */
