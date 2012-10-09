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

#ifndef HEROES__RSC_FILES_HASH__H
#define HEROES__RSC_FILES_HASH__H

struct rsc_file {
  const char *name;
  const char *value;
  char *modified_value;
  int expanded;

  /* Whether the value has been set from a secure system-wide file.  */
  bool secure;
};

struct rsc_file *in_rsc_set (const char *str, unsigned int len);
void print_rsc_files (void);
const char *get_rsc_approx (const char *rsc_name);
void free_modified_rsc (void);

#endif /* HEROES__RSC_FILES_HASH__H */
