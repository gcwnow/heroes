/*-------------------------------------------------------------------.
| Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>            |
|                                                                    |
| This program is free software; you can redistribute it and/or      |
| modify it under the terms of the GNU General Public License as     |
| published by the Free Software Foundation; either version 2 of the |
| License, or (at your option) any later version.                    |
|                                                                    |
| This program is distributed in the hope that it will be useful,    |
| but WITHOUT ANY WARRANTY; without even the implied warranty of     |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  |
| General Public License for more details.                           |
|                                                                    |
| You should have received a copy of the GNU General Public License  |
| along with this program; if not, write to the Free Software        |
| Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA           |
| 02111-1307 USA                                                     |
`-------------------------------------------------------------------*/

/* Lock a file, blocking if needed.
   If EXCLUSIVE is 0 then many processes can lock the
   file (usually this is for reading), otherwise only one
   process can lock the file (for writing).

   Return 0 on success, something else on error. */
int file_lock (int fd, int exlusive);

/* Unlock a file.  */
int file_unlock (int fd);

/* WARNING: under Windows, these functions will lock or unlock
   the 10 next bytes.  Therefore you should make sure that
   you always call these functions from the same position.
*/
