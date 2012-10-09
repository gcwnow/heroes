/*------------------------------------------------------------------------.
| Copyright 1997, 1998, 2000  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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
#include "strack.h"
#include "debugmsg.h"
#include "argv.h"
#include "errors.h"

void
adjust_volume (void)
{
}

void
halve_volume (void)
{
}

int
init_sound_engine (void)
{
  nosound = 1;
  nosfx = 1;
  return 0;
}

void
uninit_sound_engine (void)
{
}

void
load_soundtrack (char *ptr)
{
  (void) ptr;
}

void
unload_soundtrack (void)
{
}

void
play_soundtrack (void)
{
}

void
decode_sound_options (char *option_string, const char *argv0)
{
  (void) option_string;
  (void) argv0;
}

void
print_drivers_list (void)
{
  wmsg (_("Heroes has been compiled without sound support."));
}
