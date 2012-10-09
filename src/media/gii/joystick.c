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
#include <ggi/gii.h>

int joystick_x[2] = { 0, 0 };	/* coord. X */
int joystick_y[2] = { 0, 0 };	/*        Y */
char joystick_b[2] = { 0, 0 };	/* buttons (on 2bits) */
char joystick_detected = 0;

gii_input_t joystick;

static void
_get_joystick_state (void)
{
  struct timeval t = { 0, 0 };

  if (giiEventPoll (joystick,
		    emValAbsolute | emKeyPress | emKeyRelease, &t) != emZero) {
    int nbr;
    gii_event ev;

    nbr = giiEventsQueued (joystick,
			   emValAbsolute | emKeyPress | emKeyRelease);
    for (; nbr; --nbr) {
      giiEventRead (joystick, &ev,
		    emValAbsolute | emKeyPress | emKeyRelease);
      if (ev.any.type == evKeyPress) {
  	dmsg (D_JOYSTICK, "joystick button #%d pressed\n", ev.key.button);
	switch (ev.key.button) {
	case 1:
	  joystick_b[0] |= 1;
	  break;
	case 2:
	  joystick_b[0] |= 2;
	  break;
	case 3:
	  joystick_b[1] |= 1;
	  break;
	case 4:
	  joystick_b[1] |= 2;
	  break;
	}
      } else if (ev.any.type == evKeyRelease) {
  	dmsg (D_JOYSTICK, "joystick button #%d released\n", ev.key.button);
	switch (ev.key.button) {
	case 1:
	  joystick_b[0] &= ~1;
	  break;
	case 2:
	  joystick_b[0] &= ~2;
	  break;
	case 3:
	  joystick_b[1] &= ~1;
	  break;
	case 4:
	  joystick_b[1] &= ~2;
	  break;
	}
      } else if (ev.any.type == evValAbsolute) {
	unsigned int i;
	for (i = ev.val.first; i < ev.val.first + ev.val.count; ++i)
	  dmsg (D_JOYSTICK, "joystick valuator #%u = %d", i,
		ev.val.value[i - ev.val.first]);
	if (0 == ev.val.first)
	  joystick_x[0] = ev.val.value[0];
	if (1 >= ev.val.first && 1 <= ev.val.first + ev.val.count)
	  joystick_y[0] = ev.val.value[1 - ev.val.first];
	if (2 >= ev.val.first && 2 <= ev.val.first + ev.val.count)
	  joystick_x[1] = ev.val.value[2 - ev.val.first];
	if (3 >= ev.val.first && 3 <= ev.val.first + ev.val.count)
	  joystick_y[1] = ev.val.value[3 - ev.val.first];
      } else {
	wmsg (_("unexpected event %d\n"), ev.any.type);
      }
    }
  }
}

char
joyinit (void)
{
  dmsg (D_JOYSTICK, "initialize joystick");
  giiInit ();
  joystick = giiOpen ("linux-joy",NULL);
  if (!joystick) {
    wmsg (_("No joystick found (run with `-J' to suppress this message)."));
    joystick_detected = 0;
  } else
    joystick_detected = 1;

  return (joystick_detected);
}

/* Update the state of the joysticks */
void
get_joystick_state (void)
{
  if (joystick_detected)
    _get_joystick_state ();
}
