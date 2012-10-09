/*------------------------------------------------------------------.
| Copyright 1997, 1998, 2000, 2001  Alexandre Duret-Lutz            |
|                                    <duret_g@epita.fr>             |
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
#include "video.h"
#include "pcx.h"
#include "keyb.h"
#include "keyvalues.h"
#include "joystick.h"
#include "errors.h"
#include "fastmem.h"
#include "sfx.h"
#include "prefs.h"
#include "scores.h"
#include "savegame.h"
#include "intro.h"
#include "menus.h"
#include "extras.h"
#include "visuals.h"
#include "render.h"
#include "renderdata.h"
#include "pixelize.h"
#include "misc.h"
#include "argv.h"
#include "debugmsg.h"

#include "strack.h"
#include "endscroll.h"

#include "heroes.h"
#include "userdir.h"
#include "userconf.h"
#include "musicfiles.h"
#include "bytesex.h"
#include "structs.h"
#include "hendian.h"
#include "rsc_files.h"
#include "rsc_files_hash.h"
#include "fader.h"
#include "const.h"
#include "scrtools.h"
#include "fontdata.h"
#include "bonus.h"
#include "sprtext.h"
#include "explosions.h"
#include "items.h"
#include "sprprogwav.h"
#include "gameid.h"
#include "persona.h"
#include "relocate.h"
#include "vars.h"
#include "camera.h"
#include "levellst.h"
#include "locales.h"
#include "main.h"
#include "pendulum.h"

char tile_set_name[128];
char glenz_name[128];

int *level_full_list;
size_t level_full_list_size = 0;
int rounds = 1;

unsigned int current_quest_level;

int level_is_finished;

/* AI won't be enabled on the first moves */
#define ia_skip_firsts_moves 2

char ia_max_depth;
char ia_cur_depth;
char ia_is_invincible;
char ia_player;
a_square_coord ia_target_x, ia_targer_y;
a_square_coord ia_wrap_x, ia_wrap_y;
char ia_wrap_left, ia_wrap_right;

#define DEMO_DURATION 90

char enable_blit;

char mouse_found = 1;
/***********************/

char txt_tmp[20];

a_timer blink_htimer;
a_timer clock_htimer;
a_timer tiles_anim_htimer;
a_timer corner_htimer;
a_timer event_htimer;
long event_time;		/* updated from event_htimer on each frame */
a_timer update_htimer;
a_timer waving_htimer;
a_timer background_htimer;
a_timer sound_track_htimer;
a_timer demo_trigger_htimer;

static unsigned char play_game (char);

static void
init_buffers (void)
{
  XMALLOC_ARRAY (render_buffer[0], xbuf * ybuf);
  XMALLOC_ARRAY (render_buffer[1], xbuf * ybuf);
}

static void
clean_buffers (void)
{
  memset (render_buffer[0], 0, xbuf * ybuf);
  memset (render_buffer[1], 0, xbuf * ybuf);
}

static void
close_buffers (void)
{
  free (render_buffer[0]);
  free (render_buffer[1]);
}

static void
add_color (char oui)
{
  int d;
  unsigned char b;
  do {
    d = rand () % lvl.square_count;
  }
  while (square_object[d] != -1);
  if (oui && (rand () % 40 == 0))
    b = 16;
  else
    b = rand () % 5;
  if ((rand () & 3) == 0)
    b |= 8;
  square_object[d] = b;
}

static void
add_cash (char oui)
{
  int d;
  unsigned char b = 0;
  do {
    d = rand () % lvl.square_count;
  }
  while (square_object[d] != -1);
  if (oui && (rand () % 40 == 0))
    b = 15;
  square_object[d] = b;
}

static void
erase_player (int i)
{
  dmsg (D_MISC, "erase player %d", i);

  if (!in_menu)
    square_occupied[player[i].x2 + player[i].y2 * lvl.square_width] = 0xff;
  if (player[i].way == w_left)
    square_occupied[player[i].y2 * lvl.square_width +
		   ((player[i].x2 - 1) & lvl.square_width_wrap)] = 0xff;
  if (player[i].way == w_right)
    square_occupied[player[i].y2 * lvl.square_width +
		   ((player[i].x2 + 1) & lvl.square_width_wrap)] = 0xff;
  if (player[i].way == w_up)
    square_occupied[player[i].x2 +
		   ((player[i].y2 - 1) & lvl.square_height_wrap)
		   * lvl.square_width] =
      0xff;
  if (player[i].way == w_down)
    square_occupied[player[i].x2 +
		   ((player[i].y2 + 1) & lvl.square_height_wrap)
		   * lvl.square_width] =
      0xff;
}


static void
reinit_player (unsigned p)
{
  int tries, m;

  unsigned start_pos;		/* 0..3: one of the 4 starting positions.  */
  a_square_corrd_pair start_coord;
  a_dir start_dir;
  a_square_index start_idx, next_idx;

  dmsg (D_MISC, "initialize player %d", p);

  tries = 4;
  start_pos = p - 1;
  do {
    --tries;
    start_pos = (start_pos + 1) & 3;
    lvl_start_position (&lvl, start_pos, &start_coord, &start_dir);
    start_idx = SQR_COORDS_TO_INDEX (&lvl, start_coord.y, start_coord.x);
    next_idx = lvl.square_move[start_dir][start_idx];
  } while (tries && (next_idx == INVALID_INDEX ||
		     square_occupied[next_idx] != 0xff));

  player[p].way = start_dir;
  player[p].x2 = start_coord.x;
  player[p].y2 = start_coord.y;
  player[p].pos = start_idx;

  /* ensure that the start position is usable,
     otherwise try another position (randomly) */

  for (;;) {
    /* Make sure the selected square is usable (nobody already present,
       and not out of the way).  */
    if (lvl.square_type[start_idx] != T_OUTWAY
	&& square_occupied[start_idx] == 0xff) {

      a_square_index si;
      bool dir_unusable[4] = { false, false, false, false };

      /* Check directions which are not usable. */

      if ((si = lvl.square_move[D_UP][start_idx]) == INVALID_INDEX
	  || square_occupied[si] != 0xff)
	dir_unusable[D_UP] = true;
      if ((si = lvl.square_move[D_RIGHT][start_idx]) == INVALID_INDEX
	  || square_occupied[si] != 0xff)
	dir_unusable[D_RIGHT] = true;
      if ((si = lvl.square_move[D_DOWN][start_idx]) == INVALID_INDEX
	  || square_occupied[si] != 0xff)
	dir_unusable[D_DOWN] = true;
      if ((si = lvl.square_move[D_LEFT][start_idx]) == INVALID_INDEX
	  || square_occupied[si] != 0xff)
	dir_unusable[D_LEFT] = true;

      /* is at least one direction ok? */
      if (!(dir_unusable[D_UP]
	    & dir_unusable[D_RIGHT]
	    & dir_unusable[D_DOWN]
	    & dir_unusable[D_LEFT])) {
	/* then rotate until we find that direction */
	while (dir_unusable[player[p].way])
	  player[p].way = (player[p].way + 1) & 3;
	break;
      }
    }
    /* else, get a new position randomly on the map */
    player[p].y2 = rand () % lvl.square_height;
    player[p].x2 = rand () % lvl.square_width;
    start_idx = SQR_COORDS_TO_INDEX (&lvl, player[p].y2, player[p].x2);
  }

  player[p].x = player[p].x2 >> 1;
  player[p].y = player[p].y2 >> 1;
/**************/

  player[p].ia_max_depth = (rand () & 1) + 5;
  if (game_mode >= M_KILLEM && game_mode != M_DEATHM)
    player[p].behaviour = 2;
  else
    player[p].behaviour = rand () & 1;

  if (two_players)
    player[p].target = col2plr[rand () & 1];
  else
    player[p].target = col2plr[0];

  player[p].target |= (ia_skip_firsts_moves * 16);

  player[p].d.e = 0;
  player[p].turbo = 1;
  player[p].turbo_level_delta = player[p].turbo_level = 1024;
  player[p].vitp = player[p].v = 4369 * 3 / 2 + (opt.speed * 2 * 1092);
  player[p].spec = 0;
  player[p].delay = 0;
  player[p].inversed_controls = 0;
  player[p].speedup = 0;
  player[p].rotozoom = 0;
  player[p].waves = 0;
  player[p].waves_begin = 0;
  player[p].fire_trail = 0;
  player[p].tunnel_inverse = 0;
  player[p].next_way = player[p].old_old_way = player[p].old_way =
    player[p].way;
  if (!in_menu)
    square_occupied[player[p].x2 + player[p].y2 * lvl.square_width] = p;
  if (player[p].way == D_LEFT)
    square_occupied[player[p].y2 * lvl.square_width +
		   SQR_COORD_LEFT (&lvl, player[p].x2)] = (char) (p + 4);
  if (player[p].way == D_RIGHT)
    square_occupied[player[p].y2 * lvl.square_width +
		   SQR_COORD_RIGHT (&lvl, player[p].x2)] = (char) (p + 4);
  if (player[p].way == D_UP)
    square_occupied[player[p].x2 +
		   SQR_COORD_UP (&lvl, player[p].y2) * lvl.square_width] =
      (char) (p + 4);
  if (player[p].way == D_DOWN)
    square_occupied[player[p].x2 +
		   SQR_COORD_DOWN (&lvl, player[p].y2) * lvl.square_width] =
      (char) (p + 4);

  start_dir = player[p].way;
  square_way[start_idx] = DIR8_PAIR (start_dir, start_dir);
  trail_offset[p] = 0;
  for (m = trail_size[p]; m >= 0; m--) {
    trail_pos[p][m] = start_idx;
    trail_way[p][m] = DIR8_PAIR (start_dir, start_dir);
  }
}

static void
find_lemming_direction (a_lemming *lem)
{
  a_dir d;
  a_dir_mask avail_dirm;

  /* The lemming advances one square.  The tail takes the place of
     the head.  */
  square_lemmings_list[lem->pos_tail] = NULL;
  lem->pos_tail = lem->pos_head;
  square_lemmings_list[lem->pos_tail] = lem;

  /* We need to compute the new square for the head (i.e. the
     direction of the lemming).  */

  /* Mask AVAIL_DIRM with all the directions impossible to use
     by the lemming.  */
  avail_dirm = DM_ALL;
  for (d = 0; d < DIR_MAX; ++d) {
    a_square_index dest = lvl.square_move[d][lem->pos_tail];

    if (/* Lemmings can't cross walls, */
	dest == INVALID_INDEX
	/* they should not go to a square occupied by a player, */
	|| square_occupied[dest] != 0xff
	/* neither should they go to a quare occupied by another
	   lemming.  */
	|| square_lemmings_list[dest])
      avail_dirm &= ~DIR_TO_DIRMASK(d);
  }
  /* Also, we don't want to allow lemmings to enter
     tunnels.  This constraint might be relaxed in the
     future, but actually this requires several things that
     I (adl) don't plan to work on at the moment:
      - handling two directions for each lemming: for the
        head and the tail, because the direction at the output
        of a tunnel might not be the same as the entrance
      - drawing the lemmings in two parts (since they need
        to be cropped under the tunnel entrance)
      - handling dead lemmings (blood pudles) on two
        non-adjacent squares (e.g. the entrance and the output
        of the tunnel, in case the lemming has been squished in
        a tunnel).
     This latter point is not only a rendering issue: the
     way dead lemmings are stored needs to be changed too
     (at the time this comment is written, they are assigned
     to *the* nearest square, and linked to the other dead
     lemmings of this square).

     So, for now, let's just prevent them from turning toward
     a tunnel entrance.  */
  if (lvl.square_type[lem->pos_tail] == T_TUNNEL)
    avail_dirm &= ~DIR_TO_DIRMASK (lvl.square_direction[lem->pos_tail]);

  /* Is the current direction available?  (We'd better avoid changing
     the direction on each square unless we decide lemmings are bees.)
     */
  if (avail_dirm & DIR_TO_DIRMASK(lem->dir)) {
    /* The current direction is free.  Go on.  */
    lem->pos_head = lvl.square_move[lem->dir][lem->pos_tail];
  } else {
    /* Current direction unavalaible.  Let's find another one.  */
    a_dir_mask i;
    int n = 0;

    /* Count the number of direction available.  */
    for (i = 1; i < DM_ALL; i <<= 1)
      if (avail_dirm & i)
	++n;

    if (n) {
      /* Chose one of the free directions. */
      n = 1 + rand () % n;
      for (d = 0; n != 0; ++d, avail_dirm >>= 1)
	if (avail_dirm & 1)
	  --n;
      lem->dir = d - 1;
      lem->pos_head = lvl.square_move[lem->dir][lem->pos_tail];
    } else {
      /* This place is reached when there is no free way for the
	 lemming to use.  We used to set lem->dir = 5; to indicate
	 this condition to the renderer (when then the lemmings cannot
	 move it should be not be rendered as `walking').  But it
	 turns out that checking whether ptr->pos_tail ==
	 lem->pos_head is sufficient to detect this condition.  */
    }
  }

  assert (lem->pos_head != INVALID_INDEX);
  if (lem->pos_head != lem->pos_tail) {
    /* We assert this only here, because it may happens that a
       lemmings is blocked under a player trail.  */
    assert (square_occupied[lem->pos_head] == 0xff);

    /* If the lemming is moving, mark the destination square as
       occupied so that no other lemming dares to move there too. */
    assert (square_lemmings_list[lem->pos_head] == 0);
    square_lemmings_list[lem->pos_head] = lem;
  }
}

static char
load_level (char *filename, char cont)
{
  unsigned int i, j, k, k2, l;
  a_lemming *ptir;
  int err;

  dmsg (D_FILE|D_LEVEL, "loading level: %s", filename);

  if (!in_menu) {
    if (two_players) {
      player[col2plr[0]].cpu = 2;
      player[col2plr[1]].cpu = 2;
      player[col2plr[2]].cpu = 0;
      player[col2plr[3]].cpu = 0;
    } else {
      player[col2plr[0]].cpu = 2;
      player[col2plr[1]].cpu = 0;
      player[col2plr[2]].cpu = 0;
      player[col2plr[3]].cpu = 0;
    }
  }
  if (in_demo) {
    player[col2plr[0]].cpu = 0;
    player[col2plr[1]].cpu = 0;
  }

  clean_buffers ();

  err = lvl_load_file (filename, &lvl, true);
  if (err) {
    dmsg (D_LEVEL|D_FILE, "cannot open %s", filename);
    dperror ("lvl_load_file");
    return err;
  }

  dmsg (D_LEVEL, "size=(%u,%u) wrap=(%x,%x) tile=%s soundtrack=%s",
	lvl.tile_width, lvl.tile_height,
	lvl.tile_width_wrap, lvl.tile_height_wrap,
	lvl_tile_sprite_map_basename (&lvl), lvl_sound_track (&lvl));

  {
    const char *bn = lvl_tile_sprite_map_basename (&lvl);
    int fd;
    char* tmp = get_non_null_rsc_file ("tiles-sets-dir");
    stpcpy (stpcpy (stpcpy (tile_set_name, tmp), bn), ".pcx");
    stpcpy (stpcpy (stpcpy (glenz_name, tmp), bn), ".glz");
    free (tmp);

    pcx_load (tile_set_name, &tile_set_img);

    dmsg (D_LEVEL|D_FILE, "loading %s", glenz_name);
    fd = open (glenz_name, O_RDONLY | O_BINARY);
    if (fd == -1) {
      dperror ("open");
      return errno;
    }

    if (read (fd, glenz, 256 * 8) != 256 * 8) {
      dperror ("read");
      return errno;
    }
    close (fd);
  }

  init_render_data ();

  if (in_menu) {
    load_soundtrack_from_alias ("MENU");
  } else {
    load_soundtrack_from_alias (lvl_sound_track (&lvl));
  }

  dmsg (D_LEVEL, "initialize variables and maps associated to the level");

  XSALLOC_ARRAY (square_occupied, lvl.square_count, 0xff);

  allocate_explosions ();

  XMALLOC_ARRAY (square_way, lvl.square_count);

  if (init_bonuses_level ())
    return 15;

  XMALLOC_ARRAY (square_tile, lvl.square_count);
  k = 0;
  for (i = 0, l = 0; i < lvl.tile_height; i++, l += lvl.square_width * 2) {
    for (j = 0, k2 = l; j < lvl.tile_width; j++, k2 += 2, k++) {
      square_tile[k2] = k;
      square_tile[k2 + 1] = k;
      square_tile[lvl.square_width + k2] = k;
      square_tile[lvl.square_width + k2 + 1] = k;
    }
  }

  XCALLOC_ARRAY (square_coord, lvl.square_count);

  if (game_mode == M_KILLEM) {
    XCALLOC_ARRAY (square_lemmings_list, lvl.square_count);
    XCALLOC_ARRAY (square_dead_lemmings_list, lvl.square_count);
    memset (lemmings_support, 0, lemmings_total * sizeof (a_lemming));
  }
  if (game_mode >= M_TCASH) {
    XMALLOC_ARRAY (square_object, lvl.square_count);
  }

  /* Init square_coord, map offsets to coordinates.   */
  for (j = 0; j < lvl.square_height; j++)
    for (i = 0; i < lvl.square_width; i++) {
      square_coord[SQR_COORDS_TO_INDEX (&lvl, j, i)].y = j;
      square_coord[SQR_COORDS_TO_INDEX (&lvl, j, i)].x = i;
    }
  /* init of players  */
  if (!in_menu)
    for (i = 0; i < 4; ++i) {
      /* trail_offset[i]=0; */
      if (game_mode == M_DEATHM) {
	trail_size[i] = 32;
	player[i].lifes = 9;
      } else
	trail_size[i] = 5;
      reinit_player ((char) i);
      if (cont == 0) {
	player[i].lifes = 9;
	player[i].score = 0;
	player[i].wins = 0;
      } else {
	/* reinitialize dead computers: give them an empty score
	   and decrase their total of wins */
	if (player[i].cpu < 2 && player[i].lifes == 0)
	  {
	    player[i].lifes = 9;
	    player[i].score = 0;
	    if (player[i].wins > 0)
	      --player[i].wins;
	  }
      }
      player[i].autopilot = 1;
      player[i].score_delta = player[i].score << 2;
      player[i].invincible = 0;
      player[i].time = 3000;
      player[i].cash = 0;
      player[i].martians_nbr = 0;
    }
  /* reinit player once again to avoid the case where
     some vehicles could have been put in front of others */
  erase_player (0);
  reinit_player (0);
  erase_player (1);
  reinit_player (1);
  erase_player (2);
  reinit_player (2);
  erase_player (3);
  reinit_player (3);

  if (!opt.autopilot_one)
    player[col2plr[0]].autopilot = 0;
  if (two_players && !opt.autopilot_two)
    player[col2plr[1]].autopilot = 0;

  /*** init of lemmings ***/
  if (game_mode == M_KILLEM) {
    ptir = lemmings_support;
    l = lvl.square_count;
    for (i = 0; i < 4; i++) {
      for (j = lemmings_per_players; j != 0; j--) {
	/* Drop the lemming randomly on an accessible unoccupied
	   square.  The lemmings might still not be able to move,
	   but it's unimportant: it should just stay still until
	   it can move.  */
	do {
	  k = rand () % l;
	  assert (k < lvl.square_count);
	} while (lvl.square_type[k] == T_OUTWAY
		 || square_occupied[k] != 0xff
		 || square_lemmings_list[k] != NULL);
	ptir->pos_head = k;
	find_lemming_direction (ptir);
	ptir->min = 0;
	ptir->next_dead = NULL;
	ptir->couleur = i;
	ptir->dead = 0;
	ptir++;
      }
      player[i].lemmings_nbr = lemmings_per_players;
    }
    assert (ptir == lemmings_support + 4 * lemmings_per_players);
    lemmings_move_offset = 0;
  }

  if (game_mode >= M_TCASH) {
    for (i = 0; i < lvl.square_count; ++i)
      if (lvl.square_type[i] == T_OUTWAY)
	square_object[i] = -2;	/* -2 = you can't drive here */
      else
	square_object[i] = -1;
    if (game_mode == M_COLOR) {
      objects_nbr = lvl.square_count / 14 + 1;
      for (i = objects_nbr; i != 0; i--)
	add_color (1);
    }
    if (game_mode == M_TCASH) {
      objects_nbr = lvl.square_count / 13 + 1;
      for (i = objects_nbr; i != 0; i--)
	add_cash (1);
    }
  }

  level_is_finished = 0;

  if (!in_menu)
    spread_bonuses ();
  return (0);
}

static void
unload_level (void)
{
  dmsg (D_LEVEL, "unloading level");

  uninit_render_data ();
  uninit_bonuses_level ();
  img_free (&tile_set_img);
  free (square_occupied);
  release_explosions ();
  free (square_way);
  free (square_tile);
  free (square_coord);
  if (game_mode == M_KILLEM && !in_menu) {
    free (square_lemmings_list);
    free (square_dead_lemmings_list);
  }
  if (game_mode >= M_TCASH && !in_menu)
    free (square_object);
  lvl_free (&lvl);
  unload_soundtrack ();
}

static void
load_level_from_number (int nbr, char cont)
{
  char e;

  dmsg (D_SECTION, "load level #%d", nbr);
  e = load_level (level_list[nbr].name, cont);
  if (e != 0)
    emsg (_("Cannot load level %s (error %d)"), level_list[nbr].name, e);
}

static void
compute_level_full_list (void)
{
  int i;
  unsigned int j;

  dmsg (D_SECTION, "compute level full list");

  level_full_list_size = level_list_size + extra_nbr;
  XMALLOC_ARRAY (level_full_list, level_full_list_size);

  if (extra_nbr == 0) {
    extrasel = 0;
    opt.extras = 0;
  }
  if (extrasel) {
    i = extra_nbr - 1;
    while ((i >= 0) && extra_selected_list[i] == 0)
      i--;
    if (i < 0)
      opt.extras = 0;
  }
  i = 0;
  if (opt.extras != 2)
    for (; i < (int) level_list_size; i++)
      level_full_list[i] = i;
  if (opt.extras != 0)
    for (j = 0; j < extra_nbr; j++)
      if (extra_selected_list[j] || (extrasel == 0))
	level_full_list[i++] = (j | 0x10000);
  level_full_list_size = i;
}

static void
free_level_full_list (void)
{
  free (level_full_list);
}

static int
random_level (void)
{
  unsigned int i, j, k;
  assert (level_full_list_size > 0);
  i = rand () % level_full_list_size;
  level_full_list_size--;
  k = level_full_list[i];
  for (j = i; j < level_full_list_size; j++)
    level_full_list[j] = level_full_list[j + 1];
  return (k);
}

static void
load_random_wrapped_level (bool wrapped, char cont)
{
  int t;

  dmsg (D_SECTION, "load random wrapped level");

  do {
    t = rand () % level_list_size;
  } while (wrapped && !level_list[t].wrapped);

  load_level_from_number (t, cont);
}

static void
load_random_level (char cont)
{
  int t;
  char *tmp;
  char e;
  t = random_level ();

  dmsg (D_SECTION, "load random level");

  if (t & 0x10000) {
    tmp = extra_list[t & 0xffff].full_name;
  } else {
    tmp = level_list[t].name;
  }
  e = load_level (tmp, cont);
  if (e != 0)
    emsg (_("Cannot load level %s (error %d)"), tmp, e);
}

static void
play_menu (void)
{
  char cont;
  FILE *ftmp;
  int gamemodeh;
  static int l = 1, u = 0;
  char flagload = 0;
  a_keycode t;
  int i;
  a_timer flip_timer;
  long flip_pos;

  if (l == 5)
    l = 1;

  for (i = 3; i >= 0; i--) {
    col2plr[i] = opt.player_color[i];
    plr2col[opt.player_color[i]] = i;
  }

  std_white_fadein (&tile_set_img.palette);
  do {
    do {
      draw_play_menu (l);
      flush_display (corner[0]);
      if (key_or_joy_ready ()) {
	t = get_key_or_joy ();
	if (t == HK_Up || HK_Down)
	  event_sfx (1);
	if (t == HK_Up) {
	  if (l > 0)
	    l--;
	  else
	    l = 7;
	}
	if (t == HK_Down) {
	  if (l < 7)
	    l++;
	  else
	    l = 0;
	}
	if (t == HK_Escape) {
	  if (l != 7) {
	    l = 7;
	    event_sfx (1);
	  } else
	    t = HK_Enter;
	}
      } else
	t = 0;
    } while (t != HK_Enter);
    if (l == 0) {
      event_sfx (5);
      two_players ^= 1;
    }
    if (l == 7) {
      event_sfx (8);
      return;
    }
    if (l == 6) {
      load_save_records ();
      std_white_fadein (&tile_set_img.palette);
      do {
	background_menu ();
	draw_saved_games_info (0, u, false);
	flush_display (corner[0]);
	if (key_or_joy_ready ()) {
	  t = get_key_or_joy ();
	  if (t == HK_Up) {
	    if (u > 0)
	      u--;
	    else
	      u = 9;
	  }
	  if (t == HK_Down) {
	    if (u < 9)
	      u++;
	    else
	      u = 0;
	  }
	} else
	  t = 0;
	if (t == HK_Enter && saverec[u].used == 0) {
	  event_sfx (18);
	  t = 0;
	}
      } while (t != HK_Enter && t != HK_Escape);
      if (t == HK_Escape) {
	event_sfx (8);
      }
      if (t == HK_Enter)
	flagload = 1;
    }
  } while (l == 0 || (l == 6 && flagload == 0));
  event_sfx (9);

  flip_timer = new_htimer (T_GLOBAL, HZ (280));
  do {
    if (flagload == 0)
      draw_play_menu (l);
    else {
      background_menu ();
      draw_saved_games_info (0, u, false);
    }
    flip_pos = -read_htimer (flip_timer);
    if (flip_pos < -256)
      flip_pos = -256;
    flip_buffer (flip_pos);
    flush_display (render_buffer[1]);
  } while (flip_pos > -256);
  flush_display (render_buffer[1]);
  /* erase the one-line flipped screen */
  memset (render_buffer[1] + 100 * xbuf, 0, 320);
  flush_display (render_buffer[1]);

  free_htimer (flip_timer);

  if (flagload == 0) {
    if (l == 1 )
      game_mode = M_QUEST;
    else if (l == 2)
      game_mode = M_DEATHM;
    else if (l == 3)
      game_mode = M_KILLEM;
    else if (l == 4)
      game_mode = M_TCASH;
    else if (l == 5)
      game_mode = M_COLOR;
    gamemodeh = game_mode;
    cont = 0;
    current_quest_level = 0;
    create_gameid (game_id);
  } else {
    gamemodeh = game_mode = M_QUEST;
    current_quest_level = saverec[u].level;
    copy_gameid (game_id, saverec[u].gid);
    for (t = 0; t < 4; t++) {
      player[col2plr[t]].lifes = saverec[u].lifes[t];
      player[col2plr[t]].score = saverec[u].points[t];
    }
    cont = 1;
  }

  unload_level ();		/* also stop the music */

  if (opt.sfx)
    load_sfx_mode (game_mode);
  if (game_mode > M_QUEST) {
    compute_level_full_list ();
    rounds = rounds_nbr_values[opt.gamerounds];
  } else
    rounds = 1;
  while (((current_quest_level < level_list_size)
	  || ((game_mode > M_QUEST)
	      && (current_quest_level < level_full_list_size)))
	 && (rounds > 0) && (play_game (cont) == 0)) {
    cont = 1;
    if (game_mode > M_QUEST) {
      rounds--;
      if (level_full_list_size == 0)
	rounds = 0;
    }
  }

  if (game_mode > M_QUEST)
    free_level_full_list ();

  /* END OF THE GAME */

  if ((game_mode == M_QUEST) && (current_quest_level >= level_list_size)) {
    if (level_is_finished != 15) {
      /* End scroller */
      char* tmp;
      char* tmp2;
      dmsg (D_SECTION, "-- end scroller --");
      tmp = get_non_null_rsc_file ("tiles-sets-dir");
      tmp2 = strcat_alloc (tmp, "level02.glz");
      free (tmp);
      ftmp = fopen (tmp2, "rb");
      free (tmp2);
      fread (glenz, 256, 8, ftmp);
      fclose (ftmp);

      load_soundtrack_from_alias ("ENDSCROLL");
      play_soundtrack ();
      end_scroll ();
      unload_soundtrack ();
    }
  }
/* ************ */

  dmsg (D_SECTION, "-- (back to) menu (from game) --");

  game_mode = M_QUEST;
/* free_all_sfx(); */
  load_random_wrapped_level (true, cont);
  load_sfx_mode (-1);

  play_soundtrack ();
  reset_htimer (sound_track_htimer);
  /* The logic is quite convolued here because we instert the score in
     a file which might be shared by several process.

     First we load the scores (to get up-to-date informations) and
     then if the current score deserve an insertion we ask for the
     player's name, and finally lock the file for insertion.

     Note that if the scores file is updated while the player enter
     his names, the current scores might not deserve an insertion in
     the file anymore.  The only consequence is that the player has
     entered his name for nothing.  */
  load_scores ();
  for (t = 0; t < 4; t++)
    if (player[t].cpu == 2) {
      if (insert_scores (gamemodeh, 0, game_id, player[t].score)) {
	char player_name[PLAYER_NAME_SIZE + 1];
	enter_your_name (plr2col[t] + 1, player_name);
	load_scores_and_keep_locked ();
	insert_scores (gamemodeh, player_name, game_id, player[t].score);
	write_scores_locked ();
      }
    }
}

/* These variables are used to handle the moving radar and score at
   the begging of a level.  */
int radar_target_pos;
int radar_current_pos;

static void
output_screen (char n)
{
  a_pixel *src;
  int i;
  int loginf[4];		/* values for counters */

  position_camera ();

  if (game_mode == M_DEATHM)
    for (i = 0; i < 4; i++)
      loginf[i] = player[col2plr[i]].lifes;
  else if (game_mode == M_KILLEM)
    for (i = 0; i < 4; i++)
      loginf[i] = player[col2plr[i]].lemmings_nbr;
  else if (game_mode >= M_TCASH)
    for (i = 0; i < 4; i++)
      loginf[i] = player[col2plr[i]].cash;

  if (two_players == false) {
    compute_corner (0, n);
    draw_level (0);
    if (player[col2plr[0]].waves) {
      wave_buffer ();
      corner[0] = render_buffer[0];
    }
    if (player[col2plr[0]].rotozoom) {
      rotozoom_buffer ();
      corner[0] = render_buffer[1] + xbuf;
    }
    if (opt.radar_map)
      draw_radar_map (player[col2plr[0]].x2, player[col2plr[0]].y2,
		      radar_current_pos);
    if (opt.display_infos)
      draw_score (col2plr[0], 0, 5, 5, -radar_current_pos);
    if (game_mode != M_QUEST) {
      if (radar_current_pos <= 70)
	draw_logo_info (col2plr[0], loginf[0],
			corner[0] + (183 + ((radar_current_pos >= 30)
					    ? ((radar_current_pos - 30) >> 1)
					    : 0)) * xbuf + 24);
      if (radar_current_pos <= 60)
	draw_logo_info (col2plr[1], loginf[1],
			corner[0] + (183 + ((radar_current_pos >= 20)
					    ? ((radar_current_pos - 20) >> 1)
					    : 0)) * xbuf + 98);
      if (radar_current_pos <= 50)
	draw_logo_info (col2plr[2], loginf[2],
			corner[0] + (183 + ((radar_current_pos >= 10)
					    ? ((radar_current_pos - 10) >> 1)
					    : 0)) * xbuf + 172);
      if (radar_current_pos <= 40)
	draw_logo_info (col2plr[3], loginf[3],
			corner[0] + (183 + (radar_current_pos >> 1)) * xbuf +
			246);
    }
    if (player[col2plr[0]].spec != 0xde)
      show_txt_bonus (col2plr[0], corner[0] + 40 + 5 * xbuf);
  } else {
    compute_corner (0, n);
    compute_corner (1, n);
    draw_level (0);
    draw_level (1);
    if (level_is_finished == 0) {
      if (player[col2plr[0]].waves) {
	wave_half_buffer (0);
	corner[0] = render_buffer[0];
      }
      if (player[col2plr[0]].rotozoom) {
	rotozoom_half_buffer (0);
	corner[0] = render_buffer[0] + xbuf - 180 + xbuf;
      }
      if (player[col2plr[1]].waves) {
	wave_half_buffer (1);
	corner[1] = render_buffer[1];
      }
      if (player[col2plr[1]].rotozoom) {
	rotozoom_half_buffer (1);
	corner[1] = render_buffer[1] + xbuf - 180 + xbuf;
      }
    }
    if (game_mode != M_QUEST) {
      if (radar_current_pos <= 60)
	draw_logo_info (col2plr[0], loginf[0],
			corner[swapside] + (5 - ((radar_current_pos >= 30)
						 ? ((radar_current_pos - 30)
						    >> 1) : 0)) * xbuf + 44);
      if (radar_current_pos <= 50)
	draw_logo_info (col2plr[1], loginf[1],
			corner[swapside] + (5 - ((radar_current_pos >= 20)
						 ? ((radar_current_pos - 20)
						    >> 1) : 0)) * xbuf + 104);
      if (radar_current_pos <= 40)
	draw_logo_info (col2plr[2], loginf[2],
			corner[1 - swapside] + (5 - ((radar_current_pos >= 10)
						     ? (
							(radar_current_pos -
							 10) >> 1) : 0)) *
			xbuf + 6);
      if (radar_current_pos <= 30)
	draw_logo_info (col2plr[3], loginf[3],
			corner[1 - swapside] + (5 -
						(radar_current_pos >> 1)) *
			xbuf + 66);
    }
    if (player[col2plr[0]].spec != 0xde)
      show_txt_bonus (col2plr[0], corner[0] + 1 + 185 * xbuf);
    if (player[col2plr[1]].spec != 0xde)
      show_txt_bonus (col2plr[1], corner[1] + 2 + 185 * xbuf);

    src = corner[swapside] + 158;
    for (i = 200; i != 0; i--) {
      *src = glenz[0][(int) *src];
      ++src;
      *src = glenz[0][(int) glenz[0][(int) *src]];
      src += xbuf - 1;
    }

    src = corner[1 - swapside];
    for (i = 200; i != 0; i--) {
      *src = glenz[0][(int) glenz[0][(int) *src]];
      ++src;
      *src = glenz[0][(int) *src];
      src += xbuf - 1;
    }

    if (opt.display_infos) {
      if (swapside) {
	draw_score (col2plr[0], 0, 5, 122, radar_current_pos);
	draw_score (col2plr[1], 1, 5, 5, -radar_current_pos);
      } else {
	draw_score (col2plr[0], 0, 5, 5, -radar_current_pos);
	draw_score (col2plr[1], 1, 5, 122, radar_current_pos);
      }
    }
  }
}

void
grow_trail (int pl, int size)
{
  int i, k;

  k = ((trail_offset[pl] + trail_size[pl] - 1) & (maxq - 1));
  while (size != 0 && trail_size[pl] + 5 < maxq) {
    i = ((trail_offset[pl] + trail_size[pl]) & (maxq - 1));
    trail_pos[pl][i] = trail_pos[pl][k];
    trail_way[pl][i] = trail_way[pl][k];
    trail_size[pl]++;
    --size;
  }

  if (trail_size[pl] >= 55 && game_mode == M_QUEST) {
    if (player[pl].cpu == 2)
      event_sfx (89);
    add_end_level_bonuses ();
  }
}

/* Shrink the trail for player PL by SIZE squares.  */
void
shrink_trail (int pl, int size)
{
  int i;

  while (size != 0 && trail_size[pl] > 5) {
    --size;
    --trail_size[pl];
    i = ((trail_offset[pl] + trail_size[pl]) & (maxq - 1));
    square_occupied[trail_pos[pl][i]] = 0xff;
    i = ((trail_offset[pl] + trail_size[pl] - 1) & (maxq - 1));
    /* Setup the new trail tail, but make sure we don't redraw
       anything if the trail has been erased before (this happens when
       the player dies: the square is erased (it explodes) and then
       shrinked).  */
    if (square_occupied[trail_pos[pl][i]] != 0xff)
      square_occupied[trail_pos[pl][i]] = (unsigned char) (pl + 12);
  }
}

static void
erase_trail (int c)
{
  a_square_index i;

  for (i = 0; i < lvl.square_count; ++i)
    if ((square_occupied[i] & 3) == c && square_occupied[i] < 16) {
      square_occupied[i] = 0xff;
      trigger_explosion (i, EXPLOSION_IMMEDIATE);
    }
}

/****************/

/* Adjust speed of AI-player C.  */
static void
ai_throttle (int c)
{
  /* DEAD_END[C] is TRUE if player C stopped (or tried to) because
     some opponent prevents it to move.  */
  static bool dead_end[4] = {false, false, false, false};

  unsigned free_directions = 4;
  bool seen_opponent = false;
  bool seen_opponent_head = false;
  /* Check squares neighboring next position.  */
  a_square_index next_pos = lvl.square_move[player[c].way][player[c].pos];
  a_dir i;
  for (i = 0; i < 4; i++) {
    a_square_index idx = lvl.square_move[i][next_pos];
    if (idx == INVALID_INDEX || i == REVERSE_DIR (player[c].way)) {
      --free_directions;
    } else {
      a_u8 o = square_occupied[idx];
      if (o != 0xff) {
	if ((o & 3) != c) {
	  seen_opponent = true;
	  if (o < 8)
	    seen_opponent_head = true;
	}
	--free_directions;
      } else if (square_explo_state[idx] < EXPLOSION_IMMEDIATE + 2
		 && ! player[c].invincible) {
	--free_directions;
      }
    }
  }

  if (! seen_opponent || free_directions > 1) {
    /* If no opponent is wandering around, or there is at least
       two free direction, there is no need to worry.  */
    player[c].turbo = 1;
    dead_end[c] = false;
    return;
  }

  if (free_directions) {
    /* There are some opponent in the vicinity, but there is at least
       one free direction.

       Speed up if we are near an opponent head (maybe he is trying to
       overtake us).  Don't accelerate otherwise, because we would not
       have the time to stop if he cuts us up.  */
    if (seen_opponent_head)
      player[c].turbo = 2;
    else
      player[c].turbo = 1;
    dead_end[c] = false;
    return;
  }

  if (! dead_end[c]) {
    /* This is a cul de sac and we haven't yet stopped.
       Better do it now.  */
    int chance = 3;
    assert (seen_opponent && free_directions == 0);
    if (player[c].rotozoom)
      --chance;
    if (player[c].waves)
      --chance;
    if (rand() % 4 <= chance)
      player[c].turbo = 0;
    /* Remember we already tried to stop, so we don't give
       another chance to this AI vehicle if it failed to stop.  */
    dead_end[c] = true;
  }
}

static unsigned int
ia_eval_dist (int pos)
{
  a_square_coord curx, cury, distx, disty;
  curx = square_coord[pos].x;
  cury = square_coord[pos].y;
  if (ia_wrap_left) {
    if (curx <= ia_wrap_x)
      distx = curx + lvl.square_width - ia_target_x;
    else if (curx <= ia_target_x)
      distx = ia_target_x - curx;
    else
      distx = curx - ia_target_x;
  } else {
    if (curx >= ia_wrap_x)
      distx = ia_target_x + lvl.square_width - curx;
    else if (curx <= ia_target_x)
      distx = ia_target_x - curx;
    else
      distx = curx - ia_target_x;
  }
  if (ia_wrap_right) {
    if (cury <= ia_wrap_y)
      disty = cury + lvl.square_height - ia_targer_y;
    else if (cury <= ia_targer_y)
      disty = ia_targer_y - cury;
    else
      disty = cury - ia_targer_y;
  } else {
    if (cury >= ia_wrap_y)
      disty = ia_targer_y + lvl.square_height - cury;
    else if (cury <= ia_targer_y)
      disty = ia_targer_y - cury;
    else
      disty = cury - ia_targer_y;
  }
  return (distx + disty);
}

/* used by ia_goto_target
 */

#define ia_eval_dir_target_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
	    tmp = ia_eval_dir_target (idx);				\
	    if (tmp < mindist) mindist = tmp;				\
    }

#define ia_eval_dir_bonus_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
            tmp = ia_eval_dir_bonus (idx);				\
	    if (tmp > mindist) mindist = tmp;				\
    }

#define ia_eval_dir_lemming_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
	    tmp = ia_eval_dir_lemming (idx);				\
	    if (tmp > mindist) mindist = tmp;				\
    }

#define ia_eval_dir_cash_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
	    tmp = ia_eval_dir_cash (idx);				\
	    if (tmp > mindist) mindist = tmp;				\
    }

#define ia_eval_dir_color_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
	    tmp = ia_eval_dir_color(idx);				\
	    if (tmp > mindist) mindist = tmp;				\
    }

/*
-> maluses in the position evaluation function
neighb wall ..... : 2pts,
neighb enemy .... : 5pts,
self (old) ...... : 2pts,
self (new) (128). : 1pts,
4pts count as one square in the distance function
*/

static int
ia_eval_neighb_pos (a_dir dir, a_square_index pos)
{
  a_square_index idx;
  unsigned char c;
  idx = lvl.square_move[dir][pos];
  if (idx != INVALID_INDEX) {
    c = square_occupied[idx];
    if (c < 128) {
      if ((c & 3) == ia_player)
	return 2;
      else
	return 5;
    } else if (c == 128)
      return 1;
    else
      return 0;
  }
  return 2;
}

static unsigned int
ia_eval_dir_target (a_square_index pos)
{
  a_u32 mindist;
  a_square_index idx;
  unsigned int tmp;

  ia_cur_depth--;
  if (ia_cur_depth != 0) {
    square_occupied[pos] = 128;
    mindist = U32_MAX;

    ia_eval_dir_target_inline (D_UP);
    ia_eval_dir_target_inline (D_RIGHT);
    ia_eval_dir_target_inline (D_DOWN);
    ia_eval_dir_target_inline (D_LEFT);

    square_occupied[pos] = 0xff;
    ia_cur_depth++;
    return mindist;
  } else {
    ia_cur_depth++;
    tmp = ia_eval_neighb_pos (D_UP, pos) + ia_eval_neighb_pos (D_RIGHT, pos)
      + ia_eval_neighb_pos (D_DOWN, pos) + ia_eval_neighb_pos (D_LEFT, pos);
    return ((ia_eval_dist (pos) << 16) + (tmp << 14) + ia_max_depth -
	    ia_cur_depth);
  }

}

static int
ia_eval_dir_lemming (a_square_index pos)
{
  int mindist;
  a_square_index idx;
  int tmp, tmp2;
  a_lemming *tmppti;

  ia_cur_depth--;
  if (ia_cur_depth != 0) {
    square_occupied[pos] = 128;
    mindist = 0;
    tmppti = square_lemmings_list[pos];
    if (tmppti >= lemmings_support
	&& tmppti < (lemmings_support + lemmings_total)) {
      if (tmppti->couleur == ia_player)
	tmp2 = -100;
      else
	tmp2 = 20;
    } else
      tmp2 = 0;

    ia_eval_dir_lemming_inline (D_UP);
    ia_eval_dir_lemming_inline (D_RIGHT);
    ia_eval_dir_lemming_inline (D_DOWN);
    ia_eval_dir_lemming_inline (D_LEFT);

    mindist += tmp2;		/* *(5+ia_cur_depth); */

    square_occupied[pos] = 0xff;
    ia_cur_depth++;
    return mindist;
  } else {
    ia_cur_depth++;
    tmp = ia_eval_neighb_pos (D_UP, pos) + ia_eval_neighb_pos (D_RIGHT, pos)
      + ia_eval_neighb_pos (D_DOWN, pos) + ia_eval_neighb_pos (D_LEFT, pos);
    return -(tmp << 2);
  }

}

static int
ia_eval_dir_color (a_square_index pos)
{
  signed int mindist;
  a_square_index idx;
  int d, tmp, tmp2;

  ia_cur_depth--;
  if (ia_cur_depth != 0) {
    square_occupied[pos] = 128;
    mindist = 0;
    d = (signed int) square_object[pos];
    tmp2 = 0;
    if (d >= 0) {
      if (d <= 4) {
	if (d == ia_player)
	  tmp2 = 100;
	else
	  tmp2 = -200;
      } else if (d <= 12) {
	if (d == ia_player + 8)
	  tmp2 = -200;
	else
	  tmp2 = 100;
      } else if (d == 16)
	tmp2 = 100;
      else if (d == 24)
	tmp2 = -40;
    }

    ia_eval_dir_color_inline (D_UP);
    ia_eval_dir_color_inline (D_RIGHT);
    ia_eval_dir_color_inline (D_DOWN);
    ia_eval_dir_color_inline (D_LEFT);

    mindist += tmp2;		/* *(5+ia_cur_depth); */

    square_occupied[pos] = 0xff;
    ia_cur_depth++;
    return mindist;
  } else {
    ia_cur_depth++;
    tmp = ia_eval_neighb_pos (D_UP, pos) + ia_eval_neighb_pos (D_RIGHT, pos)
      + ia_eval_neighb_pos (D_DOWN, pos) + ia_eval_neighb_pos (D_LEFT, pos);
    return -(tmp << 2);
  }
}

static int
ia_eval_dir_cash (a_square_index pos)
{
  signed int mindist;
  a_square_index idx;
  int d, tmp, tmp2;

  ia_cur_depth--;
  if (ia_cur_depth != 0) {
    square_occupied[pos] = 128;
    mindist = 0;
    d = (signed int) square_object[pos];

    if (d >= 0)
      tmp2 = 500;
    else
      tmp2 = -20;

    ia_eval_dir_cash_inline (D_UP);
    ia_eval_dir_cash_inline (D_RIGHT);
    ia_eval_dir_cash_inline (D_DOWN);
    ia_eval_dir_cash_inline (D_LEFT);

    mindist += tmp2;		/* *(5+ia_cur_depth); */

    square_occupied[pos] = 0xff;
    ia_cur_depth++;
    return mindist;
  } else {
    ia_cur_depth++;
    tmp = ia_eval_neighb_pos (D_UP, pos) + ia_eval_neighb_pos (D_RIGHT, pos)
      + ia_eval_neighb_pos (D_DOWN, pos) + ia_eval_neighb_pos (D_LEFT, pos);
    return -(tmp << 2);
  }

}

static int
ia_eval_dir_bonus (a_square_index pos)
{
  ia_cur_depth--;
  if (ia_cur_depth != 0) {
    int tmp2 = 0;
    int mindist = 0;
    int d = square_tile[pos];
    a_square_index idx;
    int tmp;
    square_occupied[pos] = 128;
    if (tile_bonus_cpu[d] == 0) {
      tmp = tile_bonus[d];
      if ((tmp != 0) && (tmp != 0xff)) {
	tile_bonus_cpu[d] = 1;
	if (tmp < 128)
	  tmp2 = bonus_points[0][tmp - 1];
	else
	  tmp2 = bonus_points[1][tmp - 129];
      }
    }
    ia_eval_dir_bonus_inline (D_UP);
    ia_eval_dir_bonus_inline (D_RIGHT);
    ia_eval_dir_bonus_inline (D_DOWN);
    ia_eval_dir_bonus_inline (D_LEFT);

    mindist += tmp2 * (5 + ia_cur_depth) /* /ia_max_depth */ ;

    if (tmp2)
      tile_bonus_cpu[square_tile[pos]] = 0;
    square_occupied[pos] = 0xff;
    ia_cur_depth++;
    return mindist;
  } else {
    int tmp;
    ia_cur_depth++;
    tmp = ia_eval_neighb_pos (D_UP, pos) + ia_eval_neighb_pos (D_RIGHT, pos)
      + ia_eval_neighb_pos (D_DOWN, pos) + ia_eval_neighb_pos (D_LEFT, pos);
    return -(tmp << 3);
  }

}

/* give the *way* to follow to get a given position */

#define ia_goto_target_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
	  ia_cur_depth=ia_max_depth;					\
	  tmp[dir] = ia_eval_dir_target (idx);				\
	  if (tmp[dir] < mindist) {					\
		mindist = tmp[dir];					\
		mindir = dir;						\
	  }								\
    }

#define ia_goto_bonus_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
	  ia_cur_depth = ia_max_depth;					\
	  tmp[dir] = ia_eval_dir_bonus(idx);				\
	  if (tmp[dir] > mindist) {					\
		mindist = tmp[dir];					\
		mindir = dir;						\
	  }								\
    }

#define ia_goto_lemming_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
	  ia_cur_depth = ia_max_depth;					\
	  tmp[dir] = ia_eval_dir_lemming(idx);				\
	  if (tmp[dir] > mindist) {					\
		mindist = tmp[dir];					\
		mindir = dir;						\
	  }								\
    }

#define ia_goto_color_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
	  ia_cur_depth = ia_max_depth;					\
	  tmp[dir] = ia_eval_dir_color (idx);				\
	  if (tmp[dir] > mindist) {					\
		mindist = tmp[dir];					\
		mindir = dir;						\
	  }								\
    }

#define ia_goto_cash_inline(dir)					\
    idx = lvl.square_move[dir][pos];					\
    if (idx != INVALID_INDEX)						\
    if ((square_occupied[idx] == 0xff) &&				\
       ((square_explo_state[idx] >= EXPLOSION_IMMEDIATE + 2)		\
        || ia_is_invincible)) {						\
	  ia_cur_depth = ia_max_depth;					\
	  tmp[dir] = ia_eval_dir_cash(idx);				\
	  if (tmp[dir] > mindist) {					\
		mindist = tmp[dir];					\
		mindir = dir;						\
	  }								\
    }

static char
ia_goto_target (int c, int targetx_, int targety_)
{
  a_square_index idx, pos;
  a_u32 tmp[4] = { U32_MAX, U32_MAX, U32_MAX, U32_MAX };
  a_u32 mindist = U32_MAX;
  a_dir mindir = 0;

  ia_player = c;
  ia_max_depth = player[c].ia_max_depth;
  ia_target_x = targetx_;
  ia_targer_y = targety_;
  ia_wrap_x = ia_target_x + lvl.tile_width;
  if (ia_wrap_x >= lvl.square_width) {
    ia_wrap_x -= lvl.square_width;
    ia_wrap_left = 1;
  } else
    ia_wrap_left = 0;
  ia_wrap_y = ia_targer_y + lvl.tile_height;
  if (ia_wrap_y >= lvl.square_height) {
    ia_wrap_y -= lvl.square_height;
    ia_wrap_right = 1;
  } else
    ia_wrap_right = 0;

  ia_is_invincible = (player[c].invincible != 0);
  pos = player[c].pos;
  ia_goto_target_inline (D_UP);
  ia_goto_target_inline (D_RIGHT);
  ia_goto_target_inline (D_DOWN);
  ia_goto_target_inline (D_LEFT);
  if (tmp[player[c].way] == tmp[mindir])
    return player[c].way;
  else
    return mindir;
}

static char
ia_goto_nearest_bonus (int c)
{
  a_square_index idx, pos;
  int tmp[4] = { 0, 0, 0, 0 };
  int mindist = 0;
  a_dir mindir = 0;

  ia_player = c;
  ia_max_depth = player[c].ia_max_depth;
  ia_is_invincible = (player[c].invincible != 0);
  pos = player[c].pos;
  ia_goto_bonus_inline (D_UP);
  ia_goto_bonus_inline (D_RIGHT);
  ia_goto_bonus_inline (D_DOWN);
  ia_goto_bonus_inline (D_LEFT);
  if (tmp[player[c].way] == tmp[mindir])
    return player[c].way;
  else
    return mindir;
}

static char
ia_goto_nearest_lemming (int c)
{
  a_square_index idx, pos;
  int tmp[4] = { 0, 0, 0, 0 };
  int mindist = 0;
  a_dir mindir = 0;

  ia_player = c;
  ia_max_depth = player[c].ia_max_depth;
  ia_is_invincible = (player[c].invincible != 0);
  pos = player[c].pos;
  ia_goto_lemming_inline (D_UP);
  ia_goto_lemming_inline (D_RIGHT);
  ia_goto_lemming_inline (D_DOWN);
  ia_goto_lemming_inline (D_LEFT);
  if (tmp[player[c].way] == tmp[mindir])
    return player[c].way;
  else
    return mindir;
}

static char
ia_goto_nearest_color (int c)
{
  a_square_index idx, pos;
  int tmp[4] = { 0, 0, 0, 0 };
  int mindist = 0;
  a_dir mindir = 0;

  ia_player = c;
  ia_max_depth = player[c].ia_max_depth;
  ia_is_invincible = (player[c].invincible != 0);
  pos = player[c].pos;
  ia_goto_color_inline (D_UP);
  ia_goto_color_inline (D_RIGHT);
  ia_goto_color_inline (D_DOWN);
  ia_goto_color_inline (D_LEFT);
  if (tmp[player[c].way] == tmp[mindir])
    return player[c].way;
  else
    return mindir;
}

static char
ia_goto_nearest_cash (int c)
{
  a_square_index idx, pos;
  int tmp[4] = { 0, 0, 0, 0 };
  int mindist = 0;
  a_dir mindir = 0;

  ia_player = c;
  ia_max_depth = player[c].ia_max_depth;
  ia_is_invincible = (player[c].invincible != 0);
  pos = player[c].pos;
  ia_goto_cash_inline (D_UP);
  ia_goto_cash_inline (D_RIGHT);
  ia_goto_cash_inline (D_DOWN);
  ia_goto_cash_inline (D_LEFT);
  if (tmp[player[c].way] == tmp[mindir])
    return player[c].way;
  else
    return mindir;
}

/****************/

/* search for a free direction */
static void
find_free_way (int c)
{
  int d = 0, n = 0, o[4] = { 0xff, 0xff, 0xff, 0xff }, e;
  int i, m;
  a_dir f;

  m = player[c].x2 + player[c].y2 * lvl.square_width;
  e = 1;
  for (i = 0; i < 4; i++) {
    a_square_index idx = lvl.square_move[i][m];
    if (idx != INVALID_INDEX)
      o[i] = square_occupied[idx];

    /* Forbid turn back.  This is usually not needed because the
       square behind the vehicle is already occupied, but in some
       tunnel configurations this may not be the case. */
    o[REVERSE_DIR (player[c].way)] = c;

    if (o[i] != 0xff || idx == INVALID_INDEX)
      d |= e;
    e += e;
  }

  f = player[c].next_way;

  /* Since the auto pilot is deactivated in case a player runs into a
     fire trail, it's possible for a human player to do a one-eighty
     turn and crash into his own tail. That can be quite annoying in
     case you want to go back one square left or right of your current
     lane and you're too fast pressing the buttons.

     Explicitly ignore the new direction in this case.  */
  if (f == REVERSE_DIR (player[c].way))
    player[c].next_way = player[c].way;

  /* If the way is free the autopilot has nothing to do.  */
  if (!(d & (1 << f)))
    return;

  /* If we reach this place, NEXT_WAY cannot be taken because there is
     a wall or someone else.  Therefore we will want to find some
     other direction automatically.  */

  if (player[c].cpu & 2) {
    /* The autopilot for human players does not work against fire trails,
       that would be too easy :).
       If NEXT_WAY would lead to a fired square, return immediately,
       unless the player is invincible, in which case the autopilot still
       apply.  */
    a_square_index idx = lvl.square_move[f][m];
    if (idx != INVALID_INDEX
	&& square_explo_state[idx] <= EXPLOSION_IMMEDIATE
	&& !player[c].invincible)
      return;
  }

  e = o[player[c].next_way];

  /* when a trail force someone to turn, the owner of this trail is credited */
  if ((e & 3) != c && (!level_is_finished) && e != 0xff
      && (player[e & 3].spec != 0xde))
    player[e & 3].score += 5;

  if (!(d & (1 << player[c].old_old_way))) {
    player[c].next_way = player[c].old_old_way;
    return;
  }

  if (!(d & (1 << player[c].old_way))) {
    player[c].next_way = player[c].old_way;
    return;
  }

  if (player[c].spec != t_ice)
    for (i = 1; i != 16; i += i)
      if (!(d & i))
	n++;

  e = d;
  if (n != 0) {
    n = (char) (1 + rand () % n);
    for (i = 0; n != 0; i++, d >>= 1)
      if (!(d & 1))
	n--;
    player[c].next_way = (char) (i - 1);
/*  if (w2d[i-1]&e) fatal_error("find_free_way() return nonsense !"); */
    assert (((1 << (i - 1)) & e) == 0);
  } else
    player[c].spec = 0xff;
}

/* * * * * * * * * * * * * * * * * * * * *\
 * Handling of players moves, collisions, *
 * bonus effects, lemmings dies, etc.     *
 \* * * * * * * * * * * * * * * * * * * **/

/* FIXME: there are many things here that should be moved in
   a separate function (e.g. `update_game') in order to be run only
   once per update, and not four times. */
static void
update_player (int c)
{
  a_square_index idx;
  a_tile_index d;
  int l, i;
  a_square_index d2;
  int t;
  a_lemming *tmppti;

  if ((player[c].score_delta >> 2) < player[c].score) {
    player[c].score_delta++;
    /* 1 life every 10.000 points */
    if (player[c].score_delta % (10000 << 2) == 0)
      apply_bonus (c, 15);
  }
/* if ((player[c].score_delta>>2)>player[c].score) player[c].score_delta--; */
  if (player[c].turbo_level_delta < player[c].turbo_level) {
    player[c].turbo_level_delta += 8;
    if (player[c].turbo_level_delta > player[c].turbo_level)
      player[c].turbo_level_delta = player[c].turbo_level;
  } else if (player[c].turbo_level_delta > player[c].turbo_level) {
    player[c].turbo_level_delta -= 8;
    if (player[c].turbo_level_delta < player[c].turbo_level)
      player[c].turbo_level_delta = player[c].turbo_level;
  }

  if (player[c].invincible > 0)
    player[c].invincible--;
  if (game_mode >= M_TCASH) {
    if (player[c].time > 0) {
      player[c].time--;
/* stop the game if the player is alone */
/*       if ((!level_is_finished) && */
/*           (player[(c+1)&3].spec==0xde) && */
/*           (player[(c+2)&3].spec==0xde) && */
/*           (player[(c+3)&3].spec==0xde)) { level_is_finished=c+1; return; } */
    } else if (!level_is_finished) {
      player[c].spec = 0xde;
      erase_trail (c);
      /* stop the game if all human players are dead or
	 if there is no more colors or dollars */
      if ((!(((player[0].cpu & 2) && (player[0].time))
	     || ((player[1].cpu & 2) && (player[1].time))
	     || ((player[2].cpu & 2) && (player[2].time))
	     || ((player[3].cpu & 2) && (player[3].time))))
	  || (objects_nbr == 0)) {
	/* KLUGE: mark all players whose time is 0 as dead.  This is
	   needed because many players can reach 0 simultaneously but
	   this block is only run for the first player when this is
	   discovered.  */
	for (i = 0; i < 4; ++i)
	  if (player[i].time == 0) {
	    player[i].spec = 0xde;
	    erase_trail (i);
	  }
	/* find out the richest player and set level_is_finished accordingly */
	level_is_finished = 0;
	for (i = 1; i < 4; i++)
	  if (player[i].cash > player[level_is_finished].cash)
	    level_is_finished = i;
	level_is_finished++;
      }
    }
  }
  update_player_bonus_vars (c);
  d = (player[c].x2 >> 1) + (player[c].y2 >> 1) * lvl.tile_width;
  if (player[c].rotozoom != 0)
    player[c].rotozoom--;
  if (player[c].waves != 0) {
    player[c].waves--;
    if (player[c].waves > 128 && player[c].waves_begin < 128)
      player[c].waves_begin++;
    if (player[c].waves < 128 && player[c].waves_begin > 0)
      player[c].waves_begin--;
  }
  if (player[c].fire_trail)
    --player[c].fire_trail;

  if (player[c].spec == 0xde)
    return;

  if (player[c].inversed_controls > 0) {
    player[c].inversed_controls--;
    sprintf (txt_tmp, _("INVERTED %d"), player[c].inversed_controls / 20 + 1);
    set_txt_bonus (c, txt_tmp, 2);
  }

  if (player[c].delay == 0) {
    /* Adjust speed of AI-controled vehicles.  */
    if (cpuon && ((player[c].cpu & 2) == 0))
      ai_throttle (c);
    if (player[c].turbo != 1 && player[c].turbo_level > 0
	&& player[c].speedup == 0) {
      player[c].vitt = (player[c].v + player[c].vi) * player[c].turbo;
      player[c].turbo_level -= 2;
    } else if (player[c].speedup > 0) {
      player[c].vitt = (player[c].v + player[c].vi) << 1;
      player[c].speedup--;
    } else if (player[c].speedup < 0) {
      player[c].vitt = (player[c].v + player[c].vi) >> 1;
      player[c].speedup++;
    } else
      player[c].vitt = (player[c].v + player[c].vi);
    if (player[c].vitp < player[c].vitt)
      if (player[c].vitp + 512 < player[c].vitt)
	player[c].vitp += 512;
      else
	player[c].vitp = player[c].vitt;
    else if (player[c].vitp > player[c].vitt) {
      if (player[c].vitp + 512 > player[c].vitt)
	player[c].vitp -= 512;
      else
	player[c].vitp = player[c].vitt;
    }
    player[c].d.e += player[c].vitp;
  } else {
    player[c].delay--;
    sprintf (txt_tmp, _("STOPPED %d"), player[c].delay / 20 + 1);
    set_txt_bonus (c, txt_tmp, 2);
  }


  if (player[c].d.h.h != 0 || player[c].delay == 1) {

/**** handling of trails ****/
    if (player[c].delay == 0) {
      int a;
      l = player[c].x2 + player[c].y2 * lvl.square_width;
      square_occupied[l] = (char) (c + 8);
      trail_offset[c] = (char) ((trail_offset[c] - 1) & (maxq - 1));
      trail_pos[c][trail_offset[c]] = l;
      trail_way[c][trail_offset[c]] = square_way[l] =
	DIR8_PAIR (player[c].way, player[c].old_way);
      a = (trail_offset[c] + trail_size[c]) & (maxq - 1);
      if (trail_pos[c][a]
	  != trail_pos[c][(trail_offset[c] + trail_size[c] - 1)
			 & (maxq - 1)]) {
	if (square_occupied[trail_pos[c][a]] == c + 12)
	  square_occupied[trail_pos[c][a]] = 0xff;
	a = (trail_offset[c] + trail_size[c] - 1) & (maxq - 1);
	if (square_occupied[trail_pos[c][a]] == c + 8)
	  square_occupied[trail_pos[c][a]] = (char) (c + 12);
      } else
	square_occupied[trail_pos[c][a]] = (char) (c + 12);

      /* If the player has fire_trail on, trigger explosions on
	 the head and the tail of the trail.  */
      if (player[c].fire_trail) {
	trigger_explosion (trail_pos[c][a], EXPLOSION_IMMEDIATE);
	trigger_explosion (trail_pos[c][trail_offset[c]], EXPLOSION_IMMEDIATE);
      }
    }

    d2 = player[c].y2 * lvl.square_width + player[c].x2;
    if (player[c].delay == 0)
      d2 = lvl.square_move[player[c].way][d2];
    player[c].pos = d2;
    player[c].x2 = square_coord[d2].x;
    player[c].y2 = square_coord[d2].y;
    player[c].d.h.h = 0;

    if (player[c].spec == t_tunnel) {
      player[c].way = player[c].tunnel_way;
      player[c].old_way = player[c].way;
      player[c].spec = 0;
    }

    if (cpuon) {
      if (player[c].target < 16) {
	if ((player[c].cpu & 2) == 0) {
	  if (player[c].behaviour == 1)
	    player[c].next_way = ia_goto_nearest_bonus (c);
	  else if (player[c].behaviour == 2) {
	    if (game_mode == M_KILLEM)
	      player[c].next_way = ia_goto_nearest_lemming (c);
	    if (game_mode == M_TCASH)
	      player[c].next_way = ia_goto_nearest_cash (c);
	    if (game_mode == M_COLOR)
	      player[c].next_way = ia_goto_nearest_color (c);
	  } else
	    player[c].next_way =
	      ia_goto_target (c, player[player[c].target].x2,
			      player[player[c].target].y2);
	}
      } else
	player[c].target -= 16;
    }
    square_occupied[d2] = c;
    if ((square_explo_state[d2] <= EXPLOSION_IMMEDIATE) &&
	player[c].invincible == 0)
      player[c].spec = 0xff;

    d = (player[c].x2 >> 1) + (player[c].y2 >> 1) * lvl.tile_width;

    if (lvl.square_type[player[c].pos] == T_ICE)
      player[c].spec = t_ice;
    if ((lvl.square_type[player[c].pos] == T_STOP && player[c].delay == 0)
	|| player[c].notify_delay) {
      player[c].notify_delay = 0;
      player[c].delay = 100;
      player[c].d.e = 0;
      return;
    }

    if (game_mode == M_KILLEM) {
      tmppti = square_lemmings_list[d2];
      if (tmppti) {
	assert (tmppti >= lemmings_support
		&& tmppti < lemmings_support + lemmings_total);
	if ((tmppti->pos_tail == d2 && lemmings_move_offset < 38000)
	    || (tmppti->pos_head == d2 && lemmings_move_offset > 28000)) {
	  if (!level_is_finished) {
	    player[c].score += 10;
	    player[tmppti->couleur].lemmings_nbr--;
	  }
	  tmppti->dead = (rand () & 15) + 1;
	  if (rand () & 63) {
	    tmppti->couleur = 0;
	    if (player[c].cpu == 2)
	      event_sfx (90 + ((tmppti->dead - 1) >> 1));
	  } else {
	    tmppti->couleur = 1;
	    if (!level_is_finished)
	      player[c].score += 140;
	    player[c].martians_nbr++;
	    if (player[c].cpu == 2)
	      event_sfx (98);
	  }
	  /* We will assign the dead lemming to the nearest square.  */
	  if (lemmings_move_offset < 32536) {
	    /* If it's the tail square, the offset can be kept as-is.  */
	    tmppti->min = lemmings_move_offset;
	    i = tmppti->pos_tail;
	  } else {
	    /* If it's the head square, the offset and the
	       direction needs to be inverted.  */
	    tmppti->min = 65536 - lemmings_move_offset;
	    assert (tmppti->min < 65536);
	    i = tmppti->pos_head;
	    tmppti->dir = REVERSE_DIR (tmppti->dir);
	  }
	  square_lemmings_list[tmppti->pos_tail] = NULL;
	  square_lemmings_list[tmppti->pos_head] = NULL;
	  tmppti->next_dead = square_dead_lemmings_list[i];
	  square_dead_lemmings_list[i] = tmppti;
	}
      }
      for (i = 0; i < 4; i++)
	if (player[(i + 1) & 3].lemmings_nbr == 0 &&
	    player[(i + 2) & 3].lemmings_nbr == 0 &&
	    player[(i + 3) & 3].lemmings_nbr == 0)
	  level_is_finished = i + 1;
    }
    if (game_mode == M_COLOR) {
      t = square_object[d2];
      if ((signed char) t >= 0) {
	if ((!level_is_finished)) {
	  player[c].score += 2;
	  if (player[c].cpu == 2) {
	    if (t == c)
	      event_sfx (100);
	    else if (t <= 4)
	      event_sfx (101);
	    else if (t == 8 + c)
	      event_sfx (102);
	    else if (t <= 12)
	      event_sfx (103);
	    else if (t == 16)
	      event_sfx (104);
	    else if (t == 24)
	      event_sfx (105);
	  }
	  if ((t < 4) && (player[t].spec != 0xde))
	    player[t].cash++;
	  else if (t == 4) {
	    if (player[(c + 1) & 3].spec != 0xde)
	      player[(c + 1) & 3].cash++;
	    if (player[(c + 2) & 3].spec != 0xde)
	      player[(c + 2) & 3].cash++;
	    if (player[(c + 3) & 3].spec != 0xde)
	      player[(c + 3) & 3].cash++;
	  } else if (t < 12) {
	    if (player[t & 3].cash > 0)
	      player[t & 3].cash--;
	  } else if (t == 12) {
	    if ((player[(c + 1) & 3].cash > 0)
		&& (player[(c + 1) & 3].spec != 0xde))
	      player[(c + 1) & 3].cash--;
	    if ((player[(c + 2) & 3].cash > 0)
		&& (player[(c + 2) & 3].spec != 0xde))
	      player[(c + 2) & 3].cash--;
	    if ((player[(c + 3) & 3].cash > 0)
		&& (player[(c + 3) & 3].spec != 0xde))
	      player[(c + 3) & 3].cash--;
	  } else if (t == 16)
	    player[c].time += 1000;
	  else if (t == 24) {
	    if (player[c].time > 333)
	      player[c].time -= 333;
	    else
	      player[c].time = 1;
	  }
	}
	square_object[d2] = -1;
	/* add_color(0); */
	objects_nbr--;
      }
    }

    if (game_mode == M_TCASH) {
      t = square_object[d2];
      if ((signed char) t >= 0) {
	if (!level_is_finished) {
	  player[c].score += 2;
	  if (t == 0) {
	    player[c].cash++;
	    if (player[c].cpu == 2)
	      event_sfx (80);
	  }
	  if (t == 15) {
	    player[c].time += 1000;
	    if (player[c].cpu == 2)
	      event_sfx (81);
	  }
	}
	square_object[d2] = -1;
	/* add_cash(0); */
	objects_nbr--;
      }
    }

    if (game_mode == M_DEATHM) {
      for (i = 0; i < 4; i++)
	if (player[(i + 1) & 3].lifes == 0
	    && player[(i + 2) & 3].lifes == 0
	    && player[(i + 3) & 3].lifes == 0)
	  level_is_finished = i + 1;
    }
    {
      int bonus = tile_bonus[d];
      if (bonus && bonus != 0xff) {
	rem_bonus (d);
	if (!level_is_finished) {
	  player[c].score += 10;
	  if (bonus & 128) {
	    if (player[c].cpu == 2)
	      event_sfx (39 + (bonus & 127));
	    for (i = 0; i < 4; i++)
	      if ((c != i) && (player[i].spec != 0xde))
		apply_bonus (i, (bonus & 127));
	  } else
	    apply_bonus (c, bonus);
	}
	if (player[c].notify_delay) {
	  player[c].notify_delay = 0;
	  player[c].delay = 100;
	  player[c].d.e = 0;
	  return;
	}
      }
    }
    player[c].old_old_way = player[c].old_way;
    player[c].old_way = player[c].way;

    if (player[c].autopilot)
      find_free_way (c);	/* here t_tunnel*4 become t_tunnel*8 !!! */
    if ((!player[c].autopilot)
	&& (player[c].next_way == (player[c].old_way ^ 2)))
      player[c].next_way = player[c].old_way;
    if (player[c].spec != t_ice)
      player[c].way = player[c].next_way;
    else {
      player[c].next_way = player[c].way;
      player[c].spec = 0;
    }

    idx = lvl.square_move[player[c].way][d2];
    if (idx == INVALID_INDEX)
      player[c].spec = 0xff;
    else if (square_occupied[idx] != 0xff)
      player[c].spec = 0xff;

    if (player[c].spec == 0xff) {
      erase_trail (c);
      if (! level_is_finished) {
	if (! player[c].invincible && game_mode != M_DEATHM)
	  shrink_trail (c, 5);
	if (player[c].lifes == 1) {
	  player[c].lifes = 0;
	  player[c].spec = 0xde;
	  if (!(((player[0].cpu & 2) && (player[0].lifes))
		|| ((player[1].cpu & 2) && (player[1].lifes))
		|| ((player[2].cpu & 2) && (player[2].lifes))
		|| ((player[3].cpu & 2) && (player[3].lifes)))) {
	    level_is_finished = 15;
	  }
	  if (player[c].cpu == 2)
	    event_sfx (62);
	  return;
	}
      }
      reinit_player (c);
      if (player[c].lifes != 0 && player[c].invincible == 0
	  && (!level_is_finished)) player[c].lifes--;
      if (!level_is_finished) {
	if (player[c].lifes > 1) {
	  if (player[c].cpu == 2)
	    event_sfx (60);
	} else {
	  if (player[c].cpu == 2)
	    event_sfx (61);
	}
	/* TRANS: %d, the number of remaining lives, is always
	   positive.  */
	sprintf (txt_tmp, ngettext("LAST LIFE", "%d LIVES LEFT",
				   player[c].lifes), player[c].lifes);
      }
      if (!level_is_finished)
	set_txt_bonus (c, txt_tmp, 150);
      player[c].invincible = 350;
      return;
    }

/******************/
    if (lvl.square_type[player[c].pos] == T_TUNNEL
	&& lvl.square_direction[player[c].pos] == player[c].next_way) {
      a_dir dir;
      a_square_index dest;
      player[c].spec = t_tunnel;
      if ((player[c].cpu == 2) && (!level_is_finished))
	event_sfx (69);
      dest = lvl.square_move[player[c].next_way][player[c].pos];
      if (lvl.square_type[dest] == T_TUNNEL)
	dir = lvl.square_direction[dest] ^ 2;
      else
	dir = lvl.square_direction[player[c].pos] ^ 2;
      player[c].tunnel_way = dir;
      player[c].next_way = player[c].tunnel_way;
      if (player[c].tunnel_inverse)
	player[c].next_way ^= 2;
    }
/*****************/

    /*    if (player[c].spec!=t_tunnel*8) */
    {
      if (lvl.square_type[player[c].pos] == T_SPEED) {
	a_dir dir = lvl.square_direction[player[c].pos];

	if (player[c].way == dir)
	  player[c].vi = player[c].v;
	else if ((player[c].way ^ 2) == dir)
	  player[c].vi = -(player[c].v >> 1);
	else
	  player[c].vi = 0;
      } else
	player[c].vi = 0;

      if (lvl.square_type[player[c].pos] == T_DUST)
	player[c].vi = -(player[c].v >> 1);
      trigger_possible_explosion (d2);
    }
    player[c].delay = 0;

    assert (lvl.square_move[player[c].way][d2] != INVALID_INDEX);
    square_occupied[lvl.square_move[player[c].way][d2]] = (char) (c + 4);
  }
}

/* lemming moves */
static void
update_lemmings (void)
{
  int j;
  a_lemming *lem;
  lem = lemmings_support;
  lemmings_move_offset &= 0xffff;
  for (j = lemmings_total; j != 0; j--, lem++)
    if (lem->dead == 0) {
      find_lemming_direction (lem);
    }
}

static int
update_all (char plr)
{
  int n = 0;
  long frames = read_htimer (update_htimer);

  update_explosions ();

  for (; frames; --frames) {
    if (plr) {
      update_player (0);
      update_player (1);
      update_player (2);
      update_player (3);
      update_bonuses ();
      if (radar_current_pos < radar_target_pos)
	radar_current_pos++;
      else if (radar_current_pos > radar_target_pos)
	radar_current_pos--;
    }
    if (game_mode == M_KILLEM) {
      lemmings_move_offset += 1024;
      if (lemmings_move_offset >= 65536)
	update_lemmings ();
    }
    n++;
  }

  return (n);
}


void
play_demo (void)
{
  int n, i;
  char notbyebye = 1;
  a_timer demo_htimer = new_htimer (T_GLOBAL, HZ (1));
  a_fader_status fade_stat = F_UNKNOWN;

  dmsg (D_SECTION, "-- play demo --");

  in_menu = 0;
  tutor = 0;
  load_random_wrapped_level (false, 0);

  play_soundtrack ();
  set_pal_with_luminance (&tile_set_img.palette);
  radar_current_pos = 81;
  radar_target_pos = 0;
/* .... game .... */
  if (two_players) {
    nbr_tiles_cols = 8;
  }
  init_camera ();
  init_keyboard_map ();
  n = 1;

  update_htimers ();
  reset_htimer_with_offset (event_htimer, 4);
  reset_htimer (clock_htimer);
  reset_htimer (bonus_anim_htimer);
  reset_htimer (tiles_anim_htimer);
  reset_htimer (blink_htimer);
  reset_htimer (update_htimer);
  output_screen ((char) n);	/* update corner[] */
/*   corner[0]=render_buffer[0]; */
/*   corner[1]=render_buffer[1]; */

  if (two_players == false) {
    int flip_pos;
    a_pendulum *p = pendulum_create ();
    n = 0;
    while (pendulum_update (p, &flip_pos) < 3000) {
      flip_buffer (flip_pos);
      flush_display (render_buffer[1]);
      output_screen ((char) n);
      n = update_all (0);
    }
    pendulum_destroy (p);
  }
  if (two_players) {
    int buffer_pos = 39;
    do {
      flush_display2_moving (buffer_pos);
      output_screen ((char) n);
      n = update_all (0);
      for (i = n; i > 0; i--)
	if (buffer_pos > 0)
	  buffer_pos--;
    } while (buffer_pos != 0);
  }

/* * * * * * * * * * *\
* MAIN LOOP in demos  *
\* * * * * * * * * * */
  dmsg (D_SECTION, "demo main loop");

  do {
    event_time = read_htimer (event_htimer);
    update_text_waving_step ();
    if (enable_blit) {
      long duration = read_htimer (demo_htimer);
      if (duration >= (DEMO_DURATION - 1) && fade_stat == F_UNKNOWN) {
	dmsg (D_MISC, "end of demo time reached");
	/* startup a black fade-out */
	std_black_fadeout (&tile_set_img.palette);
	fader_status_flagback (&fade_stat);
      }
      if (two_players == false) {
	flush_display (corner[0]);
      } else {
	flush_display2 (corner[0], corner[1]);
      }
    }
    enable_blit = 1;
    output_screen ((char) n);
    process_input_events ();
    if (level_is_finished == 0)
      n = update_all (1);

    if (devparm && keyboard_map[HK_F12])
      level_is_finished = 1;

    if (opt.ctrl_one ^ opt.ctrl_two)
      if (is_joystick_button_b (1) && enable_blit)
	notbyebye = 0;

    if (keyboard_map[HK_Escape] && enable_blit)
      notbyebye = 0;

    if (!(notbyebye && level_is_finished == 0) && fade_stat == F_UNKNOWN) {
      /* startup a black fade-out */
      std_black_fadeout (&tile_set_img.palette);
      fader_status_flagback (&fade_stat);
    }
  } while (fade_stat != F_FINISHED);

  free_htimer (demo_htimer);
  uninit_keyboard_map ();
  unload_level ();
  nbr_tiles_cols = 15;
  in_menu = 1;
  reset_htimer (background_htimer);
}

static void
load_demo (void)
{
  char olddeuxplr;
  char gamemodeh;
  int i;

  dmsg (D_SECTION, "-- load demo --");

  for (i = 3; i >= 0; i--)
    col2plr[i] = opt.player_color[i];
  for (i = 3; i >= 0; i--)
    plr2col[opt.player_color[i]] = i;
  gamemodeh = (rand () & 3) + 2;
  olddeuxplr = two_players;
  two_players = rand () & 1;
  if (gamemodeh == 2)
    game_mode = M_DEATHM;
  else if (gamemodeh == 3)
    game_mode = M_KILLEM;
  else if (gamemodeh == 4)
    game_mode = M_TCASH;
  else if (gamemodeh == 5)
    game_mode = M_COLOR;
  current_quest_level = 0;

  unload_level ();		/* stop also the music */
  in_demo = 1;
  if (opt.sfx)
    load_sfx_mode (game_mode);
  rounds = 1;
  play_demo ();
  game_mode = M_QUEST;
  in_demo = 0;
  two_players = olddeuxplr;
  load_random_wrapped_level (true, 0);
  load_sfx_mode (-1);

  play_soundtrack ();
  reset_htimer (sound_track_htimer);
  reset_htimer (demo_trigger_htimer);
}

static void
main_menu (void)
{
  static char l = 0;
  a_keycode t;
  char flag = 0;

  dmsg (D_SECTION, "-- menu --");

  load_random_wrapped_level (true, 0);
  load_sfx_mode (-1);

  play_soundtrack ();
  reset_htimer (sound_track_htimer);
  reset_htimer (background_htimer);
  reset_htimer (demo_trigger_htimer);
  reset_htimer_with_offset (event_htimer, 4);
  event_time = read_htimer (event_htimer);
  do {
    std_white_fadein (&tile_set_img.palette);
    do {
      background_menu ();
      draw_main_menu (l);
      flush_display (corner[0]);

      if (key_or_joy_ready () || demo_ready) {
	if (demo_ready == 0)
	  t = get_key_or_joy ();
	else
	  t = 0;
	if (t == HK_Up || t == HK_Down || t == HK_Escape)
	  event_sfx (1);
	if (t == HK_Up) {
	  if (l > 0)
	    l--;
	  else
	    l = 6;
	}
	if (t == HK_Down) {
	  if (l < 6)
	    l++;
	  else
	    l = 0;
	}
	if (t == HK_Escape) {
	  if (l != 6)
	    l = 6;
	  else
	    t = HK_Enter;
	}
	if (devparm && (t == HK_s || t == HK_S))
	  end_scroll ();
	if (t == HK_d || t == HK_D || demo_ready) {
	  a_timer pixelize_timer;
	  long pixelize_pos;

	  event_sfx (130);
	  reset_htimer (corner_htimer);
	  for (;;) {
	    background_menu ();
	    draw_main_menu (l);
	    if (corner_buffer ())
	      break;
	    flush_display (corner[0]);
	  }
	  load_demo ();

	  if (check_what == check_demo)
	    exit_heroes (0);

	  dmsg (D_SECTION, "-- (back to) menu (from demo) --");
	  demo_ready = 0;
	  event_sfx (131);
	  std_white_fadein (&tile_set_img.palette);
	  pixelize_timer = new_htimer (T_GLOBAL, HZ (7));
	  {
	    a_pixel *pixbuf;
	    XMALLOC_ARRAY (pixbuf, xbuf * 200);
	    do {
	      background_menu ();
	      draw_main_menu (l);
	      pixelize_pos = read_htimer (pixelize_timer);
	      if (pixelize_pos > 6)
		pixelize_pos = 6;
	      pixelize[6 - pixelize_pos] (pixbuf, corner[0]);
	      flush_display (pixbuf);
	    } while (pixelize_pos < 6);
	    free (pixbuf);
	  }
	  free_htimer (pixelize_timer);
	}

      } else
	t = 0;
    } while (t != HK_Enter);
    event_sfx (10 + l);

    if (l == 0)
      play_menu ();
    if (l == 1) {
      option_menu ();
      save_preferences ();
    }
    if (l == 2)
      help_menu ();
    if (l == 3)
      jukebox_menu ();
    if (l == 4)
      scores_menu ();
    if (l == 5) {
      if (mouse_found)
	editor_first_menu ();
    }
    if (l == 6)
      flag = quit_menu ();
  } while (flag == 0);


  /* pixelize and fade-out, before exit */
  {
    a_timer pixelize_timer;
    long pixelize_pos;
    a_fader_status fade_stat;
    a_pixel *pixbuf;
    XMALLOC_ARRAY (pixbuf, 200 * xbuf);
    pixelize_timer = new_htimer (T_GLOBAL, HZ (7));
    std_black_fadeout (&tile_set_img.palette);
    fader_status_flagback (&fade_stat);
    do {
      background_menu ();
      draw_quit_menu (0);
      pixelize_pos = read_htimer (pixelize_timer);
      if (pixelize_pos > 6)
	pixelize_pos = 6;
      pixelize[pixelize_pos] (pixbuf, corner[0]);
      flush_display (pixbuf);
    } while (fade_stat != F_FINISHED);
    free (pixbuf);
    free_htimer (pixelize_timer);
  }
}

static void
get_input_directions (void)
{				/* for humans only */
  unsigned char j = (opt.ctrl_one == 1);

  char flag1 = 0;
  char flag2 = 0;

  if (keyboard_map[HK_Pause] && enable_blit)
    pause_menu ();

  if (opt.ctrl_one == 0) {

    if (keyboard_map[opt.player_keys[0][0]]) {
      player[col2plr[0]].next_way = w_up;
      flag1 = 1;
    }
    if (keyboard_map[opt.player_keys[0][1]]) {
      player[col2plr[0]].next_way = w_left;
      flag1 = 1;
    }
    if (keyboard_map[opt.player_keys[0][2]]) {
      player[col2plr[0]].next_way = w_down;
      flag1 = 1;
    }
    if (keyboard_map[opt.player_keys[0][3]]) {
      player[col2plr[0]].next_way = w_right;
      flag1 = 1;
    }
    player[col2plr[0]].turbo = keyboard_map[opt.player_keys[0][4]] ? 2 : 1;
    if (keyboard_map[opt.player_keys[0][5]]) {
      if (player[col2plr[0]].turbo == 2) {
	/* two buttons pushed */
	player[col2plr[0]].turbo = 1;
      } else
	player[col2plr[0]].turbo = 0;
    }
  } else {
    joystick_x[0] = 0;
    joystick_y[0] = 0;
    joystick_x[1] = 0;
    joystick_y[1] = 0;
    get_joystick_state ();
    if (is_joystick_up (0)) {
      player[col2plr[0]].next_way = w_up;
      flag1 = 1;
    }
    if (is_joystick_left (0)) {
      player[col2plr[0]].next_way = w_left;
      flag1 = 1;
    }
    if (is_joystick_down (0)) {
      player[col2plr[0]].next_way = w_down;
      flag1 = 1;
    }
    if (is_joystick_right (0)) {
      player[col2plr[0]].next_way = w_right;
      flag1 = 1;
    }
    player[col2plr[0]].turbo = (is_joystick_button_a (0) ? 2 : 1);
    if (is_joystick_button_b (0)) {
      if (player[col2plr[0]].turbo == 2) {
	/* two buttons pushed */
	player[col2plr[0]].turbo = 1;
      } else
	player[col2plr[0]].turbo = 0;
    }
  }

  if (player[col2plr[0]].inversed_controls != 0) {
    if (flag1)
      player[col2plr[0]].next_way ^= 2;
    player[col2plr[0]].tunnel_inverse = 1;
  } else
    player[col2plr[0]].tunnel_inverse = 0;
  if (((player[col2plr[0]].next_way ^ 2) == player[col2plr[0]].tunnel_way)
      && (player[col2plr[0]].spec == t_tunnel))
    player[col2plr[0]].next_way = player[col2plr[0]].tunnel_way;
  if (two_players == true) {
    if (opt.ctrl_two == 0) {
      if (keyboard_map[opt.player_keys[1][0]]) {
	player[col2plr[1]].next_way = w_up;
	flag2 = 1;
      }
      if (keyboard_map[opt.player_keys[1][1]]) {
	player[col2plr[1]].next_way = w_left;
	flag2 = 1;
      }
      if (keyboard_map[opt.player_keys[1][2]]) {
	player[col2plr[1]].next_way = w_down;
	flag2 = 1;
      }
      if (keyboard_map[opt.player_keys[1][3]]) {
	player[col2plr[1]].next_way = w_right;
	flag2 = 1;
      }
      player[col2plr[1]].turbo = keyboard_map[opt.player_keys[1][4]] ? 2 : 1;
      if (keyboard_map[opt.player_keys[1][5]]) {
	if (player[col2plr[1]].turbo == 2) {
	  /* two buttons pushed */
	  player[col2plr[1]].turbo = 1;
	} else
	  player[col2plr[1]].turbo = 0;
      }
    } else {
      if (!j) {
	joystick_x[0] = 0;
	joystick_y[0] = 0;
	get_joystick_state ();
      }
      if (is_joystick_up (j)) {
	player[col2plr[1]].next_way = w_up;
	flag2 = 1;
      }
      if (is_joystick_left (j)) {
	player[col2plr[1]].next_way = w_left;
	flag2 = 1;
      }
      if (is_joystick_down (j)) {
	player[col2plr[1]].next_way = w_down;
	flag2 = 1;
      }
      if (is_joystick_right (j)) {
	player[col2plr[1]].next_way = w_right;
	flag2 = 1;
      }
      player[col2plr[1]].turbo = (is_joystick_button_a (j) ? 2 : 1);
      if (is_joystick_button_b (j)) {
	if (player[col2plr[1]].turbo == 2) {
	  /* two buttons pushed */
	  player[col2plr[1]].turbo = 1;
	} else
	  player[col2plr[1]].turbo = 0;
      }
    }
    if (player[col2plr[1]].inversed_controls != 0) {
      if (flag2)
	player[col2plr[1]].next_way ^= 2;
      player[col2plr[1]].tunnel_inverse = 1;
    } else
      player[col2plr[1]].tunnel_inverse = 0;

/*   if ((player[col2plr[1]].spec==t_tunnel)) printf("ss:%d,nxss:%d,ssold:%d,ssold2:%d,sstun:%d.\n", */
/*       player[col2plr[1]].way,player[col2plr[1]].next_way,player[col2plr[1]].old_way,player[col2plr[1]].old_old_way,player[col2plr[1]].tunnel_way); */
    if (((player[col2plr[1]].next_way ^ 2) == player[col2plr[1]].tunnel_way)
	&& (player[col2plr[1]].spec == t_tunnel))
      player[col2plr[1]].next_way = player[col2plr[1]].tunnel_way;
  }
  if (opt.ctrl_one ^ opt.ctrl_two) {
    if (is_joystick_button_a (1) && enable_blit)
      keyboard_map[HK_Pause] = 1;
  }
}

static unsigned char
play_game (char cont)
{
  int n, i;
  a_keycode t;
  char notbyebye = 1, flag;
  int l = 0, pos, u;
  char editflag = 0;
  static char tmpname[20];
  char bufstr[32];
  a_sprite *levelname;

  dmsg (D_SECTION, "-- play game --");

  in_menu = 0;
  tutor = (char) (game_mode == M_QUEST && current_quest_level == 0);
  if (loadulevel == 1) {
    char* tmp = get_non_null_rsc_file ("levels-dir");
    strappend (tmp, level_name);
    if (load_level (level_name, cont))
      emsg (_("Error during level loading"));
    free (tmp);
  } else if (game_mode == M_QUEST /*&& questmode==0 */ )
    load_level_from_number (current_quest_level++, cont);
/* else if (game_mode==M_QUEST) loadlvlpasrandq2(current_quest_level++,cont); */
  else
    load_random_level (cont);

  play_soundtrack ();
  reset_htimer (sound_track_htimer);
  set_pal_with_luminance (&tile_set_img.palette);
  radar_current_pos = 81;
  radar_target_pos = 0;

/* . ... game ... . */
  if (two_players) {
    nbr_tiles_cols = 8;
  }
  init_camera ();
  init_keyboard_map ();
  n = 1;

  update_htimers ();
  reset_htimer_with_offset (event_htimer, 4);
  reset_htimer (clock_htimer);
  reset_htimer (bonus_anim_htimer);
  reset_htimer (tiles_anim_htimer);
  reset_htimer (blink_htimer);
  reset_htimer (update_htimer);

  dmsg (D_SECTION, "introduce game");

  output_screen ((char) n);	/* update corner[] */
  process_input_events ();

  if (game_mode == M_QUEST)
    sprintf (bufstr, _("-LEVEL %d-"), current_quest_level);
  else
    sprintf (bufstr, _("-ROUND %d/%d-"),
	     rounds_nbr_values[opt.gamerounds] - rounds + 1,
	     rounds_nbr_values[opt.gamerounds]);

  levelname = compile_menu_text (bufstr, T_CENTERED, 99, 159);

  if (two_players == false) {
    int pendulum_pos;
    a_pendulum *p = pendulum_create ();
    n = 0;
    while (pendulum_update (p, &pendulum_pos) < 3000) {
      flip_buffer (pendulum_pos);

      DRAW_SPRITE (levelname, render_buffer[1]);

      flush_display (render_buffer[1]);
      output_screen ((char) n);
      process_input_events ();
      n = update_all (0);
    }
    pendulum_destroy (p);
  }
  if (two_players) {
    int buffer_pos = 39;

    corner[0] = render_buffer[0];
    clear_scr_area (corner[0]);
    DRAW_SPRITE (levelname, corner[0]);
    flush_display (corner[0]);

    sleep (1);
    update_htimers ();
    reset_htimer (update_htimer);

    n = 1;
    do {
      DRAW_SPRITE (levelname, corner[swapside] + (buffer_pos << 2));
      DRAW_SPRITE (levelname, corner[1 - swapside] - (buffer_pos << 2) - 160);
      flush_display2_moving (buffer_pos);
      output_screen ((char) n);
      process_input_events ();
      n = update_all (0);
      for (i = n; i > 0; i--)
	if (buffer_pos > 0)
	  buffer_pos--;
    } while (buffer_pos != 0);

  }
  event_sfx (141);

  free_sprite (levelname);

/* * * * * * * * * * * * * * *\
* MAIN LOOP during the games *
\* * * * * * * * * * * * * * */

  dmsg (D_SECTION, "game main loop");

  do {
    event_time = read_htimer (event_htimer);
    update_text_waving_step ();
    if (enable_blit) {
      if (two_players == false) {
	flush_display (corner[0]);
      } else {
	flush_display2 (corner[0], corner[1]);
      }
    }
    enable_blit = 1;
    output_screen ((char) n);
    process_input_events ();
    get_input_directions ();
    n = update_all (1);
    if (devparm && keyboard_map[HK_F12])
      level_is_finished = 1;
    if (opt.ctrl_one ^ opt.ctrl_two)
      if (is_joystick_button_b (1) && enable_blit)
	notbyebye = quit_yes_no ();
    if (keyboard_map[HK_Escape] && enable_blit)
      notbyebye = quit_yes_no ();
  } while (notbyebye && level_is_finished == 0);

  dmsg (D_SECTION, "game finished");

  if (level_is_finished >= 1 && level_is_finished <= 4)
    player[level_is_finished - 1].wins++;

  radar_target_pos = 81;
  if (notbyebye) {

    dmsg (D_SECTION, "print end level info");

    uninit_keyboard_map ();
    l = 0;
    if (level_is_finished != 15)
      if (player[level_is_finished - 1].cpu == 2)
	event_sfx (63);
      else
	event_sfx (64);
    else
      event_sfx (65);
    do {
      event_time = read_htimer (event_htimer);
      update_text_waving_step ();
      if (two_players == false) {
	draw_end_level_info (0, l);
	flush_display (corner[0]);
      } else {
	a_pixel* tmp;

	tmp = corner[0];
	draw_end_level_info (swapside ? -160 : 0, l);
	corner[0] = corner[1];
	draw_end_level_info (swapside ? 0 : -160, l);
	corner[0] = tmp;
	flush_display2 (corner[0], corner[1]);
      }
      output_screen ((char) n);
      n = update_all (1);
      t = 0;
      if (key_or_joy_ready ()) {
	t = get_key_or_joy ();
	if (t == HK_Down && game_mode == M_QUEST)
	  l = 1;
	if (t == HK_Up)
	  l = 0;
      }
      flag = 1;
      if (opt.ctrl_one || opt.ctrl_two) {
	get_joystick_state ();
	if (is_joystick_down (0))
	  l = 1;
	if (is_joystick_up (0))
	  l = 0;
	if (is_joystick_button_a (0))
	  flag = 0;
	if (opt.ctrl_one && opt.ctrl_two) {
	  if (is_joystick_down (1))
	    l = 1;
	  if (is_joystick_up (1))
	    l = 0;
	  if (is_joystick_button_a (1))
	    flag = 0;
	}
      }
      if (l == 1 && (t == HK_Enter || flag == 0) && game_mode == M_QUEST) {
	event_sfx (67);
	load_save_records ();
	t = 0;
	l = 0;
	editflag = 0;
	do {
	  if (two_players == false) {
	    draw_saved_games_info (0, l, true);
	    flush_display (corner[0]);
	  } else {
	    a_pixel *tmp;

	    tmp = corner[0];
	    draw_saved_games_info (swapside ? -160 : 0, l, true);
	    corner[0] = corner[1];
	    draw_saved_games_info (swapside ? 0 : -160, l, true);
	    corner[0] = tmp;
	    flush_display2 (corner[0], corner[1]);
	  }
	  output_screen ((char) n);
	  n = update_all (1);
	  if (key_or_joy_ready ()) {
	    if (editflag == 2)
	      editflag = 0;
	    if (editflag == 0) {
	      t = get_key_or_joy ();
	      if (t == HK_Down) {
		((l < 9) ? (l++) : (l = 0));
		event_sfx (120);
	      }
	      if (t == HK_Up) {
		((l > 0) ? (l--) : (l = 9));
		event_sfx (120);
	      }
	      if (t == HK_BackSpace || t == HK_Delete) {
		saverec[l].used = 0;
		saverec[l].name[0] = 0;
		event_sfx (128);
		FREE_SPRITE0 (saverec_name[l]); /* force recompilation */
		write_save_one_record (l);
	      }
	      if (t == HK_Enter) {
		editflag = 1;
		strcpy (tmpname, saverec[l].name);
		pos = strlen (saverec[l].name);
		saverec[l].name[pos] = '^';
		saverec[l].name[pos + 1] = 0;
		event_sfx (125);
		FREE_SPRITE0 (saverec_name[l]); /* force recompilation */
	      }
	      if (t == HK_Escape)
		event_sfx (123);
	    } else {
	      t = get_key_or_joy ();
	      u = keycode_to_ascii (t);
	      pos = strlen (saverec[l].name);
	      if (u >= 'a' && u <= 'z')
		u -= 'a' - 'A';
	      if (pos < 13)
		if ((u > 20 && u <= 95) || (u == 20 && pos != 0)) {
		  saverec[l].name[pos - 1] = u;
		  saverec[l].name[pos] = '^';
		  saverec[l].name[pos + 1] = 0;
		  event_sfx (121);
		  FREE_SPRITE0 (saverec_name[l]); /* force recompilation */
		}
	      if ((t == HK_BackSpace || t == HK_Delete) && (pos > 1)) {
		saverec[l].name[pos - 1] = 0;
		saverec[l].name[pos - 2] = '^';
		event_sfx (122);
		FREE_SPRITE0 (saverec_name[l]); /* force recompilation */
	      } else if (t == 0x1c0a) { /* FIXME: choose a keysym to use */
		saverec[l].name[pos - 1] = 0;
		event_sfx (127);
		editflag = 2;
		FREE_SPRITE0 (saverec_name[l]); /* force recompilation */
	      } else if (t == HK_Escape) {
		strcpy (saverec[l].name, tmpname);
		event_sfx (123);
		editflag = 2;
		FREE_SPRITE0 (saverec_name[l]); /* force recompilation */
	      } else if (t == HK_Enter) {
		saverec[l].name[pos - 1] = 0;
		saverec[l].level = current_quest_level /*+1 */ ;
		for (u = 0; u < 4; u++) {
		  saverec[l].points[u] = player[col2plr[u]].score;
		  copy_gameid (saverec[l].gid, game_id);
		  saverec[l].lifes[u] = player[col2plr[u]].lifes;
		}
		saverec[l].used = 1;
		editflag = 0;
		event_sfx (124);
		write_save_one_record (l);
		FREE_SPRITE0 (saverec_name[l]); /* force recompilation */
	      }
	    }
	  }
	} while ((t != HK_Escape && t != HK_Enter) || editflag != 0);
	/* while (keyboard_map[HK_Escape]) process_input_events (); */
	l = 1;
      } else if (t == HK_Escape) {
	if (joystick_detected & 1)
	  do
	    get_joystick_state ();
	  while (is_joystick_button_a (0) != 0);
	if (quit_yes_no () == 0)
	  l = 255;
      }
    } while ( /*keyboard_map[HK_Escape]==0 */ l != 255
	     && ((t != HK_Enter && flag) || l == 1));
    init_keyboard_map ();

    if (l == 255 || game_mode == M_QUEST) {
      if (l == 0)
	event_sfx (66);		/* next level */
      if (l == 1)
	event_sfx (67);		/* save game */
      if (l == 255)
	event_sfx (68);		/* echap */
      for (i = 1; i <= 40; i += n) {
	event_time = read_htimer (event_htimer);
	update_text_waving_step ();
	if (two_players == false) {
	  draw_end_level_info (0, l);
	  flush_display_moving (i);
	} else {
	  a_pixel *tmp;

	  tmp = corner[0];
	  draw_end_level_info (swapside ? -160 : 0, l);
	  corner[0] = corner[1];
	  draw_end_level_info (swapside ? 0 : -160, l);
	  corner[0] = tmp;
	  flush_display2_moving (i);
	}
	output_screen ((char) n);
	process_input_events ();
	n = update_all (1);
      }
      if (two_players == false) {
	draw_end_level_info (0, l);
	flush_display_moving (40);
      } else {
	a_pixel *tmp;
	tmp = corner[0];
	draw_end_level_info (swapside ? -160 : 0, l);
	corner[0] = corner[1];
	draw_end_level_info (swapside ? 0 : -160, l);
	corner[0] = tmp;
	flush_display2_moving (40);
      }
    } else {
      dmsg (D_SECTION, "print round info");

      l = 0;
      flag = 1;

      event_sfx (129);
      do {
	event_time = read_htimer (event_htimer);
	update_text_waving_step ();
	if (two_players == false) {
	  draw_round_info (0);
	  flush_display (corner[0]);
	} else {
	  a_pixel *tmp;
	  tmp = corner[0];
	  draw_round_info (swapside ? -160 : 0);
	  corner[0] = corner[1];
	  draw_round_info (swapside ? 0 : -160);
	  corner[0] = tmp;
	  flush_display2 (corner[0], corner[1]);
	}
	if (! keyboard_map[HK_Enter])
	  flag = 0;
	output_screen ((char) n);
	process_input_events ();
	n = update_all (1);
	if (keyboard_map[HK_Escape])
	  if (quit_yes_no () == 0)
	    l = 255;
      } while ( /*! keyboard_map[HK_Escape] */ l != 255
	       && (flag || ! keyboard_map[HK_Enter]));
/*   if (keyboard_map[HK_Escape]) l=255; */

      if (l == 0)
	event_sfx (66);		/*next */
      if (l == 1)
	event_sfx (67);		/*save */
      if (l == 255)
	event_sfx (68);		/*esc */
      for (i = 1; i <= 40; i += n) {
	event_time = read_htimer (event_htimer);
	update_text_waving_step ();
	if (two_players == false) {
	  draw_round_info (0);
	  flush_display_moving (i);
	} else {
	  a_pixel *tmp;
	  tmp = corner[0];
	  draw_round_info (swapside ? -160 : 0);
	  corner[0] = corner[1];
	  draw_round_info (swapside ? 0 : -160);
	  corner[0] = tmp;
	  flush_display2_moving (i);
	}
	output_screen ((char) n);
	process_input_events ();
	n = update_all (1);
      }
      if (two_players == false) {
	draw_round_info (0);
	flush_display_moving (40);
      } else {
	a_pixel *tmp;
	tmp = corner[0];
	draw_round_info (swapside ? -160 : 0);
	corner[0] = corner[1];
	draw_round_info (swapside ? 0 : -160);
	corner[0] = tmp;
	flush_display2_moving (40);
      }
/* end of round info */
    }

  }
  if ((!notbyebye) || (level_is_finished == 15))
    l = 255;
  uninit_keyboard_map ();
  unload_level ();
  nbr_tiles_cols = 15;
  in_menu = 1;
/* if (l!=0) cont=0; */

  return (l);
}

int
heroes_main (int argc, char *argv[])
{
  int i;

  mtrace (); /* GNU libc's malloc debugging facility */

  dmsg_init (argv[0]);
  dmsg (D_SECTION, "initialization");

  var_initialize ();		/* Needed by init_persona.  */
  init_persona ();

  relocate_data (argv[0]);
  init_locales ();
  init_sound_track_list ();

  /* Allow to override system-conf.  That's especially used by the
     testsuite; and not documented (although it could).  */
  {
    char *system_conf = getenv ("HEROES_SYSTEM_CONF");
    if (system_conf) {
      dmsg (D_SYSTEM, "HEROES_SYSTEM_CONF = %s", system_conf);
      set_rsc_file ("system-conf", system_conf, false);
    }
  }

  /* Read the system-wide configuration file. */
  {
    char* tmp;
    bool sec = true;
    tmp = get_rsc_file_secure ("system-conf", &sec);
    if (tmp) {
      read_userconf (tmp, sec);
      free (tmp);
    }
  }

  if (setup_userdir ())
    exit (1);

  init_scores ();
  init_save_records ();
  user_persona_definitively ();

  add_default_extra_directories ();

  /* Read the user configuration file. */
  {
    char* tmp;
    bool sec = true;
    tmp = get_rsc_file_secure ("user-conf", &sec);
    if (tmp) {
      read_userconf (tmp, sec);
      free (tmp);
    }
  }

  /* Reopen the score and save file, because the user
     might have changed them from his configuration file.  */
  reinit_scores_if_needed ();
  reinit_save_records_if_needed ();

  freeze_sound_track_list ();

  dmsg (D_SYSTEM, "parsing command line");
  {
    int err = parse_argv (argc, argv, 0, 0);
    if (err < 0)
      exit (0);
    if (err > 0)
      exit (err);
  }

  dmsg (D_SYSTEM, "randomize");
  srand (time (0));

  read_level_list ();
  if (showlevels) {
    print_level_list ();
    exit (0);
  }

  browse_extra_directories ();
  if (reinitopt)
    reinit_preferences ();
  else
    load_preferences ();
  if (showprefs) {
    output_preferences (stdout);
    exit (0);
  }

  /* FIXME: remove this option.  */
  if (reinitsco) {
    clear_scores ();
    write_scores ();
  }

  if (reinitsav) {
    clear_save_records ();
    write_save_records ();
  }

  if (joyoff) {
    joystick_detected = 0;
    /* reset controlers configuration to keyboards */
    opt.ctrl_one = 0;
    opt.ctrl_two = 0;
  } else
    joyinit ();

  sys_persona ();
  if (init_sound_engine ())
    exit (2);
  user_persona ();

  /* We read the SFX configuration only once the sound engine has
     been initialized, because during this initialization we might
     decide to disable SFX (hence no need to load the config file).
     */
  if (read_sfx_conf ())
    emsg (_("error in sfx.cfg"));

  sys_persona ();
  init_video ();
  user_persona ();

  init_htimer ();
  clock_htimer = new_htimer (T_GLOBAL, HZ (10));
  blink_htimer = new_htimer (T_GLOBAL, HZ (6));
  event_htimer = new_htimer (T_GLOBAL, HZ (70));
  waving_htimer = new_htimer (T_GLOBAL, HZ (70));
  corner_htimer = new_htimer (T_GLOBAL, HZ (280));
  update_htimer = new_htimer (T_LOCAL, HZ (70));
  background_htimer = new_htimer (T_LOCAL, HZ (70));
  sound_track_htimer = new_htimer (T_GLOBAL, HZ (2));
  tiles_anim_htimer = new_htimer (T_GLOBAL, HZ (70));
  demo_trigger_htimer = new_htimer (T_GLOBAL, HZ (1));
  init_text_waving_step ();
  init_fader ();

  if (!directmenu || check_what == check_intro) {
    play_intro ();
    if (check_what == check_intro)
      exit_heroes (0);
  }

  init_buffers ();
  init_bonuses ();

  pcx_load_from_rsc ("main-font", &main_font_img);
  pcx_load_from_rsc ("vehicles-img", &vehicles_img);
  pcx_load_from_rsc ("jukebox-font", &font_deck_img);

  init_fonts ();
  init_explosions ();
  init_items ();

  for (i = 0; i != 32; i++)
    minisinus[i] = ceil (sin (i * 2.0 * 3.141592653 / 32.0) * 1.7);
  compute_lut ();

  init_menus_sprites ();
  dummy_moving_background_init ();
  update_htimers ();
  main_menu ();

  dummy_moving_background_uninit ();
  uninit_menus_sprites ();
  uninit_fader ();
  uninit_text_waving_step ();

  uninit_items ();
  uninit_explosions ();
  uninit_fonts ();

  free_htimer (demo_trigger_htimer);
  free_htimer (sound_track_htimer);
  free_htimer (tiles_anim_htimer);
  free_htimer (background_htimer);
  free_htimer (update_htimer);
  free_htimer (waving_htimer);
  free_htimer (event_htimer);
  free_htimer (corner_htimer);
  free_htimer (clock_htimer);
  free_htimer (blink_htimer);

  img_free (&font_deck_img);
  img_free (&vehicles_img);
  img_free (&main_font_img);
  free_all_sfx ();
  close_sfx_handle ();
  unload_level ();
  free_level_list ();
  uninit_bonuses ();
  close_buffers ();
  uninit_sound_engine ();
  uninit_video ();
  var_uninitialize ();
  uninit_sound_track_list ();
  free_extra_list ();
  free_extra_directories ();
  save_preferences ();
  free_save_records ();
  free_scores ();
  free_preferences ();
  free_userdir ();
  free_modified_rsc ();
  return 0;
}
