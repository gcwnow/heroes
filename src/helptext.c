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
#include "helptext.h"
#include "misc.h"

const char *const help_text1 = N_("\
\n\
%{img C 444 0 40 320}\n\
\n\
\n\
\n\
\n\
%{center}%pUse the arrow keys to scroll up and down.\n\
\n\
%{head 1}CONTROL & BOARD\n\
\n\
%{img BC 357 55 80 205}\n\
%{textat cL -9 -81}%yPAUSE\n\
%{textat cR -9 97}%yQUIT GAME\n\
%{textat cR 26 -62}%yCONTROL\n\
%{textat cL 30 74}%ySTOP\n\
%{textat cL 52 68}%yTURBO\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
%{img BCG 486 0 140 320}\n\
%{textat cR -7 -82}%ySCORE\n\
%{textat cR 1 8}%yCOMMENTARY\n\
%{textat cL 16 29}%yHEROES\n\
%{textat cL 33 29}%yWALLS\n\
%{textat cL 55 29}%yBONUS\n\
%{textat cL 29 -102}%yLIFES\n\
%{textat cL 51 -109}%yTURBO ENERGY\n\
%{textat cL 72 -104}%yTIME\n\
%{textat cR 96 40}%yPLAYER COLOR\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
");

const char *const help_text2 = N_("\
\n\
\n\
\n\
%{head 2}BONUS\n\
\n\
%{img L 60 0 20 22}Affects the player that gets the item\n\
%{img L 60 22 20 22}Affects all players except the one who gets the bonus\n\
\n\
%{img L 0 0 20 22}%{>}%{img L 0 22 20 22}%{>>>}Trail +1\n\
%{img L 0 44 20 22}%{>}%{img L 0 66 20 22}%{>>>}Trail -1\n\
%{img L 0 88 20 22}%{>}%{img L 0 110 20 22}%{>>>}Speed Up\n\
%{img L 0 132 20 22}%{>}%{img L 0 154 20 22}%{>>>}Speed Down\n\
%{img L 0 176 20 22}%{>}%{img L 0 198 20 22}%{>>>}Random Bonus\n\
%{img L 0 220 20 22}%{>}%{img L 0 242 20 22}%{>>>}Points\n\
%{img L 0 264 20 22}%{>}%{img L 0 286 20 22}%{>>>}Fire Trail\n\
%{img L 20 0 20 22}%{>}%{img L 20 22 20 22}%{>>>}Stop\n\
%{img L 20 44 20 22}%{>}%{img L 20 66 20 22}%{>>>}Inverted controls\n\
%{img L 20 88 20 22}%{>}%{img L 20 110 20 22}%{>>>}Power +\n\
%{img L 20 132 20 22}%{>}%{img L 20 154 20 22}%{>>>}Power -\n\
%{img L 20 176 20 22}%{>}%{img L 20 198 20 22}%{>>>}End of Level\n\
%{img L 20 220 20 22}%{>}%{img L 20 242 20 22}%{>>>}Invulnerability\n\
%{img L 20 264 20 22}%{>}%{img L 20 286 20 22}%{>>>}Rotozoom\n\
%{img L 40 0 20 22}%{>}%{img L 40 22 20 22}%{>>>}Extra Life\n\
%{img L 40 44 20 22}%{>}%{img L 40 66 20 22}%{>>>}Wavy visions\n\
\n\
");

const char *const help_text3 = N_("\
%{head 3}GAME MODES\n\
\n\
%{>>}%yQUEST\n\
\n\
%{>>>}The %yQUEST%w mode allows you to play all 100 levels HEROES offers \
you.  Your aim is to collect a given number (10) of Trail+1 bonuses and \
then to get the End-of-Level bonus that will send you to next level.\n\
%{>>>}%{img L 20 176 20 22}End-of-Level\n\
%{img R 0 0 20 22}%{flushright}Trail+1\n\
%{>>>}Trail+1 has the effect of growing your trail.  There are two way to \
get it smaller: take a Trail-1 or lose a life.  The End-of-Level bonus \
first appears when someone's trail reach 10 units.\n\
\n\
%{>>}%yDEATH MATCH\n\
\n\
%{>>>}The %yDEATH MATCH%w mode allows you to face the other players in \
a fight to death.  The aim of the game is to be the last player still \
alive.  For this, feel free to use any stratagems you want.  Nothing is \
forbidden.\n\
\n\
%{img BLVr 130 9 30 57}%{>>}%yKILL THEM ALL\n\
\n\
%{>>>}The %yKILL THEM ALL%w mode has only one rule: You have to \
overwhelm all the people of your competitors.  It was said \"There can \
be only one\", make sure it is yours!\n\
\n\
%{img LG 131 70 20 100}\n\
%{img LG 151 70 20 80}They are among us, find them !\n\
\n\
%{>>}%yCOLOR\n\
\n\
%{>>>}The %yCOLOR%w mode is very simple. You have a certain amount of \
time to get a maximum of gems of your color.  Beware, if you go over \
your competitor's gems, it's your competitor's gem account that you \
will increase.  Be also sure not to get crossed gems because you'll \
lose collected gems.  Grey ones gives gems to all players except the one \
that goes over it.  The Clocks give you extra time.\n\
\n\
%{>>}%yTIME CA$H\n\
\n\
%{>>>}In %yTIME CA$H%w mode the winner is the player who collected \
the largest amount of money.\n\
\n\
");

const char *const help_text4 = N_("\
%{head 4}GAME OPTIONS\n\
\n\
%{img L 293 165 20 134}\n\
%{>>}%gCOLOR SELECT\n\
\n\
%{>>>}This option allows you to choose the color of your vehicle between \
%pPINK%w, %yYELLOW%w, %bBLUE%w and %gGREEN%w.\n\
\n\
%{img L 335 165 20 32}\n\
%{>>}%gGAME SPEED\n\
\n\
%{>>>}The %gGAME SPEED%w option allows you to select the initial speed of \
the shuttles.  The 3 available speeds are: NORMAL, EXPERT, HEROES.\n\
\n\
%{img L 314 264 20 32}\n\
%{>>}%gROUNDS\n\
\n\
%{>>>}The %gROUNDS%w option allows you to select the number of rounds in \
the tournament modes: DEATH MATCH, KILL'EM ALL, COLOR and TIME CA$H.  \
These values range from 1 to 100 rounds.\n\
\n\
%{img L 293 0 20 33}%{img L 293 132 20 33}\n\
%{>>}%gRADAR & INFORMATION\n\
\n\
%{>>>}You can disable the %gRADAR%w and the %gINFORMATIONS%w for better \
visibility.\n\
\n\
%{img L 314 231 20 32}\n\
%{>>}%gLIGHTING\n\
\n\
%{>>>}The %gLIGHTING%w option is here to tune the luminosity of your screen.\n\
\n\
%{img L 314 0 20 32}\n\
%{>>}%gINERTIA\n\
\n\
%{>>>}The %gINERTIA%w switch enables or disables the scrolling inertia.\n\
\n\
%{img L 293 99 20 32}\n\
%{>>}%gAUTOPILOT\n\
\n\
%{>>>}If you are mad, you can try to play without %gAUTOPILOT%w, \
rest in peace...  Note that %gAUTOPILOT%w does not work against fire trails.\n\
\n\
");

const char *const help_text5 = N_("\
%{head 1}CONTACT\n\
\n\
%{center}home page:\n\
%{img BCVr 183 124 30 41}\n\
%{center}http://heroes.sourceforge.net/\n\
\n\
%{center}send bug reports to:\n\
%{img BCVr 183 124 30 41}\n\
%{center}heroes-bugs@lists.sourceforge.net\n\
\n\
");

const char *const help_text6 = N_("\
%{img C 175 0 10 119}\n\
%{img L 96 200 40 50}%{img R 96 200 40 50} We take care of today's \
environmental and ecological issues and did our best to use only \
recycled bytes in this program.\n\
%{img C 175 0 10 119}\n\
\n\
\n\
%{img CG 145 150 40 120}\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
%{img C 220 3 60 130}\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
%{center}Copyright 1997, 1998, 2000  Alexandre Duret-Lutz, Romuald Genevois, \
Alexandre Liverneaux and Philippe Meisburger.\n\
\n\
");

a_read_data *
compile_helptext (void)
{
  a_read_data *p = 0;
  char *s = 0;
  s = xstrdup (_(help_text1));
  s = strappend (s, _(help_text2));
  s = strappend (s, _(help_text3));
  s = strappend (s, _(help_text4));
  s = strappend (s, _(help_text5));
  s = strappend (s, _(help_text6));
  p = compile_reader_data (p, s);
  free (s);
  return p;
}
