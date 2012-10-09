/*------------------------------------------------------------------------.
| Copyright 2000  Alexandre Duret-Lutz <duret_g@epita.fr>                 |
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
#include "explosions.h"
#include "sprzcol.h"
#include "const.h"
#include "timer.h"
#include "debugmsg.h"

a_sprite *explosions[NBR_EXPLOSION_KINDS][NBR_EXPLOSION_FRAMES];

void
init_explosions (void)
{
  int i;
  for (i = 0; i < 6; ++i)
    explosions[0][i] = compile_sprzcol (IMGPOS (vehicles_img, 65, (5 - i)*33),
					0, 32, 32, vehicles_img.width, xbuf);
  for (; i < NBR_EXPLOSION_FRAMES; ++i)
    explosions[0][i] = compile_sprzcol (IMGPOS (vehicles_img, 32,
						(NBR_EXPLOSION_FRAMES - i - 1)
						* 33),
					0, 32, 32, vehicles_img.width, xbuf);
  for (i = 0; i < 6; ++i)
    explosions[1][i] = compile_sprzcol (IMGPOS (vehicles_img, 131, (5 - i)*33),
					0, 32, 32, vehicles_img.width, xbuf);
  for (; i < NBR_EXPLOSION_FRAMES; ++i)
    explosions[1][i] = compile_sprzcol (IMGPOS (vehicles_img, 98,
						(NBR_EXPLOSION_FRAMES - i - 1)
						* 33),
					0, 32, 32, vehicles_img.width, xbuf);
}

void
uninit_explosions (void)
{
  int i, j;
  for (i = 0; i < NBR_EXPLOSION_KINDS; ++i)
    for (j = 0; j < NBR_EXPLOSION_FRAMES; ++j)
      FREE_SPRITE0 (explosions[i][j]);
}

an_explosion *square_explo_state;
a_u8 *square_explo_type;

typedef struct an_explosion_info an_explosion_info;
struct an_explosion_info {
  long orig_time;		/* Date (in 70th of secs) when the explosion
				   was triggered.  */
  a_square_index idx;
  bool neighb_done;		/* True if the explosion has propagated
				   to neighbors.  */
};

static an_explosion_info *explo_list;
static unsigned int explo_list_max;
static unsigned int explo_list_first_unused;

static a_timer explo_timer;
static long explo_time;		/* Updated from explo_timer on each call
				   to update_explosion.  */

void
allocate_explosions (void)
{
  XSALLOC_ARRAY (square_explo_state, lvl.square_count, EXPLOSION_UNTRIGGERED);
  XMALLOC_ARRAY (square_explo_type, lvl.square_count);
  explo_list_max = 64;
  explo_list_first_unused = 0;
  XMALLOC_ARRAY (explo_list, explo_list_max);
  explo_timer = new_htimer (T_GLOBAL, HZ (70));
  explo_time = 0;
}

void
release_explosions (void)
{
  free_htimer (explo_timer);
  XFREE0 (square_explo_state);
  XFREE0 (square_explo_type);
  XFREE0 (explo_list);
}

static void trigger_explosion_at_time (a_square_index idx, long orig_time);


/* Usually, this will return values between EXPLOSION_TRIGGERED and 0.
   I.e., positive values that fit in the 'an_explosion' type.  But
   this function can also be used on very old explosions for which it
   returns a negative value.  Hence the 'int' return type.  */
static int
compute_explosion_state (long orig_time)
{
  /* Elapsed time in "frames".  */
  int ef = (explo_time - orig_time) / EXPLOSION_SLICES_PER_FRAMES;
  assert (ef >= 0);
  return EXPLOSION_TRIGGERED - ef;
}

/* Trigger neighbor squares if needed.

   NOTE: This function should be called only after EXPLOSION_DELAY has
   elapsed.  See the call in trigger_explosion_at_time() for example.
   */
static void
propagate_to_neighbors_maybe (a_square_index idx, long orig_time)
{
  /* Propagation to neighbors occurs EXPLOSION_DELAY 70th of seconds
     after the explosion has been triggered.  This is a precise
     instant in the life of an explosion, but since we may miss this
     instant (because of a low frame rate) we also need to be able to
     propagate explosions retro-actively at a subsequent time,
     counterbalancing the lag to have neighbors explosions behaving as
     if they were activated at the right time.

     This is simply achieved by calling trigger_explosion_at_time()
     with an ORIG_TIME based on the ORIG_TIME of the current explosion
     (i.e. no computations are based on the current time).  See the
     computation of NEIGH_ORIG_TIME.

     One may wonder why we care so much about counterbalancing the
     lag.  After all, the user won't notice if a neighboring square
     explodes one or two frames later.  Older versions of Heroes
     behaved this way.  There are two problems with this scheme:

     - It is non deterministic.  The propagation of the explosions
     depends on the frame rate achieved by the game.  From the same
     original state, different games with different frame rate with
     have different evolutions.  This will be a problem the day (the
     century?) Heroes become a networked game.

     - The frame rate can fall very very low.  Then funny effects can
     occur if we don't counterbalance.  For instance if the frame rate
     is so low that an explosion has the opportunity to activate its
     neighbor only less than EXPLOSION_DELAY/70 seconds before it
     vanish, then it will be in a triggerable state when this neighbor
     start to propagate to its own neighbors.  So our square will be
     activated by the neighbor it had activated and vice-versa: these
     two squares will explode alternatively and infinitely (or until
     the frame rate change).  */
  long neigh_orig_time =
    orig_time + ((EXPLOSION_TRIGGERED - EXPLOSION_TRIGGER_NEIGHBORS)
		 * EXPLOSION_SLICES_PER_FRAMES);

  /* Iterate on neighbor squares.  */
  a_dir d;
  for (d = 0; d < 4; ++d) {
    a_square_index ngb = lvl.square_move[d][idx];
    if (ngb != INVALID_INDEX && EXPLOSION_SQUARE_TRIGGERABLE_P (ngb))
      trigger_explosion_at_time (ngb, neigh_orig_time);
  }
}

static void
trigger_explosion_at_time (a_square_index idx, long orig_time)
{
  int state;
  bool propagated;

  /* We don't trigger explosions in the future.  */
  assert (explo_time >= orig_time);

  state = compute_explosion_state (orig_time);

  /* Maybe we need to propagate this explosion.  Note that STATE may
     happen to be negative, meaning the explosion vanished; we still
     try propagate.  */
  if (state <= EXPLOSION_TRIGGER_NEIGHBORS) {
    propagate_to_neighbors_maybe (idx, orig_time);
    propagated = true;
  } else {
    propagated = false;
  }

  /* Insert the explosion into the list of active explosions
     only if it's active...  */
  if (state >= 0) {
    if (explo_list_max <= explo_list_first_unused) {
      explo_list_max += 64;
      XREALLOC_ARRAY (explo_list, explo_list_max);
    }

    explo_list[explo_list_first_unused].orig_time = orig_time;
    explo_list[explo_list_first_unused].idx = idx;
    explo_list[explo_list_first_unused].neighb_done = propagated;
    ++explo_list_first_unused;

    /* Update level rendering information.  */
    square_explo_state[idx] = state;
    square_explo_type[idx] = rand () % NBR_EXPLOSION_KINDS;
  }
}

/* Trigger an explosion but set the ORIG_TIME so that the explosion
   is at frame FRAME_START now.  */
void
trigger_explosion (a_square_index idx, an_explosion frame_start)
{
  assert (frame_start <= EXPLOSION_TRIGGERED);

  trigger_explosion_at_time (idx, (explo_time
				   - ((EXPLOSION_TRIGGERED - frame_start)
				      * EXPLOSION_SLICES_PER_FRAMES)));
}

void
trigger_possible_explosion (a_square_index idx)
{
  if (EXPLOSION_SQUARE_TRIGGERABLE_P (idx))
    trigger_explosion (idx, EXPLOSION_TRIGGERED);
}

void
update_explosions (void)
{
  unsigned int i;
  /*
    Imagine that the array of explosions is as follow, where
    A is an active explosion, and V a vanished explosion.
       0A 1A 2V 3V 4V 5A 6A 7A 8A 9V 10V 11V...
    Since the V's are vanished, we want to remove them from the
    array.

    One idea is to shift the A's one by one as we process the
    array, overwriting the V's.  So, for instance, after processing
    the A at position 5, we would copy it at position 2. Similarly
    the A at position 6 would be copied at position 3, etc.

    However we can do better.  Granted, the explosions are not sorted
    over the ORIG_TIME: that why A's and V's can be mixed.  This is
    unfortunate because if we add all the V at the beginning, and the
    A's at the end and we could shift all the latter over the former in
    one shot (of memmove).  However, they are usually _almost_ sorted.
    Indeed: if we were only calling
      trigger_explosion (index, EXPLOSION_TRIGGERED)
    to append new explosions, they would be naturally kept in a
    chronological order.  The trouble comes from calls like
      trigger_explosion (index, EXPLOSION_IMMEDIATE)
    that creates an *older* explosion in order to have it exploding
    immediately (this is used when a player die, or with fire trails).
    This, combined with how the explosions are propagated means
    that the array is not sorted, but you can split this
    array in slices in which explosions are sorted.  The good thing
    is that these slices are easy to shift as explained above.
    Therefore, instead of shifting activated explosions one by one,
    we will shift them slice per slice.

    The two variables below are used for this purpose.  SHIFT_SRC
    is the first explosion of the last sequence of activated explosion.
    SHIFT_DEST is the for explosion of the last sequence of
    vanished explosions.  When we are processing 8A in the
    example above we have SHIFT_DEST=2 and SHIFT_SRC=5.  When
    we encounter 9V, we can shift all the A starting at SHIFT_SRC
    toward position SHIFT_SRC in a single shot.
  */
  int shift_src;
  int shift_dest;

  explo_time = read_htimer (explo_timer);

  if (explo_list_first_unused)
    dmsg (D_MISC, "explosions: %d/%d",
	  explo_list_first_unused, explo_list_max);

  shift_src = -1;
  shift_dest = 0;
  for (i = 0; i < explo_list_first_unused; ++i) {
    a_square_index idx = explo_list[i].idx;
    int state = compute_explosion_state (explo_list[i].orig_time);

    /* Propagate to neighbors.  */
    if (! explo_list[i].neighb_done
	&& state <= EXPLOSION_TRIGGER_NEIGHBORS) {
      propagate_to_neighbors_maybe (idx, explo_list[i].orig_time);
      explo_list[i].neighb_done = true;
    }

    if (state >= 0) {
      /* Update rendering info.  */
      square_explo_state[idx] = state;

      /* Mark position of for explosition in a sequence of
	 activated explosions.  */
      if (shift_src < 0)
	shift_src = i;
    } else {
      square_explo_state[idx] = EXPLOSION_UNTRIGGERED;

#define SHIFT_ACTIVATED_EXPLOSIONS				\
      /* Shift activated explosions over vanished ones.  */	\
      if (shift_src > shift_dest) {				\
	int activated_qty = i - shift_src;			\
	memmove (explo_list + shift_dest,			\
		 explo_list + shift_src,			\
		 activated_qty * sizeof (*explo_list));		\
	shift_dest += activated_qty;				\
	shift_src = -1;						\
      } else {							\
	shift_dest = i;						\
      }
      SHIFT_ACTIVATED_EXPLOSIONS;
    }
  }

  SHIFT_ACTIVATED_EXPLOSIONS;
  explo_list_first_unused = shift_dest;
}
