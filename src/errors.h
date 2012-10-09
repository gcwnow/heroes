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


#ifndef HEROES__ERRORS__H
#define HEROES__ERRORS__H

/* warning messages */

extern int disable_wmsg;

#ifdef VA_START
void wmsg (const char* msg, ...) ATTRIBUTE_PRINTF (1, 2);
#else
void wmsg (void);
#endif

/* error messages */

extern int disable_emsg;

#ifdef VA_START
void emsg (const char* msg, ...) ATTRIBUTE_PRINTF (1, 2) ATTRIBUTE_NORETURN;
#else
void emsg (void);
#endif

/* exit heroes, closing libraries */

void exit_heroes (int code) ATTRIBUTE_NORETURN;

#endif  /* HEROES__ERRORS__H */
