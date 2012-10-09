/*				 -*- buffer-read-only: t -*- vi: set ro:
** 
** DO NOT EDIT THIS FILE   (src/people.c)
** 
** It has been AutoGen-ed  Saturday March 30, 2002 at 04:43:41 PM CET
** From the definitions    src/people.def
** and the template file   people
*/
/*
** Heroes is free software.
** 
** You may redistribute it and/or modify it under the terms of the
** GNU General Public License, as published by the Free Software
** Foundation; either version 2, or (at your option) any later version.
** 
** Heroes is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with Heroes.  See the file "COPYING".  If not,
** write to:  The Free Software Foundation, Inc.,
**            59 Temple Place - Suite 330,
**            Boston,  MA  02111-1307, USA.
*/
#include "system.h"
#include "people.h"
#include "misc.h"

typedef struct a_strpair a_strpair;
struct a_strpair {
  const char*	str;
  bool		i18n;
};

const a_strpair people_strings[] = {
  { "\n", false },
  { N_("%{center}You owe this game to the following people"), true },
  { "\n%{head 1}", false },
  { N_("Authors\n"), true },
  { "\n", false },
  { "%{>>>}Alexandre Duret-Lutz\n", false },
  { "%{>>>}Romual Genevois\n", false },
  { "%{>>>}Alexandre Liverneaux\n", false },
  { "%{>>>}Philippe Meisburger\n", false },

  { "\n%{head 2}", false },
  { N_("Contributors\n"), true },
  { "\n", false },
  { "%{>>>}Pierre Baillet\n", false },
  { "%{>>>}Uwe Hermann\n", false },
  { "%{>>>}Ingo van Lil\n", false },

  { "\n%{head 3}", false },
  { N_("Translators\n"), true },
  { "\n", false },
  { "%{>>>}Hermann J. Beackers (%b", false },
  { N_("German"), true },
  { "%w)\n", false },
  { "%{>>>}Jos Boersema (%b", false },
  { N_("Dutch"), true },
  { "%w)\n", false },
  { "%{>>>}Luzemario Dantas Rocha (%b", false },
  { N_("Brazilian Portuguese"), true },
  { "%w)\n", false },
  { "%{>>>}Alessandro Dotti (%b", false },
  { N_("Italian"), true },
  { "%w)\n", false },
  { "%{>>>}Alexandre Duret-Lutz (%b", false },
  { N_("French"), true },
  { "%w)\n", false },
  { "%{>>>}Ingo van Lil (%b", false },
  { N_("German"), true },
  { "%w)\n", false },

  { "\n%{head 4}", false },
  { N_("Packagers\n"), true },
  { "\n", false },
  { "%{>>>}Daniel Burrows (%bDebian GNU/Linux%w)\n", false },
  { "%{>>>}Jean Delvare (%bRPM & Slackware Linux%w)\n", false },
  { "%{>>>}Eugenia Loli-Queru (%bBeOS%w)\n", false },
  { "%{>>>}Pascal Rigaux (%bLinux-Mandrake%w)\n", false },
  { "%{>>>}Maxim Sobolev (%bFreeBSD%w)\n", false },

  { "\n%{head 1}", false },
  { N_("Porters\n"), true },
  { "\n", false },
  { "%{>>>}Stephane Denis (%bWin/Visual C%w)\n", false },
  { "%{>>>}Mattias Engdegaard (%bSolaris%w)\n", false },
  { "%{>>>}Maxim Sobolev (%bFreeBSD%w)\n", false },

  { "\n%{head 2}", false },
  { N_("Other people we wish to thanks\n"), true },
  { "\n", false },

  { "%{>>>}Todd Allendorf\n", false },

  { "%{>>>}William Black\n", false },

  { "%{>>>}Beiad Ian Q. Dalton\n", false },

  { "%{>>>}Laurent Duperval\n", false },

  { "%{>>>}Christoph Egger\n", false },

  { "%{>>>}Gregory Giannoni\n", false },

  { "%{>>>}Janos Holanyi\n", false },

  { "%{>>>}Wolfgang Holzinger\n", false },

  { "%{>>>}Brian S. Julin\n", false },

  { "%{>>>}Jesper Kjolsrud\n", false },

  { "%{>>>}David Olofson\n", false },

  { "%{>>>}Raphael Poss\n", false },

  { "%{>>>}Thomas Pospisek\n", false },

  { "%{>>>}Benjamin Preidecker\n", false },

  { "%{>>>}Christian T. Steigies\n", false },

  { "%{>>>}Eero Tamminen\n", false },

  { 0, 0 }
};

a_read_data *
compile_people (void)
{
  a_read_data *p = 0;
  char *s = 0;
  const a_strpair *r = people_strings;
  s = xstrdup (r->i18n ? _(r->str) : r->str);
  while ((++r)->str)
    s = strappend (s, r->i18n ? _(r->str) : r->str);
  p = compile_reader_data (p, s);
  free (s);
  return p;
}

