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
#include <ggi/ggi.h>
#include "video_low.h"
#include "debugmsg.h"
#include "errors.h"

/* LibGGI driver for Heroes.

   Heroes is a 320x200x8bit game so we try to use such a video mode.
   That's not always possible, so we negotiate the closest video mode
   available.

   When the opened video mode is not 8bit depth, another intermediate
   visual (`render_visu') is opened in memory to hold the 8bit
   rendered screens; then this visual is CrossBlited to the main visual
   by LibGGI.

   Hence, there are two different drawing mode: either direct drawing
   to the real visual (`visu') if it has a 8bit depth, or drawing to
   the intermediate visual (`render_visu').  Testing for render_visu
   == NULL, is enough to know in which mode we are.

   We use double buffering for `visu' whenever possible.
*/

ggi_visual_t visu = 0;		/* The real display, which receive events. */
static ggi_visual_t render_visu = NULL; /* A 8bit memory-display on which
					   the game is drawn, if visu is not
					   8bit.*/
static int scr_w, scr_h;	/* rendering buffer width and height */
static int scr_pitch;
static int scr_stretch;

static ggi_mode vid_mode;
static char *display_params = NULL;
static int full_screen = 0;
/* Direct buffer for each frame.
   We might have db[0] == db[1] if double buffering is not available.*/
static const ggi_directbuffer *db[2] = { NULL, NULL };
static int current_frame = 0;	/* used as index to `db' for
				   double buffering */

static bool ggi_initialized = false;

/* GGI fails to allocate a 8bit mode for some special X configuration.
   (Probably when the server supports 8bits depth although it's not
   the _default_ depth.)  A workaround is to not ask GGI for a 8bit
   mode; this is activated if DONT_REQUEST_8BIT_MODE is TRUE.  */
static bool dont_request_8bit_mode = false;

void
set_display_params (const char* str)
{
  /* Turn on the dont_request_8bit_mode workaround if the user typed
     `-G not8'.  Other GGIs argument may be supplied after a colon as
     in `-G not8:mumble' */
  if (strncmp(str, "not8", 4) == 0 && (str[4] == ':' || str[4] == 0)) {
    dont_request_8bit_mode = true;
    if (str[4] == 0)
      return;
    str += 5;
  }
  XFREE0 (display_params);
  display_params = xstrdup (str);
}

void
set_full_screen_mode (void)
{
  full_screen = 1;
}

static bool
setup_320x200x8_display (void)
{
  if (dont_request_8bit_mode) {
    dmsg (D_VIDEO, "skipped 320x200x8/2 mode negociation");
    return false;
  }
  dmsg (D_VIDEO, "negotiate 320x200x8/2 mode");
  vid_mode.frames = 2;
  vid_mode.visible.x = scr_w;
  vid_mode.visible.y = scr_h;
  vid_mode.virt.x = vid_mode.virt.y = GGI_AUTO;
  vid_mode.dpp.x = vid_mode.dpp.y = GGI_AUTO;
  vid_mode.graphtype = GT_8BIT;

  if (ggiCheckMode (visu, &vid_mode) == 0)
    return ggiSetMode (visu, &vid_mode) == 0;
  return false;
}

static bool
setup_320x200xB_display (void)
{
  dmsg (D_VIDEO, "negotiate any 320x200xB/2 mode");
  vid_mode.frames = 2;
  vid_mode.visible.x = scr_w;
  vid_mode.visible.y = scr_h;
  vid_mode.virt.x = vid_mode.virt.y = GGI_AUTO;
  vid_mode.dpp.x = vid_mode.dpp.y = GGI_AUTO;
  vid_mode.graphtype = GGI_AUTO;

  if (ggiCheckMode (visu, &vid_mode) == 0)
    return ggiSetMode (visu, &vid_mode) == 0;
  return false;
}

static bool
setup_WWWxHHHxB_display (void)
{
  dmsg (D_VIDEO, "negotiate any (2-frame) mode");
  vid_mode.frames = 2;
  vid_mode.visible.x = scr_w;
  vid_mode.visible.y = scr_h;
  vid_mode.virt.x = vid_mode.virt.y = GGI_AUTO;
  vid_mode.dpp.x = vid_mode.dpp.y = GGI_AUTO;
  vid_mode.graphtype = GT_8BIT;

  if (ggiCheckMode (visu, &vid_mode) == 0)
    return ggiSetMode (visu, &vid_mode) == 0;
  return false;
}

static void
get_directbuffers (ggi_visual_t v)
{
  int numbuf;
  int i;

  dmsg (D_VIDEO, "ask for direct-buffers");
  numbuf = ggiDBGetNumBuffers (v);
  dmsg (D_VIDEO, "%d direct buffers available", numbuf);
  for (i = 0; i < numbuf; ++i) {
    const ggi_directbuffer *d = ggiDBGetBuffer (v, i);
    if (d == NULL)
      continue;
    dmsg (D_VIDEO, "DB#%d: type=%x frame=%d", i, d->type, d->frame);
    if (!(d->type & GGI_DB_SIMPLE_PLB))
      continue;
    if (db[d->frame] == NULL)
      db[d->frame] = d;
  }

  if (db[0] != NULL && db[1] == NULL) {
    dmsg (D_VIDEO, "got only the first direct buffer");
    db[1] = db[0];
  } else if (db[0] == NULL && db[1] != NULL) {
    dmsg (D_VIDEO, "got only the second direct buffer ???");
    db[0] = db[1];
  } else if (db[0] == NULL && db[1] == NULL)
    /* FIXME: ask to send a bug report with a trace */
    emsg (_("failed to get any direct buffer for this visual"));
}

void
init_video_low (int stretch, int *pitch)
{
  scr_stretch = stretch;

  dmsg (D_VIDEO | D_SYSTEM, "initialize GGI");
  if (ggiInit ()) {
    dmsg (D_VIDEO, "failed");
    exit (EXIT_FAILURE);
  }

  ggi_initialized = true;

  dmsg (D_VIDEO, "open main visual (%s)",
	display_params ? display_params : "null");
  visu = ggiOpen (display_params);
  if (!visu)
    /* TRANS: a `visual' is associated by LibGGI to a display.  You can
       actually call it a `display', this makes no big difference.  */
    emsg (_("failed to open visual."));

  scr_w = 320 * stretch;
  scr_h = 200 * stretch;

  if (!(setup_320x200x8_display () ||
	setup_320x200xB_display () ||
	setup_WWWxHHHxB_display ()))
    emsg (_("cannot setup a correct display."));
  /* get info about the mode that was selected */
  ggiGetMode (visu, &vid_mode);
  dmsg (D_VIDEO, "video mode is %dx%dx%d/%d",
	vid_mode.visible.x, vid_mode.visible.y,
	GT_DEPTH (vid_mode.graphtype), vid_mode.frames);
  if (vid_mode.virt.x < 320 || vid_mode.virt.y < 200)
    emsg (_("negotiated video mode is too small (width=%d, height=%d)"),
	  vid_mode.virt.x, vid_mode.virt.y);
  if (GT_DEPTH (vid_mode.graphtype) == 8) {
    /* no intermediate buffer required */
    render_visu = NULL;
    scr_pitch = vid_mode.virt.x;
  } else {
    /* open an intermediate rendefing buffer */
    dmsg (D_VIDEO, "open display-memory visual");
    render_visu = ggiOpen ("display-memory", NULL);
    if (render_visu == NULL)
      /* TRANS: `display-memory' is the name of a LibGGI driver and
	 make no sense to translate. */
      emsg (_("Failed to open an internal `display-memory' visual."));
    if (ggiSetGraphMode (render_visu, scr_w, scr_h, scr_w, scr_h, GT_8BIT))
      emsg (_("Failed to set 320x200x8 mode on display-memory"));
    scr_pitch = scr_w;
  }
  *pitch = scr_pitch;

  get_directbuffers (render_visu != NULL ? render_visu : visu);

  dmsg (D_VIDEO, "set display flags");
  ggiAddFlags (visu, GGIFLAG_ASYNC);
  if (GT_SCHEME (vid_mode.graphtype) == GT_PALETTE)
    ggiSetColorfulPalette (visu);
  if (render_visu != NULL)
    ggiSetColorfulPalette (render_visu);
}

void
uninit_video_low (void)
{
  dmsg (D_MISC, "uninitialize video");
  XFREE0 (display_params);
  if (render_visu != NULL) {
    dmsg (D_VIDEO, "close memory visual");
    ggiClose (render_visu);
    render_visu = NULL;
  }
  if (visu != NULL) {
    dmsg (D_VIDEO, "close real visual");
    ggiClose (visu);
    visu = NULL;
  }
  if (ggi_initialized) {
    dmsg (D_VIDEO, "exit GGI");
    ggiExit ();
    ggi_initialized = false;
  }
}

void
set_pal_entry (unsigned char c,
	       unsigned char r, unsigned char g, unsigned char b)
{
  ggi_color cmap[256];
  cmap[c].r = r * 1024;
  cmap[c].g = g * 1024;
  cmap[c].b = b * 1024;
  dmsg (D_VIDEO, "set color %d=(%d,%d,%d)",c,r,g,b);
  if (GT_SCHEME (vid_mode.graphtype) == GT_PALETTE)
    ggiSetPalette (visu, c, 1, cmap);
  if (render_visu != NULL)
    ggiSetPalette (render_visu, c, 1, cmap);
}

void
set_pal (const unsigned char *ptr, int p, int n)
{
  signed i;

  ggi_color cmap[256];
  for (i = 0; i < 256; ++i) {
    cmap[i].r = *ptr++ * 1024;
    cmap[i].g = *ptr++ * 1024;
    cmap[i].b = *ptr++ * 1024;
  }
  dmsg (D_VIDEO, "set %d colors", n/3);
  if (GT_SCHEME (vid_mode.graphtype) == GT_PALETTE)
    ggiSetPalette (visu, p / 3, n / 3, cmap);
  if (render_visu != NULL)
    ggiSetPalette (render_visu, p / 3, n / 3, cmap);
}

void
vsynchro2_low (const a_pixel *s1, const a_pixel *s2, a_copy_function f)
{
  a_pixel *dest;

  current_frame ^= 1;

  if (ggiResourceAcquire (db[current_frame]->resource, GGI_ACTYPE_WRITE))
    wmsg (_("failed to acquire direct-buffer"));

  dest = db[current_frame]->write;

  if (render_visu == NULL)
    /* center */
    dest += (vid_mode.visible.x - scr_w)/2 +
      scr_pitch * (vid_mode.visible.y - scr_h)/2;

  if (s2 == NULL) {
    f (s1, dest, 320);
  } else {
    f (s1, dest, 160);
    f (s2, dest + 160 * scr_stretch, 160);
  }

  if (ggiResourceRelease (db[current_frame]->resource))
    wmsg (_("failed to release direct-buffer"));

  if (render_visu != NULL) {
    ggiSetWriteFrame(visu, current_frame);
    ggiCrossBlit (render_visu, 0, 0, scr_w, scr_h, visu,
		  (vid_mode.visible.x - scr_w)/2,
		  (vid_mode.visible.y - scr_h)/2);
  }

  ggiSetDisplayFrame (visu, current_frame);
  ggiFlush (visu);
}

void
vsynchro_low (const a_pixel *s, a_copy_function f)
{
  vsynchro2_low (s, NULL, f);
}
