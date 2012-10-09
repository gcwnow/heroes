/*------------------------------------------------------------------.
| Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
|                                                                   |
| This file is part of Heroes.                                      |
|                                                                   |
| Heroes is free software; you can redistribute it and/or modify it |
| under the terms of the GNU General Public License as published by |
| the Free Software Foundation; either version 2 of the License, or |
| (at your option) any later version.                               |
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

/* An evil hack.  The sources in src/* are compiled with
   -Wcast-qual so that the compiler will warn about any conversion
   of (char*) to (const char*), even explicit.
   Yet there are few case (like when calling a system function
   with a dubious prototype) where such cast is required.

   So we simply define a function that do that.  Because this
   function is compiled in lib/ where -Wcast-qual is not used,
   we avoid the warning.  This incur a sligh run-time penalty, but
   we don't care; this is really seldom used.  */

char *
const_cast_string (const char *s)
{
  return (char *) s;
}
