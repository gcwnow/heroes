/*------------------------------------------------------------------------.
| Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
|                                                                         |
| This file is part of Heroes.                                            |
|                                                                         |
| Heroes is free software; you can redistribute it and/or modify it under |
| the terms of the GNU General Public License version 2 as published by   |
| the Free Software Foundation.                                           |
|                                                                         |
| Heroes is distributed in the hope that it will be useful, but WITHOUT   |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   |
| for more details.                                                       |
|                                                                         |
| You should have received a copy of the GNU General Public License along |
| with this program; if not, write to the Free Software Foundation, Inc., |
| 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA                   |
`------------------------------------------------------------------------*/

#include "system.h"
#include "fader.h"
#include "debugmsg.h"
#include "scrtools.h"
#include "timer.h"

/*
 *  Either we fade in to a palette (F_IN),
 *  or we fade out from a palette (F_OUT),
 *  or we fade from one palette to another (F_PAL),
 *  or we do nothing (F_NONE)..
 */
enum fader_kind {F_NONE, F_IN, F_OUT, F_PAL};

/*
 *  The color to fade to (F_OUT) or from (F_IN).
 */
enum fader_color {F_BLACK = 0, F_WHITE};


struct fader_s {
  const a_palette*	from;	/* palette to fade from (if needed) */
  const a_palette*	to;	/* palette to fade to (if needed) */
  enum fader_kind	kind;
  enum fader_color	color;
  a_timer		timer;	/* timer used by the fade */
  int			duration; /* Number of steps to use for fading
				     (step are incremented using
				     the timer speed) */
  int			last_step; /* last step where palette was set */
  a_fader_status*	status_ptr; /* status variable to update */
  long			delay;	/* step to wait */
};

static struct fader_s fader;

void
init_fader (void)
{
  dmsg (D_FADER, "initialize fader");
  fader.from = 0;
  fader.to = 0;
  fader.kind = F_NONE;
  fader.color = F_BLACK;
  fader.timer = new_htimer (T_LOCAL, HZ (70));
  fader.duration = 0;
  fader.last_step = -1;
  fader.status_ptr = 0;
  fader.delay = 0;
}

void
uninit_fader (void)
{
  dmsg (D_FADER, "uninitialize fader");
  free_htimer (fader.timer);
  fader.timer = 0;
}

static void
set_fader (const a_palette* from, const a_palette* to,
	   enum fader_kind kind, enum fader_color color, int duration)
{
  fader.from = from;
  fader.to = to;
  fader.kind = kind;
  fader.color = color;
  fader.duration = duration;
  fader.last_step = -1;
  fader.status_ptr = 0;
}

void
std_white_fadein (const a_palette* to)
{
  dmsg (D_FADER, "set standard white fade-in");
  set_fader (0, to, F_IN, F_WHITE, 64);
}

void
std_black_fadein (const a_palette* to)
{
  dmsg (D_FADER, "set standard black fade-in");
  set_fader (0, to, F_IN, F_BLACK, 64);
}

void
std_black_fadeout (const a_palette* from)
{
  dmsg (D_FADER, "set standard black fade-out");
  set_fader (from, 0, F_OUT, F_BLACK, 64);
}

void
std_palette_fade (const a_palette* from, const a_palette* to)
{
  dmsg (D_FADER, "set standard palette fade");
  set_fader (from, to, F_PAL, 0, 64);
}

void
run_fader (void)
{
  int last_step = fader.last_step;
  a_palette f_pal;

  if (fader.kind == F_NONE || last_step >= fader.duration)
    return;

  if (last_step < 0) {
    /* first run of this fader */
    dmsg (D_FADER, "fader: first step");
    reset_htimer (fader.timer);
    fader.last_step = 0;
    if (fader.status_ptr)
      *fader.status_ptr = F_STARTED;
  }

  /* Handle a delay, set by fader_delay (). */
  if (fader.delay > 0) {
    fader.delay -= read_htimer (fader.timer);
    if (fader.delay > 0)
      return;
    /* fader.delay now can be negative if we waited too much */
    if (fader.delay < 0) {
      fader.last_step -= fader.delay;
      fader.delay = 0;
    }
  } else /* fader.delay == 0 */ {
    fader.last_step += read_htimer (fader.timer);
  }

  if (fader.last_step > fader.duration)
    fader.last_step = fader.duration;
  dmsg (D_FADER, "fader: last_step=%d", fader.last_step);

  /* avoid setting palette when possible */
  if (last_step == fader.last_step)
    return;

  switch (fader.kind) {
    unsigned i;
    const unsigned char* in;
    const unsigned char* out;

  case F_IN:
    out = fader.to->global;
    if (fader.color == F_BLACK)
      for (i = 0; i < 768; ++i)
	f_pal.global[i] = (out[i] * fader.last_step) / fader.duration;
    else
      for (i = 0; i < 768; ++i)
	f_pal.global[i] = ((63 * (fader.duration - fader.last_step)) +
			   (out[i] * fader.last_step)) / fader.duration;
    break;

  case F_OUT:
    in = fader.from->global;
    if (fader.color == F_BLACK)
      for (i = 0; i < 768; ++i)
	f_pal.global[i] = (in[i] * (fader.duration - fader.last_step))
	  / fader.duration;
    else
      for (i = 0; i < 768; ++i)
	f_pal.global[i] = ((in[i] * (fader.duration - fader.last_step))
			   + (63 * fader.last_step)) / fader.duration;
    break;

  case F_PAL:
    in = fader.from->global;
    out = fader.to->global;
    for (i = 0; i < 768; ++i)
      f_pal.global[i] = ((in[i] * (fader.duration - fader.last_step)) +
			 (out[i] * fader.last_step)) / fader.duration;
    break;
  case F_NONE:
    return;
  }

  if (fader.last_step == fader.duration) {
    /* fader finished its work */
    cancel_fader ();
  }

  set_pal_with_luminance (&f_pal);
}

void
cancel_fader (void)
{
  fader.kind = F_NONE;
  if (fader.status_ptr)
    *fader.status_ptr = F_FINISHED;
}

void
fader_status_flagback (a_fader_status* ptr)
{
  fader.status_ptr = ptr;
  if (ptr)
    *ptr = F_UNKNOWN;
}

void
fader_delay (long s)
{
  fader.delay = s;
}
