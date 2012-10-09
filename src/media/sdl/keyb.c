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
#include <assert.h>
#include <SDL.h>
#include "keyb_low.h"
#include "errors.h"

static int enable_mouse = 0;

int mouse_pos_x = 0;
int mouse_pos_y = 0;
char mouse_button_left = 0;
char mouse_button_right = 0;
char mouse_button_middle = 0;

/* These macros are defined in SDL 1.1.x but not in 1.0.x */
#ifndef SDL_BUTTON_LEFT
#define SDL_BUTTON_LEFT         1
#endif
#ifndef SDL_BUTTON_MIDDLE
#define SDL_BUTTON_MIDDLE       2
#endif
#ifndef SDL_BUTTON_RIGHT
#define SDL_BUTTON_RIGHT        3
#endif

int
init_mouse (void)
{
  return 0;
}

void
mouse_show (void)
{
  enable_mouse = 1;
  SDL_EventState (SDL_MOUSEMOTION, SDL_ENABLE);
  SDL_EventState (SDL_MOUSEBUTTONUP, SDL_ENABLE);
  SDL_EventState (SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
  SDL_ShowCursor (1);
}

void
mouse_hide (void)
{
  SDL_ShowCursor (0);
  SDL_EventState (SDL_MOUSEMOTION, SDL_IGNORE);
  SDL_EventState (SDL_MOUSEBUTTONUP, SDL_IGNORE);
  SDL_EventState (SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  enable_mouse = 0;
}

static int
handle_mouse_events (const SDL_Event *ev)
{
  if (ev->type == SDL_MOUSEMOTION) {
    mouse_pos_x = ev->motion.x;
    mouse_pos_y = ev->motion.y;
    return 1;
  } else if (ev->type == SDL_MOUSEBUTTONUP) {
    if (ev->button.button == SDL_BUTTON_LEFT)
      mouse_button_left = 0;
    else if (ev->button.button == SDL_BUTTON_RIGHT)
      mouse_button_right = 0;
    else if (ev->button.button == SDL_BUTTON_MIDDLE)
      mouse_button_middle = 0;
    return 1;
  } else if (ev->type == SDL_MOUSEBUTTONDOWN) {
    if (ev->button.button == SDL_BUTTON_LEFT)
      mouse_button_left = 1;
    else if (ev->button.button == SDL_BUTTON_RIGHT)
      mouse_button_right = 1;
    else if (ev->button.button == SDL_BUTTON_MIDDLE)
      mouse_button_middle = 1;
    return 1;
  }
  return 0;
}

void
update_mouse_state (void)
{
  SDL_Event ev;

  SDL_PumpEvents ();
  while (SDL_PeepEvents (&ev, 1, SDL_GETEVENT,
			 SDL_MOUSEMOTIONMASK|
			 SDL_MOUSEBUTTONDOWNMASK|
			 SDL_MOUSEBUTTONUPMASK)) {
    handle_mouse_events (&ev);
  }
}
void
process_input_events (void)
{
  SDL_Event ev;

  while (SDL_PollEvent (&ev)) {
    if (ev.type == SDL_KEYDOWN) {
      assert (ev.key.keysym.sym <= HKEY_MAX);
      keyboard_map[ev.key.keysym.sym] = 1;
    } else if (ev.type == SDL_KEYUP) {
      assert (ev.key.keysym.sym <= HKEY_MAX);
      keyboard_map[ev.key.keysym.sym] = 0;
    } else if (handle_mouse_events (&ev)) {
      /* Nothing to do, handle_mouse_events already did everything. */
    } else if (ev.type == SDL_QUIT) {
      exit_heroes (0);
    } else {
      /* printf ("unexpected event %d\n", ev.type); */
    }
  }
  keyboard_modifiers = SDL_GetModState ();
}

a_keycode
get_key (void)
{
  SDL_Event e;
  int first_time = 1;
  do {
    if (!first_time) {
      SDL_Delay (10);
    } else {
      first_time = 0;
    }
    SDL_PumpEvents ();
    /* remove all events until we get a KEYDOWN event */
    while (SDL_PeepEvents (&e, 1, SDL_GETEVENT, ~SDL_KEYDOWNMASK)) {
      /* we might need to handle mouse and quit events */
      if (enable_mouse)
	handle_mouse_events (&e);
      if (e.type == SDL_QUIT)
	exit_heroes (0);
    }
  } while (!SDL_PeepEvents (&e, 1, SDL_GETEVENT, SDL_KEYDOWNMASK));

  keyboard_modifiers = SDL_GetModState ();
  return e.key.keysym.sym;
}

int
key_ready (void)
{
  SDL_PumpEvents ();
  return SDL_PeepEvents (0, 1, SDL_GETEVENT, SDL_KEYDOWNMASK|SDL_QUITMASK);
  /* return true if there is a pending SDL_QUIT event: the next call to
     get_key will process it */
}

const char *
keys_pref_group (void)
{
  return "sdl";
}

int
keycode_to_ascii (a_keycode a)
{
  return a;
}
