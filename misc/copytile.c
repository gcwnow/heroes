/* This program tests various tile_copying functions.

   Here are some results seen so far.

   Pentium 2   K6-2        Alpha       UltraSparc

   5.380000    2.820000    7.051600    3.260000
   5.030000    2.650000    9.738528    4.690000
   6.050000    3.160000    7.072096    3.260000
   17.930000   untested    untested    untested

   Pentium     G3

   33.251687   2.490000
   33.260924   2.630000
   50.832590   2.620000
   untested    untested
*/

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>

#define xbuf 512
#define buf 20*24*1000

typedef unsigned int  a_u32;
typedef unsigned char a_pixel;
a_pixel s[27 * 10 * 20 * 24];
a_pixel d[20 * 24 * 1000];

static void
copy_tile1 (const a_pixel* src, a_pixel* dest, int tx)
{
  const a_u32 *s = (const a_u32 *) src;
  a_u32 *d = (a_u32 *) dest;
  a_u32 t1, t2;
  int y;
  for (y = 20; y; --y) {        /* FIXME: Is this really faster than a */
    t1 = s[0];                  /* straight copy?  Need a benchmark  */
    t2 = s[3];
    d[0] = t1;
    d[3] = t2;
    t1 = s[1];
    t2 = s[4];
    d[1] = t1;
    d[4] = t2;
    t1 = s[2];
    t2 = s[5];
    d[2] = t1;
    d[5] = t2;
    s = (const a_u32 *) (((a_pixel *) s) + tx);
    d = (a_u32 *) (((a_pixel *) d) + xbuf);
  }
}

static void
copy_tile2 (const a_pixel* src, a_pixel* dest, int tx)
{
  const a_u32 *s = (const a_u32 *) src;
  a_u32 *d = (a_u32 *) dest;
  int y;
  for (y = 20; y; --y) {
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    d[4] = s[4];
    d[5] = s[5];
    s = (const a_u32 *) (((a_pixel *) s) + tx);
    d = (a_u32 *) (((a_pixel *) d) + xbuf);
  }
}

static void
copy_tile3 (const a_pixel* src, a_pixel* dest, int tx)
{
  const a_u32 *s = (const a_u32 *) src;
  const a_u32 *s2 = (const a_u32 *) (src + tx);
  a_u32 *d = (a_u32 *) dest;
  a_u32 *d2 = (a_u32 *) (dest + xbuf);
  int y;
  tx *= 2;
  for (y = 10; y; --y) {
    a_u32 t1 = s[0];
    a_u32 t2 = s2[0];
    d[0] = t1;
    d2[0] = t2;
    t1 = s[1];
    t2 = s2[1];
    d[1] = t1;
    d2[1] = t2;
    t1 = s[2];
    t2 = s2[2];
    d[2] = t1;
    d2[2] = t2;
    t1 = s[3];
    t2 = s2[3];
    d[3] = t1;
    d2[3] = t2;
    t1 = s[4];
    t2 = s2[4];
    d[4] = t1;
    d2[4] = t2;
    t1 = s[5];
    t2 = s2[5];
    d[5] = t1;
    d2[5] = t2;
    s = (const a_u32 *) (((const a_pixel *) s) + tx);
    s2 = (const a_u32 *) (((const a_pixel *) s2) + tx);
    d = (a_u32 *) (((a_pixel *) d) + 2 * xbuf);
    d2 = (a_u32 *) (((a_pixel *) d2) + 2 * xbuf);
  }
}

static void
copy_tile4 (const a_pixel* src, a_pixel* dest, int tx)
{
  int y;
  for (y = 20; y; --y) {
    memcpy (dest, src, 24);
    src += tx;
    dest += xbuf;
  }
}

/* Subtract the `struct timeval' values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0.  */

int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Perform the carry for the later subtraction by updating Y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     `tv_usec' is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

struct timeval utime;

void
start (void)
{
  struct rusage usage;
  getrusage (RUSAGE_SELF, &usage);
  utime = usage.ru_utime;
}

void
stop (void)
{
  struct timeval result;
  struct rusage usage;
  getrusage (RUSAGE_SELF, &usage);
  timeval_subtract (&result, &usage.ru_utime, &utime);
  printf ("%f\n", result.tv_sec * 1.0 + result.tv_usec / 1000000.0);
}


#define TIMES 5000000
int main()
{
  unsigned i, n;

  memset (s, 0, sizeof s);
  memset (d, 0, sizeof d);

  start();
  for (i = 1, n = 0; i < TIMES; i++) {
    copy_tile1 (s + (n * 547) % (27 * 10), d + (n % 1000) * 20 * 24, 27 * 24);
  }
  stop();

  start();
  for (i = 1, n = 0; i < TIMES; i++) {
    copy_tile2 (s + (n * 547) % (27 * 10), d + (n % 1000) * 20 * 24, 27 * 24);
  }
  stop();

  start();
  for (i = 1, n = 0; i < TIMES; i++) {
    copy_tile3 (s + (n * 547) % (27 * 10), d + (n % 1000) * 20 * 24, 27 * 24);
  }
  stop();

  start();
  for (i = 1, n = 0; i < TIMES; i++) {
    copy_tile4 (s + (n * 547) % (27 * 10), d + (n % 1000) * 20 * 24, 27 * 24);
  }
  stop();

  return 0;
}
