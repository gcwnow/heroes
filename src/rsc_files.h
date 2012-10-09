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

#ifndef HEROES__RSC_FILES__H
#define HEROES__RSC_FILES__H


int set_rsc_file (const char *rsc_name, const char *file_name,
		  bool secure);

/*
 * The secure calls tell whether the returned value has been
 * set in a secure way (i.e. hardcoded or from the global
 * configuration file).  Note that the secure flag is set to 0
 * when a value is not secure, but is never set to 1: this is
 * the job of the caller.
 */
char *get_rsc_file (const char *rsc_name);
char *get_rsc_file_secure (const char *rsc_name, bool *secure);
char *get_non_null_rsc_file (const char *rsc_name);
char *get_non_null_rsc_file_secure (const char *rsc_name, bool *secure);
char *rsc_expand (char *value);
char *rsc_expand_secure (char *value, bool *secure);

#endif /* HEROES__RSC_FILES__H */
