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
#include "pendulum.h"
#include "timer.h"

/* An amortized pendulum, approximed using Euler's method.
 *
 *     +
 *     |\
 *     | \
 *     |  \
 *     |   \
 *     |_./ \
 *     |theta\
 *     |      \
 *     |       *
 *     |
 *
 *  The pendulum starts with theta(0) = PI/2, and then
 *  swings down around 0 until it is stopped.
 *
 *  Theta is a function of the time, and satisfies the
 *  following differential equation:
 *
 *    theta(t)'' = - (B * theta(t)' + A * sin(theta(t)))
 *
 *  where A and B are two arbitray constants.  (In physics
 *  they would depend on the gravitation, and the weight of
 *  the pendulum, but in Heroes we will take whatever values
 *  produce a good result on screen.)
 *
 *  This is a second-order differential equation, so we can't
 *  really apply Euler's method on it.  However, things get
 *  easier if we break it into the two first-order equations
 *  below:
 *
 *    omega(t)' = - (B * omega(t) + A * sin(theta(t)))
 *    theta(t)' = omega(t)
 *
 *  Let's call a(0), a(1), ... a(N) the sequence of approximations
 *  of theta, and b(0), b(1), ... b(N) the approxumations for omega.
 *
 *  If we know a(n) and b(n), we can apply Euler's formula on
 *  our first equation to compute b(n+1):
 *
 *   b(n+1) = b(n) - h * (B * b(n) - A * sin(a(n))
 *
 *  where h is the step in Euler's formula.
 *  Applying Euler's formula to the second equation gives:
 *
 *   a(n+1) = a(n) + h * b(n)
 *
 *  So finally we have an algorithm which allows us to compute a(n):
 *    Initialization:
 *       a(0) = PI/2      -- starting angle
 *       b(0) = 0         -- starting speed
 *    For n = 0 .. N:
 *       b(n+1) = b(n) - h * (B * b(n) - A * sin(a(n))
 *       a(n+1) = a(n) + h * b(n)
 */

struct a_pendulum {
  double theta;			/* a(n) above */
  double theta_prime;		/* b(n) above */
#define EULER_STEP 0.006	/* h above */
#define NBR_STEPS 10		/* we make many small steps rather
				   than one big for more accuracy */
  a_timer pendulum_timer;
  unsigned elapsed_time;	/* Number of step performed. */
};

a_pendulum*
pendulum_create (void)
{
  NEW (a_pendulum, p);
  p->theta = 3.1415926535 / 2;
  p->theta_prime = 0;
  p->pendulum_timer = new_htimer (T_LOCAL, HZ (70));
  p->elapsed_time = 0;
  return p;
}

static void
pendulum_one_step (a_pendulum *p)
{
  double aux;
  /* We use  B = 0.33 and A = 1 here. */
  aux = EULER_STEP * (0.33 * p->theta_prime + sin (p->theta));
  p->theta += EULER_STEP * p->theta_prime;
  p->theta_prime -= aux;
}

unsigned
pendulum_update (a_pendulum *p, int *angle256)
{
  int i;
  int n = read_htimer (p->pendulum_timer);

  p->elapsed_time += NBR_STEPS * n;

  /* Make many smalls step rather than one big step for more accuracy. */
  for (i = NBR_STEPS * n; i; --i)
    pendulum_one_step (p);

  /* ANGLE must be an integer between -256 and +256. */
  *angle256 = floor (p->theta * 512.0 / 3.1415926535);

  return p->elapsed_time;
}

void
pendulum_destroy (a_pendulum *p)
{
  free_htimer (p->pendulum_timer);
  free (p);
}
