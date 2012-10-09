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
#include "sprite.h"
#include "sprrle.h"
#include "sprprog.h"
#include "sprzcol.h"
#include "sprshade.h"
#include "sprglenz.h"
#include "spropaque.h"
#include "sprunish.h"
#include "sprglauto.h"

void
free_sprite (a_sprite* sprite)
{
  if (!sprite)
    return;

  /* dispatch */
  switch (sprite->all.kind) {
  case S_OPAQUE:
    free_spropaque (sprite);
    break;
  case S_RLE:
    free_sprrle (sprite);
    break;
  case S_RLE_ZCOL:
    free_sprzcol (sprite);
    break;
  case S_RLE_SHADE:
    free_sprshade (sprite);
    break;
  case S_RLE_GLENZ:
    free_sprglenz (sprite);
    break;
  case S_RLE_GLENZ_AUTO:
    free_sprglauto (sprite);
    break;
  case S_RLE_UNIC_SHADE:
    free_sprunish (sprite);
    break;
  case S_PROG:
  case S_PROG_WAV:
    free_sprprog (sprite);
    break;
  }
}
