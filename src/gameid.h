/*------------------------------------------------------------------.
| Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
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

#ifndef HEROES_GAMEID_H
#define HEROES_GAMEID_H

/*
 *   Game IDs are used to identify a game in the scores and
 * saved-games files.  This is needed, because when a player continue a
 * saved game which alrady have a high-score, this high-score should
 * be updated on the next save, rather than creating a new score.
 *
 *   Older versions of Heroes (up to 0.9) used to associate a 8 bit
 * value (called `magic' for the sake of a better name) to each game.
 * The list of used magics was scanned whenever a new magic was needed,
 * and the first unused value was selected.
 *
 *   This approach has one drawback: you need to know all the values
 * used to select a new one which doesn't clash.  If you consider the
 * case where there is a global score file for all user, but local
 * savedgames files in each user HOME dir, it cannot work: you
 * would need the scan the savedgames file of each user.
 *
 *   The present implementation represents Game IDs as 128bits values.
 * Those values are initialized randomly, and actually they are not
 * checked against already used values.  Even if this latter check
 * was done this would ont be bulletproof (the case described in the
 * above paragraph can still cause clashes), so I consider it's not
 * worth the pain since the width of the value make conflicts extremly
 * unlikely anyway.  Other ideas are welcome!
 */

#define GAMEID_SIZE 4
typedef a_u32 a_gameid[GAMEID_SIZE];
typedef a_u32 *gameid_ptr;

void create_gameid (gameid_ptr gid);
void empty_gameid (gameid_ptr gid);
bool equal_gameid (const gameid_ptr gid1, const gameid_ptr gid2);
void copy_gameid (gameid_ptr gid1, const gameid_ptr gid2);

char *gameid_to_text (const gameid_ptr gid);

/* Return true on error.  */
bool text_to_gameid (const char *src, gameid_ptr gid);

#endif /* HEROES_GAMEID_H */
