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
#include "lvl_priv.h"

static int
lvl_save_header_file (int fd, const a_level *out)
{
  a_u8 *data = xmalloc (LVL_HEADER_SIZE);

  encode_level_header (data, out);

  if (write (fd, data, LVL_HEADER_SIZE) != LVL_HEADER_SIZE) {
    free (data);
    return -1;
  }

  free (data);
  return 0;
}

static int
lvl_save_body_file (int fd, const a_level *out)
{
  ssize_t length = out->tile_count * LVL_RECORD_SIZE;
  a_u8 *data = xmalloc (length);
  memset (data, 0, length);

  encode_level_body (data, out);

  if (write (fd, data, length) != length) {
    free (data);
    return -1;
  }

  free (data);
  return 0;
}

int
lvl_save_file (const char *filename, const a_level *out)
{
  int fd;
  int err;

  fd = open (filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IRWUGO);
  if (fd == -1)
    return -1;

  err = lvl_save_header_file (fd, out);
  if (!err)
    err = lvl_save_body_file (fd, out);

  close (fd);

  return err;
}
