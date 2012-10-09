/*------------------------------------------------------------------.
| Copyright 2002  Alexandre Duret-Lutz <duret_g@epita.fr>           |
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
#include "lvl.h"
#include "error.h"

a_level lvl;

char *program_name = 0;

int
main (int argc, char *argv[])
{
  program_name = argv[0];
  if (argc != 3)
    error (1, 0, "Usage: lvlsave in out");
  printf ("Loading %s...\n", argv[1]);
  lvl_load_file (argv[1], &lvl, true);
  printf ("Saving %s...\n", argv[2]);
  lvl_save_file (argv[2], &lvl);
  lvl_free (&lvl);
  return 0;
}
