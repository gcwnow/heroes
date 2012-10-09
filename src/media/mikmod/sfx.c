/*------------------------------------------------------------------.
| Copyright 1997, 1998, 2000, 2001  Alexandre Duret-Lutz            |
|                                    <duret_g@epita.fr>             |
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
#include <mikmod.h>
#include "sfx.h"
#include "prefs.h"

void *
load_sfx_low (char *file)
{
  struct SAMPLE *tmp;

  tmp = Sample_Load (file);
  if (tmp)
    tmp->panning = (PAN_RIGHT + PAN_LEFT) / 2;
  return tmp;
}

void
free_sfx_low (void *sfx)
{
  Sample_Free (sfx);
}

void
play_sfx_low (void *sfx)
{
  struct SAMPLE *tmp = sfx;
  /* set the sample volume */
  tmp->volume = (13 - opt.sfx_volume) * 64 / 13;
  Sample_Play (tmp, 0, 0);
}
