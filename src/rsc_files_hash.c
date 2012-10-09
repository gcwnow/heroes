/* ANSI-C code produced by gperf version 2.7.2 */
/* Command-line: gperf -t -L ANSI-C -G -N in_rsc_set -W rsc_set -k '1,3,7,$' ./rsc_files.gperf  */
/*------------------------------------------------------------------------.
| Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
|                                                                         |
| This file is part of Heroes.                                            |
|                                                                         |
| Heroes is free software; you can redistribute it and/or modify it under |
| the terms of the GNU General Public License as published by the Free    |
| Software Foundation; either version 2 of the License, or (at your       |
| option) any later version.                                              |
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
#include "rsc_files_hash.h"
#include "fstrcmp.h"
struct rsc_file;

#define TOTAL_KEYWORDS 38
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 21
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 91
/* maximum key range = 85, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92,  0, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 10, 10, 15,
       5,  0, 50,  0,  0,  0, 20,  0, 25, 45,
       0,  0, 40, 92,  0,  0,  0,  0, 10,  5,
      45, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92
    };
  register int hval = len;

  switch (hval)
    {
      default:
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      case 6:
      case 5:
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

static struct rsc_file rsc_set[] =
  {
    {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false},
    {"sys-dir",		"/var/games/heroes",			0,0,true},
    {"user-dir",		"$(home-dir)/.heroes",			0,0,true},
    {"",0,0,0,false},
    {"editor-img",		"$(data-dir)/pics/edit.pcx",		0,0,true},
    {"",0,0,0,false}, {"",0,0,0,false},
    {"data-dir",		"$(prefix)/"FORWARD_RELATIVE_PKGDATADIR,0,0,true},
    {"",0,0,0,false},
    {"intro-erase-img",	"$(data-dir)/pics/erase.pcx",		0,0,true},
    {"intro-splash-img",	"$(data-dir)/pics/heroes.pcx",		0,0,true},
    {"",0,0,0,false},
    {"big-font",		"$(data-dir)/pics/fontbig.pcx",		0,0,true},
    {"arrow-img",		"$(data-dir)/pics/arrow.pcx",		0,0,true},
    {"trails-img",		"$(data-dir)/pics/trail.pcx",		0,0,true},
    {"",0,0,0,false},
    {"vehicles-img",		"$(data-dir)/pics/vehic.pcx",		0,0,true},
    {"hedlite-output-dir",	"$(user-dir)/levels/",			0,0,true},
    {"end-scroller-bg-img",	"$(data-dir)/pics/background.pcx",	0,0,true},
    {"",0,0,0,false},
    {"saved-games-file",	"$(user-dir)/savedgames",		0,0,true},
    {"",0,0,0,false},
    {"intro-vehicles-img",	"$(data-dir)/pics/intro.pcx",		0,0,true},
    {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false},
    {"new-level-menu-img",	"$(data-dir)/pics/newlevel.pcx",	0,0,true},
    {"help-font",		"$(data-dir)/pics/fontread.pcx",	0,0,true},
    {"brown-bonus-img",	"$(data-dir)/pics/bonusb.pcx",		0,0,true},
    {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false},
    {"tiles-sets-dir",		"$(data-dir)/tilesets/",		0,0,true},
    {"intro-logos-img",	"$(data-dir)/pics/olympus.pcx",		0,0,true},
    {"extra-levels-dir",	"$(data-dir)/extralvl/",		0,0,true},
    {"help-pictures-img",	"$(data-dir)/pics/helpics.pcx",		0,0,true},
    {"",0,0,0,false}, {"",0,0,0,false},
    {"levels-dir",		"$(data-dir)/levels/",			0,0,true},
    {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false},
    {"pref-file",		"$(user-dir)/preferences",		0,0,true},
    {"locale-dir",	        "$(prefix)/"FORWARD_RELATIVE_LOCALEDIR, 0,0,true},
    {"",0,0,0,false},
    {"sfx-dir",		"$(data-dir)/sfx/",			0,0,true},
    {"home-dir",		"is initialized from $HOME",		0,0,true},
    {"main-font",		"$(data-dir)/pics/fontem.pcx",		0,0,true},
    {"",0,0,0,false},
    {"purple-bonus-img",	"$(data-dir)/pics/bonusa.pcx",		0,0,true},
    {"sfx-conf-txt",		"$(data-dir)/sfx/sfx.conf",		0,0,true},
    {"",0,0,0,false},
    {"user-conf",		"$(user-dir)/heroesrc",			0,0,true},
    {"score-file",		"$(user-dir)/scores",			0,0,true},
    {"system-conf",		"$(data-dir)/etc/heroesrc",		0,0,true},
    {"menu-pictures-img",	"$(data-dir)/pics/fontem2.pcx",		0,0,true},
    {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false},
    {"editor-preview-prefix",  "$(data-dir)/pics/editp",		0,0,true},
    {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false},
    {"bonus-font",		"$(data-dir)/pics/typonus.pcx",		0,0,true},
    {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false},
    {"jukebox-img",		"$(data-dir)/pics/jukebox.pcx",		0,0,true},
    {"jukebox-font",		"$(data-dir)/pics/fontdeck.pcx",	0,0,true},
    {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false},
    {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false}, {"",0,0,0,false},
    {"prefix",			PREFIX,					0,0,true}
  };

#ifdef __GNUC__
__inline
#endif
struct rsc_file *
in_rsc_set (register const char *str, register unsigned int len)
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = rsc_set[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &rsc_set[key];
        }
    }
  return 0;
}

void
print_rsc_files (void)
{
  int i;
  for (i = MIN_HASH_VALUE; i <= MAX_HASH_VALUE; ++i) {
    if (rsc_set[i].name[0]) {
      if (rsc_set[i].modified_value) {
	printf ("%24s = %s\n", rsc_set[i].name, rsc_set[i].modified_value);
	printf ("%16s (default: %s)\n", "", rsc_set[i].value);
      } else
	printf ("%24s = %s\n", rsc_set[i].name, rsc_set[i].value);
    }
  }
}

const char *
get_rsc_approx (const char *rsc_name)
{
  int i;
  const char *res = 0;
  double best_weight = 0.7;

  for (i = MIN_HASH_VALUE; i <= MAX_HASH_VALUE; ++i) {
    if (rsc_set[i].name[0]) {
      double weight = fstrcmp (rsc_name, rsc_set[i].name);
      if (weight > best_weight) {
        best_weight = weight;
	res = rsc_set[i].name;
      }
    }
  }
  return res;
}

void
free_modified_rsc (void)
{
  int i;
  for (i = MIN_HASH_VALUE; i <= MAX_HASH_VALUE; ++i)
    XFREE (rsc_set[i].modified_value);
}
