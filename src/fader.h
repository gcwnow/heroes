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

#ifndef HEROES__FADER__H
#define HEROES__FADER__H

#include "pcx.h"

/*
 * The fader is F_STARTED after the first call to run_fader () is
 * done.  It is F_FINISHED when the fading it was configured for is
 * finished (or canceled).  It is F_UNKNOWN between the call to
 * status_flagback and the next call to run_fader () which is either a
 * first or a last step.
 */
typedef enum {F_UNKNOWN, F_STARTED, F_FINISHED} a_fader_status;

/* Configure a standard fade-in, from white to the given palette,
   using 64 steps at 70Hz */
void std_white_fadein (const a_palette* to);

/* Configure a standard fade-in, from black to the given palette,
   using 64 steps at 70Hz */
void std_black_fadein (const a_palette* to);

/* Configure a standard fade-out, from the given palette to black,
   using 64 steps at 70Hz */
void std_black_fadeout (const a_palette* from);

/* Configure a standard fade between two palettes,
   using 64 steps at 70Hz */
void std_palette_fade (const a_palette* from, const a_palette* to);

/* update the display palette (if needed).
 * NOTE: 1) usualy, there is no need to call this directly, since it is
 *          done by vsynch ().
 *       2) fader use a htimer, therefore update_htimer () must be
 *          called periodically or the fader will never be updated.
 *          This is done by vsynch () too.
 */
void run_fader (void);

/*
 * cancel running fader
 */
void cancel_fader (void);

/*
 * Supply a pointer to a variable that will reflect the
 * status of the fader.  If ptr==0, nothing is done.
 *
 * NOTE: This pointer is always reset when the fader is configured.
 * WARNING: If this points to a local variable, you must not
 *          exit the local scope before the status is F_FINISHED; or
 *          if you really need to exit, do recall this function with ptr==0.
 * FIXME: This is better called before the first run_fader () call
 *        after configuration.  Otherwise the status may be "unknown".
 */
void fader_status_flagback (a_fader_status* ptr);

/*
 * Delay the fader for s steps.
 */
void fader_delay (long s);

void init_fader (void);
void uninit_fader (void);

#endif /* HEROES__FADER__H */
