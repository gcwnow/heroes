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


#ifndef HEROES__FASTMEM__H
#define HEROES__FASTMEM__H

/* In the good old days when Heroes was still a DOS game these three
 * macros used the be inline assembly instructions.
 *
 * Today, since gcc can optimize memcpy as an inline copy loop, they are
 * just synonyms for memcpy.  
 *
 * Still, it's a good idea to keep using these macros (and not memcpy)
 * since they may be helpful to achieve some optimization with other
 * compilers.  
 */

#define fastmem1(src,dest,size) memcpy((dest),(src),(size))
#define fastmem2(src,dest,size) memcpy((dest),(src),(size)<<1)
#define fastmem4(src,dest,size) memcpy((dest),(src),(size)<<2)

#endif /* HEROES__FASTMEM__H */
