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
#include "joystick.h"
#include "debugmsg.h"
#include "errors.h"
#include <SDL.h>

int joystick_x[2] = { 0, 0 };	/* coord. X */
int joystick_y[2] = { 0, 0 };	/*        Y */
char joystick_b[2] = { 0, 0 };	/* buttons (on 2bits) */
char joystick_detected = 0;

SDL_Joystick* joystick[2] = { 0, 0 };
int joystick0_butnbr = 0;	/* The number of button of
				   the FIRST joystick */

extern void init_SDL (void);

char joyinit (void)
{
  int nbr;

  init_SDL ();

  dmsg (D_JOYSTICK, "initialize joystick");

  nbr = SDL_NumJoysticks ();

  if (nbr <= 0) {
    wmsg (_("No joystick found (run with `-J' to suppress this message)."));
    return joystick_detected = 0;
  }

  dmsg (D_JOYSTICK, "%d joystick found", nbr);

  joystick[0] = SDL_JoystickOpen (0);
  if (nbr >= 2)
    joystick[1] = SDL_JoystickOpen(1);
  SDL_JoystickEventState (SDL_IGNORE);
  joystick0_butnbr = SDL_JoystickNumButtons (joystick[0]);
  return joystick_detected = (joystick[0] != 0);
}

void get_joystick_state (void)
{
  SDL_JoystickUpdate();
  if (joystick[0]) {
    joystick_b[0] = (joystick_b[0] & ~1) |
      (SDL_JoystickGetButton (joystick[0], 0) & 1);
    joystick_b[0] = (joystick_b[0] & ~2) |
      ((SDL_JoystickGetButton (joystick[0], 1) & 1) << 1);
    joystick_x[0] = SDL_JoystickGetAxis(joystick[0], 0);
    joystick_y[0] = SDL_JoystickGetAxis(joystick[0], 1);
    if (!joystick[1] && joystick0_butnbr >= 4) {
      joystick_b[1] = (joystick_b[1] & ~1) |
	(SDL_JoystickGetButton (joystick[0], 2) & 1);
      joystick_b[1] = (joystick_b[1] & ~2) |
	((SDL_JoystickGetButton (joystick[0], 3) & 1) << 1);
    }
  }
  if (joystick[1]) {
    joystick_b[1] = (joystick_b[1] & ~1) |
      (SDL_JoystickGetButton (joystick[1], 0) & 1);
    joystick_b[1] = (joystick_b[1] & ~2) |
      ((SDL_JoystickGetButton (joystick[1], 1) & 1) << 1);
    joystick_x[1] = SDL_JoystickGetAxis(joystick[1], 0);
    joystick_y[1] = SDL_JoystickGetAxis(joystick[1], 1);
  }
  if (joystick[0])
    dmsg (D_JOYSTICK, "joystick #1 state: x=%d y=%d b=%d",
	  joystick_x[0], joystick_y[0], joystick_b[0]);
  if (joystick[1])
    dmsg (D_JOYSTICK, "joystick #2 state: x=%d y=%d b=%d",
	  joystick_x[1], joystick_y[1], joystick_b[1]);
}
