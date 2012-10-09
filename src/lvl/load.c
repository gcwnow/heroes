/*------------------------------------------------------------------.
| Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
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

static void
lvl_init_lvl_basic_fields (a_level *out)
{
  out->square_height = out->tile_height * 2;
  out->square_width = out->tile_width * 2;

  if (out->tile_height_wrap == DONT_WRAP)
    out->square_height_wrap = DONT_WRAP;
  else
    out->square_height_wrap = (out->tile_height_wrap << 1) | 1;

  if (out->tile_width_wrap == DONT_WRAP)
    out->square_width_wrap = DONT_WRAP;
  else
    out->square_width_wrap = (out->tile_width_wrap << 1) | 1;

  out->tile_count = out->tile_height * out->tile_width;
  out->square_count = out->square_height * out->square_width;
}

static int
lvl_load_header_mem (a_u8 *data, a_level *out)
{
  decode_level_header (data, out);
  lvl_init_lvl_basic_fields (out);
  return 0;
}

static int
lvl_load_header_file (int fd, a_level *out)
{
  a_u8 *data = xmalloc (LVL_HEADER_SIZE);

  if (read (fd, data, LVL_HEADER_SIZE) != LVL_HEADER_SIZE) {
    free (data);
    return -1;
  }

  lvl_load_header_mem (data, out);

  free (data);
  return 0;
}

static int
lvl_load_body_mem (a_u8 *data, a_level *out)
{
  initialize_level_body (out);
  decode_level_body (data, out);
  return 0;
}

static int
lvl_load_body_file (int fd, a_level *out)
{
  size_t length = out->tile_count * LVL_RECORD_SIZE;
  a_u8 *data = xmalloc (length);
  ssize_t rlength = read (fd, data, length);

  if (rlength < 0 || (size_t) rlength != length) {
    free (data);
    return -1;
  }

  lvl_load_body_mem (data, out);

  free (data);
  return 0;
}

int
lvl_load_file (const char *filename, a_level *out, bool load_body)
{
  int fd;
  int err;
#ifdef HAVE_MMAP
  bool use_mmap = false;
#endif
  a_u8 *data;
  struct stat st;

  memset (out, 0, sizeof *out);

  fd = open (filename, O_RDONLY | O_BINARY);
  if (fd == -1)
    return -1;

#ifdef HAVE_MMAP
  /* We need the size of the file in order to mmap() it.  */
  if (fstat (fd, &st) != 0) {
    close (fd);
    return -1;
  }
#endif

  XMALLOC_VAR (out->private);

#ifdef HAVE_MMAP
  /* Try to mmap the file.  If it fail for any reason, we'll
     fall back to the standard file reading method.  */
  data = mmap (0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data != (void*) -1) {
    /* Success.  */
    use_mmap = true;
    close (fd);
  }

  if (!use_mmap) {
#endif
    err = lvl_load_header_file (fd, out);
    if (!err && load_body)
      err = lvl_load_body_file (fd, out);
    else
      initialize_empty_level_body (out);
    close (fd);
#ifdef HAVE_MMAP
  } else {
    err = lvl_load_header_mem (data, out);
    if (!err && load_body)
      err = lvl_load_body_mem (data + LVL_HEADER_SIZE, out);
    else
      initialize_empty_level_body (out);
    munmap (data, st.st_size);
  }
#endif

  return err;
}
