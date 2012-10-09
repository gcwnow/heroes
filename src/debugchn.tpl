[~ autogen5 template
h
gperf=debughash.gperf

# Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>
#
# This file is part of Heroes.
#
# Heroes is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# Heroes is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
~]
[~ CASE (suffix) ~]
[~ == h ~][~ (dne "** " "/*\t\t\t\t") ~]
*/

/*
[~ (gpl "Heroes" "** ") ~]
*/
[~ (begin
     (define *chn_counter* 1)
     (define (chn_counter)
       (let ((counter *chn_counter*))
	 (set! *chn_counter* (* counter 2))
	 counter))) ~]
typedef enum debug_lvl { [~ FOR channel ',' ~]
  D_[~
   (sprintf "%-16s = %6d  /* %s */"
     (string-upcase! (get "name"))
     (chn_counter)
     (get "descr")) ~][~
  ENDFOR channel ~]
} debug_lvl;
[~ == gperf ~]%{
[~ (dne "** " "/*\t\t\t\t") ~]
*/

/*
[~ (gpl "Heroes" "** ") ~]
*/

#include "system.h"
#include "debughash.h"
#include "errors.h"
#include "fstrcmp.h"

/* prototype in_channel_set as static */
static struct debug_channel_s *
in_channel_set (register const char *str, register unsigned int len);
%}
struct debug_channel_s {
  const char *name;
  const char *doc;
  enum debug_lvl number;
};
%%
[~ (define (print-line name descr enum)
     (sprintf "%-17s N_(\"%-40s %s"
       (string-append name ",")
       (string-append descr "\"),")
       (string-upcase! enum))) ~][~
   (print-line "all" "all messages" "-1") ~]
[~ FOR channel '' ~][~
  (print-line (get "name") (get "descr") (string-append "D_" (get "name"))) ~]
[~ ENDFOR ~]%%
static const struct debug_channel_s *
get_channel_approx (const char *name)
{
  int i;
  const struct debug_channel_s *res = 0;
  double best_weight = 0.7;

  for (i = MIN_HASH_VALUE; i <= MAX_HASH_VALUE; ++i) {
    if (channel_set[i].name[0]) {
      double weight = fstrcmp (name, channel_set[i].name);
      if (weight > best_weight) {
        best_weight = weight;
        res = &(channel_set[i]);
      }
    }
  }
  return res;
}

enum debug_lvl
get_channel_number (const char *name)
{
  const struct debug_channel_s *res = in_channel_set (name, strlen (name));
  if (!res) {
    /* try to find a channel which name is close to the given name */
    res = get_channel_approx (name);
    if (res)
      wmsg (_("%s: no such channel, assuming you meant '%s'."),
            name, res->name);
    else
      emsg (_("%s: no such channel (--list=debug will list "
              "all known channels)"), name);
  }
  return res->number;
}

void
print_debug_channels (void)
{
  int i;
  for (i = MIN_HASH_VALUE; i <= MAX_HASH_VALUE; ++i) {
    if (channel_set[i].name[0]) {
      printf ("%-16s %s\n", channel_set[i].name, _(channel_set[i].doc));
    }
  }
}
[~ == texi ~][~ (import-head "DEBUGCHN") ~]
@table @samp[~ FOR channel '' ~]
@item [~ (string-upcase! (get "name")) ~]
[~ ? doc (get "doc") (string-append (string-capitalize (get "descr")) ".") ~]
[~ ENDFOR ~]
@item ALL
All of the above.
@end table
[~ (import-tail "DEBUGCHN") ~][~ ESAC ~]
