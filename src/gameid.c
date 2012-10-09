/*------------------------------------------------------------------.
| Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
|                                                                   |
| This file is part of Heroes.                                      |
|                                                                   |
| Heroes is free software; you can redistribute it and/or modify it |
| under the terms of the GNU General Public License version 2 as    |
| published by the Free Software Foundation.                        |
|                                                                   |
| Heroes is distributed in the hope that it will be useful, but     |
| WITHOUT ANY WARRANTY; without even the implied warranty of        |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU |
| General Public License for more details.                          |
|                                                                   |
| You should have received a copy of the GNU General Public License |
| along with this program; if not, write to the Free Software       |
| Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA          |
| 02111-1307 USA                                                    |
`------------------------------------------------------------------*/

#include "system.h"
#include "gameid.h"

void
create_gameid (gameid_ptr gid)
{
  int i;
  for (i = 0; i < GAMEID_SIZE; ++i)
    gid[i] = (a_u32) rand ();
}

void
empty_gameid (gameid_ptr gid)
{
  int i;
  for (i = 0; i < GAMEID_SIZE; ++i)
    gid[i] = 0;
}

bool
equal_gameid (const gameid_ptr gid1, const gameid_ptr gid2)
{
  int i;
  for (i = 0; i < GAMEID_SIZE; ++i)
    if (gid1[i] != gid2[i])
      return false;
  return true;
}

void
copy_gameid (gameid_ptr gid1, const gameid_ptr gid2)
{
  int i;
  for (i = 0; i < GAMEID_SIZE; ++i)
    gid1[i] = gid2[i];
}

char *
gameid_to_text (const gameid_ptr gid)
{
  int i;
  char *result;
  XMALLOC_ARRAY (result, GAMEID_SIZE * 8 + 1);

  for (i = 0; i < GAMEID_SIZE; ++i)
    sprintf (result + i * 8, "%08X", gid[i]);

  return result;
}

bool
text_to_gameid (const char *src, gameid_ptr gid)
{
  int i;
  int j;
  for (i = 0; i < GAMEID_SIZE; ++i) {
    gid[i] = 0;
    for (j = 8; j != 0; --j) {
      int c = toupper (*src++);
      gid[i] <<= 4;
      if (c >= '0' && c <= '9')
	gid[i] += c - '0';
      else if (c >= 'A' && c <= 'F')
	gid[i] += c - 'A' + 10;
      else
	return true;
    }
  }
  return false;
}
