/*------------------------------------------------------------------------.
| Copyright 1997, 1998, 2000  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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
#include "timer.h"
#include "video.h"
#include "debugmsg.h"

time_type current_time;

void
reset_htimer (a_timer timer)
{
#if HAVE_GETTIMEOFDAY
  /*-----------------------------------------------------------------.
  | NOTE: This function used to perform a struct assignment.  But it |
  | cause the gcc shipped with Red Hat 7.0 [gcc version 2.96         |
  | 20000731 (Red Hat Linux 7.0)] to produce unfaithful code when    |
  | inlining the call to reset_htimer() in read_htimer().  Using     |
  | member assignments appears to work around the bug HERE.          |
  `-----------------------------------------------------------------*/
  timer->orig_time.tv_sec = current_time.tv_sec;
  timer->orig_time.tv_usec = current_time.tv_usec;
#else
  timer->orig_time = current_time;
#endif
}

#if HAVE_GETTIMEOFDAY
/* Subtract the `struct timeval' values X and Z,
   storing the result in RES_SEC, RES_USEC (unless they are NULL pointers).
   Return 1 if the difference is negative, otherwise 0.
   (adapted from the GNU libc6 manual to not modify arguments, and return
   res_sec and res_usec separately)
*/
static int
timeval_subtract (long *res_sec, long *res_usec,
		  const time_type *x, const time_type *z)
{
  time_type y = *z;
  /* Perform the carry for the later subtraction by updating Y. */
  if (x->tv_usec < y.tv_usec) {
    int nsec = (y.tv_usec - x->tv_usec) / SECOND + 1;
    y.tv_usec -= SECOND * nsec;
    y.tv_sec += nsec;
  }
  if (x->tv_usec - y.tv_usec > SECOND) {
    int nsec = (x->tv_usec - y.tv_usec) / SECOND;
    y.tv_usec += SECOND * nsec;
    y.tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     `tv_usec' is certainly positive. */
  if (res_sec)
    *res_sec = x->tv_sec - y.tv_sec;
  if (res_usec)
    *res_usec = x->tv_usec - y.tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y.tv_sec;
}

static time_type last_time;	/* last time returned by gettimeofday */

/* make sure the times returned by gettimeofday are increasing...  */
static void
xgettimeofday (time_type *tv)
{
  time_type t;
  gettimeofday (&t, 0);
  if (timeval_subtract (0, 0, &t, &last_time))
    dmsg (D_TIMER,
	  "successive calls to gettimeofday() returned decreasing values");
  else
    last_time = t;
  *tv = last_time;
}
#endif

void
reset_htimer_with_offset (a_timer timer, long sec)
{
  reset_htimer (timer);

#ifdef HAVE_GETTIMEOFDAY
  timer->orig_time.tv_sec -= sec;
#else
  timer->orig_time -= sec * SECOND;
#endif
}

a_timer
new_htimer (enum htimer_kind kind, long slice_duration)
{
  a_timer result;
  XMALLOC_VAR (result);
  result->kind = kind;
  result->slice_duration = slice_duration;
  reset_htimer (result);
  dmsg (D_TIMER, "created new timer (kind=%d, slice_duration=%ld)=%p",
	kind, slice_duration, &result);
  return result;
}

void
free_htimer (a_timer timer)
{
  dmsg (D_TIMER, "free timer %p", timer);
  XFREE (timer);
}

void
update_htimers (void)
{
#ifdef HAVE_GETTIMEOFDAY
  xgettimeofday (&current_time);
#else
  current_time = get_current_time ();
#endif
}

void
init_htimer (void)
{
#if HAVE_GETTIMEOFDAY
  gettimeofday (&last_time, 0);
#endif
  update_htimers ();
}

long
read_htimer (a_timer timer)
{
#if HAVE_GETTIMEOFDAY
  long s, u, d, res;

  timeval_subtract (&s, &u, &current_time, &timer->orig_time);
  d = timer->slice_duration;

  for (;;) {
    /* The following formula computes `(s*SECOND+u)/d', trying to not
       overflow (obviously `s*SECOND+u' is likely to be too big) */
    res = (s*(SECOND/d)) + ((s*(SECOND%d))/d) + ((((s*(SECOND%d))%d)+u)/d);

    if ((res > 0) || (((timer->kind & T_BLOCKING) == 0)))
      break;
    else {			/* The timer is blocking */
      struct timeval present_time;
      xgettimeofday (&present_time);
      timeval_subtract (&s, &u, &present_time, &timer->orig_time);
    }
  }

  assert (res >= 0);

  if (timer->kind & T_LOCAL) {
    reset_htimer (timer);
    /* account for the time remaining from the last unfinished slice */
    timer->orig_time.tv_usec -= (((s*(SECOND%d))%d)+u)%d;
    while (timer->orig_time.tv_usec < 0) {
      timer->orig_time.tv_usec += SECOND;
      --timer->orig_time.tv_sec;
    }
  }
#else
  unsigned long c, d, res;

  c = current_time - timer->orig_time;
  d = timer->slice_duration;

  for (;;) {
    res = c / d;

    if ((res > 0) || (((timer->kind & T_BLOCKING) == 0)))
      break;
    else			/* The timer is blocking */
      c = get_current_time () - timer->orig_time;
  }

  if (timer->kind & T_LOCAL) {
    reset_htimer (timer);
    /* account for the time remaining from the last unfinished slice */
    timer->orig_time -= c % d;
  }
#endif

  dmsg (D_TIMER, "read timer %p, return %ld", timer, res);
  return res;
}

void
shift_htimer (a_timer to_shift, a_timer amount)
{
#if HAVE_GETTIMEOFDAY
  long u,s;

  /* compute the amount to shift the timer with */
  s = current_time.tv_sec - amount->orig_time.tv_sec;
  u = current_time.tv_usec - amount->orig_time.tv_usec;
  if (u < 0) {
    u += SECOND;
    --s;
  }

  /* add this amount from the timer's origin */
  to_shift->orig_time.tv_usec += u;
  to_shift->orig_time.tv_sec += s;
  if (to_shift->orig_time.tv_usec >= SECOND) {
    to_shift->orig_time.tv_usec -= SECOND;
    ++to_shift->orig_time.tv_sec;
  }
#else
  to_shift->orig_time += current_time - amount->orig_time;
#endif
  dmsg (D_TIMER, "timer %p shifted", to_shift);
}
