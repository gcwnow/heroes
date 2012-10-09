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
#include <allegro.h>
#include "joystick.h"
#include "debugmsg.h"
#include "errors.h"

int joystick_x[2] = { 0, 0 };	/* coord. X */
int joystick_y[2] = { 0, 0 };	/*        Y */
char joystick_b[2] = { 0, 0 };	/* buttons (on 2bits) */
char joystick_detected = 0;

extern void heroes_init_allegro(void);

char joyinit (void)
{
  dmsg (D_JOYSTICK, "initialize joystick");

  heroes_init_allegro();

  if (install_joystick(JOY_TYPE_AUTODETECT) != 0) {
    num_joysticks = 0;
  }

  if (num_joysticks <= 0) {
    wmsg (_("No joystick found (run with `-J' to suppress this message)."));
    return joystick_detected = 0;
  }

  dmsg (D_JOYSTICK, "%d joystick(s) found", num_joysticks);

  return ((joystick_detected = 1));
}

void get_joystick_state (void)
{
  poll_joystick ();

  if (num_joysticks >= 1) {
    joystick_b[0] = (joystick_b[0] & ~1) | (joy[0].button[0].b ? 1 : 0);
    joystick_b[0] = (joystick_b[0] & ~2) | (joy[0].button[1].b ? 2 : 0);
    if (num_joysticks == 1 && joy[0].num_buttons >= 4) {
      joystick_b[1] = (joystick_b[1] & ~1) | (joy[0].button[2].b ? 1 : 0);
      joystick_b[1] = (joystick_b[1] & ~2) | (joy[0].button[3].b ? 2 : 0);
    }

    if (joy[0].stick[0].axis[0].d1)
      joystick_x[0] = -10000;
    else if (joy[0].stick[0].axis[0].d2)
      joystick_x[0] = +10000;
    else
      joystick_x[0] = 0;

    if (joy[0].stick[0].axis[1].d1)
      joystick_y[0] = -10000;
    else if (joy[0].stick[0].axis[1].d2)
      joystick_y[0] = +10000;
    else
      joystick_y[0] = 0;
  }

  if (num_joysticks >= 2) {
    joystick_b[1] = (joystick_b[1] & ~1) | (joy[1].button[0].b ? 1 : 0);
    joystick_b[1] = (joystick_b[1] & ~2) | (joy[1].button[1].b ? 2 : 0);

    if (joy[1].stick[0].axis[0].d1)
      joystick_x[1] = -10000;
    else if (joy[1].stick[0].axis[0].d2)
      joystick_x[1] = +10000;
    else
      joystick_x[1] = 0;

    if (joy[1].stick[0].axis[1].d1)
      joystick_y[1] = -10000;
    else if (joy[1].stick[0].axis[1].d2)
      joystick_y[1] = +10000;
    else
      joystick_y[1] = 0;
  }

  if (num_joysticks >= 1)
    dmsg (D_JOYSTICK, "joystick #1 state: x=%d y=%d b=%d",
	  joystick_x[0], joystick_y[0], joystick_b[0]);
  if (num_joysticks >= 2)
    dmsg (D_JOYSTICK, "joystick #2 state: x=%d y=%d b=%d",
	  joystick_x[1], joystick_y[1], joystick_b[1]);
}
