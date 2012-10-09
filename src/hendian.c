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

#include "system.h"
#include "structs.h"
#include "hendian.h"
#include "bytesex.h"

void
bswap_level_header (a_level_header* level_info)
{
  level_info->xt = BSWAP32 (level_info->xt);
  level_info->yt = BSWAP32 (level_info->yt);
  level_info->xwrap = BSWAP32 (level_info->xwrap);
  level_info->ywrap = BSWAP32 (level_info->ywrap);
  level_info->start[0] = BSWAP32 (level_info->start[0]);
  level_info->start[1] = BSWAP32 (level_info->start[1]);
  level_info->start[2] = BSWAP32 (level_info->start[2]);
  level_info->start[3] = BSWAP32 (level_info->start[3]);
}

void
bswap_level_tiles (const a_level_header* level_info, a_tile* level_map)
{
  int i;

  for (i = level_info->xt * level_info->yt - 1; i >= 0; i--) {
    level_map[i].number = BSWAP32 (level_map[i].number);
    level_map[i].sprite = BSWAP16 (level_map[i].sprite);
    if (level_map[i].type == t_tunnel)
      level_map[i].info.tunnel.output =
	BSWAP32 (level_map[i].info.tunnel.output);
  }
}
