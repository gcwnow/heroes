/*------------------------------------------------------------------.
| Copyright 1997, 1998, 2000, 2001  Alexandre Duret-Lutz            |
|                                    <duret_g@epita.fr>             |
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
#include "errors.h"
#include "pcx.h"
#include "rsc_files.h"
#include "bytesex.h"
#include "debugmsg.h"

static void
img_init (a_pcx_image *image)
{
  XMALLOC_ARRAY (image->buffer, image->size);
}

void
img_free (a_pcx_image *image)
{
  free (image->buffer);
}

char
pcx_load (const char *file, a_pcx_image *image)
{
  unsigned long compteur;
  FILE *fptr;

  int nbrbytes, i;
  a_u8 data;

  dmsg (D_FILE, "opening image file: %s", file);

  if ((fptr = fopen (file, "rb")) == NULL) {
    emsg (_("Cannot open %s"), file);
  }
  fread (&(image->header), sizeof (a_pcx_header), 1, fptr);

  /* convert to local endianess */
  image->header.x = BSWAP16 (image->header.x);
  image->header.y = BSWAP16 (image->header.y);
  image->header.width = BSWAP16 (image->header.width);
  image->header.height = BSWAP16 (image->header.height);
  image->header.widthdpi = BSWAP16 (image->header.widthdpi);
  image->header.heightdpi = BSWAP16 (image->header.heightdpi);
  image->header.bytes_per_lines = BSWAP16 (image->header.bytes_per_lines);
  image->header.palette_kind = BSWAP16 (image->header.palette_kind);

  image->width = image->header.width - image->header.x + 1;
  image->height = image->header.height - image->header.y + 1;
  image->size = image->width * image->height;

  dmsg (D_FILE, "size=(%d,%d) rle=%d",
	image->header.width + 1, image->header.height + 1, image->header.rle);

  img_init (image);

  compteur = 0;
  if (image->header.rle) {
    while (compteur < image->size) {
      data = (a_u8) getc (fptr);
      if ((data & 192) == 192) {
	nbrbytes = data & 63;
	data = (a_u8) getc (fptr);
	while (nbrbytes--)
	  image->buffer[compteur++] = data;
      } else {
	image->buffer[compteur++] = data;
      }
    }
  } else {
    fread (image->buffer, image->size, 1, fptr);
  }
  data = (a_u8) getc (fptr);	/* data==0Ch expected */

  fread (image->palette.global, 768, 1, fptr);
  for (i = 0; i < 256 * 3; i++)
    image->palette.global[i] >>= 2;

  fclose (fptr);
  return 0;
}

char
pcx_load_from_rsc (const char *rsc, a_pcx_image *image)
{
  char *res = get_non_null_rsc_file (rsc);
  char error = pcx_load (res, image);
  free (res);
  return error;
}
