/*------------------------------------------------------------------.
| Copyright 2001, 2002  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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

#ifndef HEROES__ACCESS__H
#define HEROES__ACCESS__H

/* All the following macros assume that p is a pointer to unsigned
   char.  */

/* Read little-endian a_u16 and a_u32 values.  */
#define GET_U16(p) ((p)[0] | (p)[1] << 8)
#define GET_U32(p) (GET_U16(p) | (p)[2] << 16 | (p)[3] << 24)

#define READ_U32(p, res)			\
  do {						\
    res = GET_U32 (p);				\
    p += 4;					\
  } while (0)

#define READ_U16(p, res)			\
  do {						\
    res = GET_U16 (p);				\
    p += 2;					\
  } while (0)

/* Likewise with a_u8, for completness.  */
#define GET_U8(p) (*(p))
#define READ_U8(p, res) res = *p++

/* Write little-endian a_u16 and a_u32 values.  */
#define SET_U16(p, val)				\
  do {						\
    (p)[0] = val;				\
    (p)[1] = val >> 8;				\
  } while (0)

#define SET_U32(p, val)				\
  do {						\
    (p)[0] = val;				\
    (p)[1] = val >> 8;				\
    (p)[2] = val >> 16;				\
    (p)[3] = val >> 24;				\
  } while (0)

#define WRITE_U32(p, val)			\
  do {						\
    SET_U32 (p, val);				\
    p += 4;					\
  } while (0)

#define WRITE_U16(p, val)			\
  do {						\
    SET_U16 (p, val);				\
    p += 2;					\
  } while (0)

/* Likewise with a_u8, for completness.  */
#define SET_U8(p, val) (*(p) = val)
#define WRITE_U8(p, val) *p++ = val

#endif /* HEROES__ACCESS__H */
