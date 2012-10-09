#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <ggi/ggi.h>
#include <assert.h>
#include "keyb_low.h"

int mouse_pos_x = 0;
int mouse_pos_y = 0;
char mouse_button_left = 0;
char mouse_button_right = 0;
char mouse_button_middle = 0;

extern ggi_visual_t visu;
static int enable_mouse = 0;

int
init_mouse (void)
{
  return 0;
}

void
mouse_show (void)
{
  enable_mouse = 1;
}

void
mouse_hide (void)
{
  enable_mouse = 0;
}

void
process_input_events (void)
{
  struct timeval t = { 0, 0 };

  unsigned int mask = emKeyPress | emKeyRelease | emKeyRepeat;

  if (ggiEventPoll (visu, mask, &t)
      != emZero) {
    int nbr;
    ggi_event ev;

    nbr = ggiEventsQueued (visu, mask);
    for (; nbr; --nbr) {
      ggiEventRead (visu, &ev, mask);
      switch (ev.any.type) {

	/* keboard events */

      case evKeyPress:
	assert (ev.key.label <= HKEY_MAX);
	keyboard_map[ev.key.label] = 1;
	keyboard_modifiers = ev.key.modifiers;
	break;
      case evKeyRelease:
	assert (ev.key.label <= HKEY_MAX);
	keyboard_map[ev.key.label] = 0;
	keyboard_modifiers = ev.key.modifiers;
	break;
      case evKeyRepeat:
	/* NOP */
	break;

      default:
	printf ("unexpected event %d\n", ev.any.type);
      }
    }
  }
}

void
update_mouse_state (void)
{
  struct timeval t = { 0, 0 };

  unsigned int mask = emPointer;

  /*
  if (!enable_mouse)
    return;
  */

  if (ggiEventPoll (visu, mask, &t)
      != emZero) {
    int nbr;
    ggi_event ev;

    nbr = ggiEventsQueued (visu, mask);
    for (; nbr; --nbr) {
      ggiEventRead (visu, &ev, mask);
      switch (ev.any.type) {

	/* mouse events */

      case evPtrAbsolute:
	mouse_pos_x = ev.pmove.x;
	mouse_pos_y = ev.pmove.y;
	break;
      case evPtrRelative:
	mouse_pos_x += ev.pmove.x;
	mouse_pos_y += ev.pmove.y;
	break;
      case evPtrButtonPress:
	if (ev.pbutton.button == GII_PBUTTON_LEFT)
	  mouse_button_left = 1;
	else if (ev.pbutton.button == GII_PBUTTON_RIGHT)
	  mouse_button_right = 1;
	else if (ev.pbutton.button == GII_PBUTTON_MIDDLE)
	  mouse_button_middle = 1;
	break;
      case evPtrButtonRelease:
	if (ev.pbutton.button == GII_PBUTTON_LEFT)
	  mouse_button_left = 0;
	else if (ev.pbutton.button == GII_PBUTTON_RIGHT)
	  mouse_button_right = 0;
	else if (ev.pbutton.button == GII_PBUTTON_MIDDLE)
	  mouse_button_middle = 0;
	break;

      default:
	printf ("unexpected event %d\n", ev.any.type);
      }
    }
  }
}

a_keycode
get_key (void)
{
  ggi_event ev;

  /* Block until we get a key. */
  ggiEventRead(visu, &ev, emKeyPress | emKeyRepeat);

  keyboard_modifiers = ev.key.modifiers;
  return ev.key.sym;
}

int
key_ready (void)
{
  return ggiKbhit (visu);
}

const char *
keys_pref_group (void)
{
  return "ggi";
}

int
keycode_to_ascii (a_keycode a)
{
  return a;
}
