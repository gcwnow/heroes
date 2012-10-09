/*------------------------------------------------------------------.
| Copyright 1997, 1998, 2000, 2001, 2002  Alexandre Duret-Lutz      |
|                                          <duret_g@epita.fr>       |
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
#include <SDL.h>
#include "video_low.h"
#include "debugmsg.h"
#include "errors.h"
#include "misc.h"
#include "argv.h"
#include "cast.h"

static a_pixel *screen_rv = 0;	/* A pointer to the screen buffer associated
				   to the render visual. */
static int scr_w, scr_h;	/* rendering buffer width and height */
static int scr_stretch;

/* screen_rv may be a direct pointer to the hardware video buffer, or
   it may be a pointer to system (mallocated) memory, this depends on
   the display driver (if the videomode were available or needed to be
   emulated etc.).  If screen_rv points directly to hardware video
   it might requires locking. */

static SDL_Surface *visu = 0;
static int visu_options = SDL_HWPALETTE | SDL_DOUBLEBUF;
static bool SDL_initialized = false;
#define SDL_VIDEODRIVER "SDL_VIDEODRIVER"
static char *sdl_videodriver = 0;

void
set_display_params (const char *str)
{
  sdl_videodriver = strcat_alloc (SDL_VIDEODRIVER "=", str);
  dmsg (D_SYSTEM | D_VIDEO, "put `%s' in environment", sdl_videodriver);
  putenv (sdl_videodriver);
}

void
set_full_screen_mode (void)
{
  visu_options |= SDL_FULLSCREEN;
}

/* Fullscreen mode is toggeled by pressing Alt+Enter.
   Thanks to Sam Lantinga */
static int
Handle_AltEnter (const SDL_Event *event)
{
  if (event->type == SDL_KEYDOWN
      && event->key.keysym.sym == SDLK_RETURN
      && event->key.keysym.mod & KMOD_ALT) {
    SDL_WM_ToggleFullScreen (SDL_GetVideoSurface ());
    return 0;
  }
  return 1;
}

/* init the SDL library, this can be called from joystick.c
   or from init_video() */
void init_SDL (void);
void
init_SDL (void)
{
  if (SDL_initialized)
    return;
  dmsg (D_SYSTEM | D_VIDEO | D_JOYSTICK | D_SOUND_TRACK | D_SOUND_EFFECT,
	"initialize SDL");
  SDL_Init (SDL_INIT_VIDEO
#ifdef HAVE_LIBSDL_MIXER
	    | ((nosfx && nosound) ? 0 : SDL_INIT_AUDIO)
#endif
#ifdef HAVE_SDL_JOYSTICKOPEN
	    | (joyoff ? 0 : SDL_INIT_JOYSTICK)
#endif
#ifdef DEBUG
	    | SDL_INIT_NOPARACHUTE
#endif
	    );

  SDL_SetEventFilter(Handle_AltEnter);
  SDL_initialized = true;
}

void
init_video_low (int stretch_, int *pitch)
{
  scr_stretch = stretch_;
  scr_w = 320 * scr_stretch;
  scr_h = 200 * scr_stretch;

  init_SDL ();
  dmsg (D_VIDEO, "set video mode");
  visu = SDL_SetVideoMode (scr_w, scr_h, 8, visu_options);
  /* FIXME: the Linux/m68k binary is crashing in the vicinity */
  if (!visu)
    emsg (_("Failed to open visual: %s"), SDL_GetError());

  if (SDL_MUSTLOCK (visu))
    dmsg (D_VIDEO, "visual require locking");
  else
    screen_rv = visu->pixels;

  *pitch = visu->pitch;

  dmsg (D_VIDEO, "set misc. video parameters");

  SDL_ShowCursor (0);
  SDL_WM_SetCaption ("Heroes " VERSION,"Heroes");

  /* setup event processing rules.
     FIXME: this does not really belong to display.c
     it should rather go to keyb.c */
  SDL_EventState ((Uint8) SDL_ALLEVENTS, SDL_IGNORE);
  SDL_EventState (SDL_KEYDOWN, SDL_ENABLE);
  SDL_EventState (SDL_KEYUP, SDL_ENABLE);
  SDL_EventState (SDL_QUIT, SDL_ENABLE);
#ifdef HAVE_SDL_ENABLEKEYREPEAT
  SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
#endif
}

void
uninit_video_low (void)
{
  dmsg (D_MISC, "free screen buffer");

  if (SDL_initialized) {
    SDL_Quit ();
    SDL_initialized = false;
  }
  if (sdl_videodriver) {
    putenv (const_cast_string (SDL_VIDEODRIVER));
    XFREE0 (sdl_videodriver);
  }
}

void
set_pal_entry (unsigned char c,
	       unsigned char r, unsigned char g, unsigned char b)
{
  SDL_Color col;
  col.r = r * 4;
  col.g = g * 4;
  col.b = b * 4;
  dmsg (D_VIDEO, "set color %d=(%d,%d,%d)", c, r, g, b);
  SDL_SetColors (visu, &col, c, 1);
}

void
set_pal (const unsigned char *ptr, int p, int n)
{
  signed i;

  SDL_Color cmap[256];
  for (i = 0; i < 256; ++i) {
    cmap[i].r = *ptr++ * 4;
    cmap[i].g = *ptr++ * 4;
    cmap[i].b = *ptr++ * 4;
  }
  dmsg (D_VIDEO, "set %d colors", n/3);
  SDL_SetColors (visu, cmap, p/3, n/3);
}

void
vsynchro_low (const a_pixel *s, a_copy_function f)
{
  if (SDL_MUSTLOCK (visu))
    SDL_LockSurface (visu);

  screen_rv = visu->pixels;
  f (s, screen_rv, 320);

  if (SDL_MUSTLOCK (visu))
    SDL_UnlockSurface (visu);

  SDL_Flip (visu);		/* can change visu->pixels */
}

void
vsynchro2_low (const a_pixel *s1, const a_pixel *s2, a_copy_function f)
{
  if (SDL_MUSTLOCK (visu))
    SDL_LockSurface (visu);

  screen_rv = visu->pixels;
  f (s1, screen_rv, 160);
  f (s2, screen_rv + 160 * scr_stretch, 160);

  if (SDL_MUSTLOCK (visu))
    SDL_UnlockSurface (visu);

  SDL_Flip (visu);		/* can change visu->pixels */
}
