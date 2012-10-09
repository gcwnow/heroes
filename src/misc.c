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
#include "misc.h"

char *
strlwr (char *in)
{
  char *tmp = in;
  for (; *in != 0; ++in)
    *in = tolower (*in);
  return tmp;
}

char *
strupr (char *in)
{
  char *tmp = in;
  for (; *in != 0; ++in)
    *in = toupper (*in);
  return tmp;
}

char *
strcat_alloc (const char *fst, const char *snd)
{
  char *res = xmalloc (strlen (fst) + strlen (snd) + 1);
  return res ? strcat (strcpy (res,fst), snd) : NULL;
}

char *
strappend (char *fst, const char *snd)
{
  char *res = xrealloc (fst, strlen (fst) + strlen (snd) + 1);
  return res ? strcat (res, snd) : NULL;
}

char *
chomp (char *str)
{
  char *old = str;
  while (*str != 0 && *str != '\n' && *str != '\r')
    ++str;
  *str = 0;
  return old;
}
