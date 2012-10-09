/*------------------------------------------------------------------.
| Copyright 2001, 2002  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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
#include "error.h"
#include "lvl.h"

const char *program_name;	/* argv[0] */
int exit_status = 0;		/* $? */

struct an_option_set {
  bool print_directions;	/* --print d */
  bool print_filename;		/* --print f */
  bool print_header;		/* --print h */
  bool print_walls;		/* --print w */
  bool print_types;		/* --print t */
  bool print_type_keys;		/* --print T */
  bool print_tunnels;		/* --print @ */
  bool print_tile_details;	/* --print i */
  const char *indent;		/* Set to a prefix output on every
				   lines, e.g. "  ". (--indent) */
} options = {
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  ""
};

static void
default_options (void)
{
  options.print_filename = true;
  options.print_header = true;
  options.indent = "  ";
}

static void
version (void)
{
  puts ("heroeslvl (Heroes) " VERSION "\n");
  printf (_("Copyright (C) %d  Alexandre Duret-Lutz.\n"), 2002);
  puts (_("This is free software; see the source for copying conditions.  "
	  "There is NO\nwarranty; not even for MERCHANTABILITY or FITNESS"
	  " FOR A PARTICULAR PURPOSE."));
  exit (0);
}

static void
usage (int status)
{
  if (status) {
    fprintf (stderr, _("Try '%s --help' for more information.\n"),
	     program_name);
    exit (status);
  }

  printf (_("Usage: %s [OPTIONS]... levels\n\n"), program_name);
  puts (_("Heroeslvl is a tool used to inspect Heroes' level files.\n"));
  puts (_("\
Mandatory arguments to long options are mandatory for short options too.\n"));
  puts (_("\
  -v, --version               display version number"));
  puts (_("\
  -h, --help                  display this help"));
  puts (_("\
  -p, --print=WHAT            select information to display.  WHAT should be\n\
                                one or more of these characters:"));
  printf ("%34s%s\n", "", _("d   print square directions"));
  printf ("%34s%s\n", "", _("f   print filename"));
  printf ("%34s%s\n", "", _("h   print header"));
  printf ("%34s%s\n", "", _("i   print tile details"));
  printf ("%34s%s\n", "", _("t   print square type map"));
  printf ("%34s%s\n", "", _("T   print type keys"));
  printf ("%34s%s\n", "", _("w   print square wall map"));
  printf ("%34s%s\n", "", _("@   print tunnels"));
  puts (_("\
  -i, --indent                indent everything but the filename"));
  puts ("");
  puts (_("When no options are given, the default is -ipfh."));
  puts (_("Report bugs to <heroes-bugs@lists.sourceforge.net>."));
  exit (status);
}

const struct option long_options[] = {
  {"help",		no_argument,		NULL,	'h'},
  {"indent",		no_argument,		NULL,   'i'},
  {"print",		required_argument,	NULL,	'p'},
  {"version",		no_argument,		NULL,	'v'},
  {NULL,		0,			NULL,	0}
};

static void
decode_print_options (const char *flags)
{
  while (*flags) {
    switch (*flags) {
    case 'd':
      options.print_directions = true;
      break;
    case 'f':
      options.print_filename = true;
      break;
    case 'h':
      options.print_header = true;
      break;
    case 'i':
      options.print_tile_details = true;
      break;
    case 't':
      options.print_types = true;
      break;
    case 'T':
      options.print_type_keys = true;
      break;
    case 'w':
      options.print_walls = true;
      break;
    case '@':
      options.print_tunnels = true;
      break;
    default:
      error (0, 0, "Unknown --print selection '%c'.", *flags);
      usage (1);
    }
    ++flags;
  }
}

static void
decode_switches (int argc, char **argv)
{
  int c;
  /* If DEFAULT_PRINT is still true after processing all the options,
     that means the default printing options should be used.  */
  bool default_print = true;

  while ((c = getopt_long (argc, argv, "hip:v", long_options, NULL)) != -1) {
    switch (c) {
    case 'h':
      usage (0);
    case 'v':
      version ();
    case 'i':
      options.indent = "  ";
      break;
    case 'p':
      decode_print_options (optarg);
      default_print = false;
      break;
    default:
      usage (1);
    }
  }

  if (default_print)
    default_options ();
}

static const char *
dir_to_string (a_dir dir)
{
  switch (dir) {
  case D_UP:
    return "up";
  case D_RIGHT:
    return "right";
  case D_DOWN:
    return "down";
  case D_LEFT:
    return "left";
  }
  exit_status = 4;
  return "error";
}

static void
print_header (const a_level *lvl)
{
  printf (_("%sheight:\t%d tiles\t(%d squares)\n"),
	  options.indent, lvl->tile_height, lvl->square_height);
  printf (_("%swidth:\t%d tiles\t(%d squares)\n"),
	  options.indent, lvl->tile_width, lvl->square_width);
  if (lvl->tile_height_wrap == DONT_WRAP)
    printf (_("%sY-wrap:\tno\n"), options.indent);
  else
    printf (_("%sY-wrap:\t%d tiles\t(%d squares)\n"),
	    options.indent, lvl->tile_height_wrap, lvl->square_height_wrap);
  if (lvl->tile_width_wrap == DONT_WRAP)
    printf (_("%sX-wrap:\tno\n"), options.indent);
  else
    printf (_("%sX-wrap:\t%d tiles\t(%d squares)\n"),
	    options.indent, lvl->tile_width_wrap, lvl->square_width_wrap);
  printf (_("%ssound track alias:\t%s\n"),
	  options.indent, lvl_sound_track (lvl));
  printf (_("%stile map basename:\t%s\n"),
	  options.indent, lvl_tile_sprite_map_basename (lvl));
  printf (_("%sstarting squares and directions (y x dir):\n"), options.indent);
  {
    int i;
    for (i = 0; i < 4; ++i) {
      a_square_corrd_pair co;
      a_dir di;
      lvl_start_position (lvl, i, &co, &di);
      printf ("%s  #%d: %2d %2d %s\n", options.indent, i + 1,
	      co.y, co.x, dir_to_string (di));
    }
  }
}

static char
type_to_char (a_dir dir)
{
  switch (dir) {
  case T_NONE:
    return ' ';
  case T_STOP:
    return 'h';
  case T_SPEED:
    return 's';
  case T_TUNNEL:
    return 't';
  case T_BOOM:
    return 'b';
  case T_ANIM:
    return 'a';
  case T_ICE:
    return 'i';
  case T_DUST:
    return 'd';
  case T_OUTWAY:
    return 'X';
  default:
    exit_status = 4;
    return '?';
  }
}

const char *type_names[T_MAXTYPE] = {
  N_("none"),
  N_("stop"),
  N_("speed"),
  N_("tunnel"),
  N_("boom"),
  N_("anim"),
  N_("ice"),
  N_("dust"),
  N_("outway")
};

static void
print_type_keys (void)
{
  int i;
  const int ncols = 3;

  for (i = 0; i < T_MAXTYPE; ++i) {
    if (i % ncols == 0)
      printf ("%s", options.indent);
    printf ("'%c' %-20s", type_to_char (i), _(type_names[i]));
    if (i % ncols == ncols - 1)
      puts ("");
  }
  if (i % ncols != 0)
    puts ("");
}

static void
print_square_types (const a_level *lvl)
{
  a_square_coord y, x;
  a_square_index idx;
  for (y = 0, idx = 0; y < lvl->square_height; ++y) {
    printf ("%s|", options.indent);
    for (x = 0; x < lvl->square_width; ++x, ++idx)
      putchar (type_to_char (lvl->square_type[idx]));
    printf ("|%2u\n", y);
  }
}

static void
print_dir_mask (a_dir_mask dm)
{
  int nw = 0;

  if (dm & DM_UP)
    ++nw;
  if (dm & DM_RIGHT)
    ++nw;
  if (dm & DM_DOWN)
    ++nw;
  if (dm & DM_LEFT)
    ++nw;

  if (nw)
    printf ("%d", nw);
  else
    putchar (' ');
}

static void
print_square_walls (const a_level *lvl)
{
  a_square_coord y, x;
  a_square_index idx;
  for (y = 0, idx = 0; y < lvl->square_height; ++y) {
    printf ("%s|", options.indent);
    for (x = 0; x < lvl->square_width; ++x, ++idx)
      print_dir_mask (lvl->square_walls_out[idx]);
    printf ("|%2u\n", y);
  }
}

static char
dir_to_char (a_dir d)
{
  switch (d) {
  case D_UP:
    return '^';
  case D_RIGHT:
    return '>';
  case D_DOWN:
    return '.';
  case D_LEFT:
    return '<';
  default:
    return 'X';
  }
}

static void
print_square_directions (const a_level *lvl)
{
  a_square_coord y, x;
  a_square_index idx;
  for (y = 0, idx = 0; y < lvl->square_height; ++y) {
    printf ("%s|", options.indent);
    for (x = 0; x < lvl->square_width; ++x, ++idx)
      putchar (dir_to_char (lvl->square_direction[idx]));
    printf ("|%2u\n", y);
  }
}

static void
print_tunnels (const a_level *lvl)
{
  a_square_coord y, x;
  a_square_index idx;
  int tunbr = 0;		/* Number of tunnels.  */
  int curtun = 0;		/* Curent tunnel index.  */
  a_square_index *outputs;

  for (idx = 0; idx < lvl->square_count; ++idx)
    if (lvl->square_type[idx] == T_TUNNEL)
      ++tunbr;

  if (tunbr == 0) {
    printf ("%sNo tunnels.\n", options.indent);
    return;
  }

  XMALLOC_ARRAY (outputs, tunbr);

  for (curtun = 0, idx = 0; idx < lvl->square_count; ++idx)
    if (lvl->square_type[idx] == T_TUNNEL)
      outputs[curtun++] = idx;

  printf (_("%sTunnels:\n"), options.indent);
  /* TRANS: This is the header of an array (output by heroeslvl -p@),
     so the position of these words is important.

     SRCIDX is a short for 'source index' (entrance of a tunnel) and
     DESTIDX means 'destination index' (output of the tunnel).  */
  printf (_("%s  NBR  SRCIDX   Y   X  DIR      DESTIDX\n"), options.indent);

  for (curtun = 0, y = 0, idx = 0; y < lvl->square_height; ++y)
    for (x = 0; x < lvl->square_width; ++x, ++idx)
      if (lvl->square_type[idx] == T_TUNNEL) {
	a_dir d = lvl->square_direction[idx];
	a_square_index outidx = lvl->square_move[d][idx];

	/* Search the number of the output tunnel.  */
	int i;
	for (i = 0; i < tunbr; ++i)
	  if (outputs[i] == outidx)
	    break;

	printf ("%s  #%-2d  %6d  %3d %3d %-8s %7d",
		options.indent, ++curtun, idx, y, x,
		dir_to_string (d), lvl->square_move[d][idx]);
	if (i < tunbr)
	  printf ("\t#%d", i + 1);
	puts ("");
      }

  free (outputs);
}

static const char *
anim_kind_to_str (an_anim_kind k)
{
  switch (k) {
  case A_NONE:
    /* TRANS: `still' is used to describe tiles which are not animated.  */
    return _("still");
  case A_LOOP:
    /* TRANS: `loop' is for animated tiles where frames are displayed
       from 1 to n, and then again from 1 to n, etc.  */
    return _("loop");
  case A_PINGPONG:
    /* TRANS: `pingpong' is for tiles animated in loop where frame
       displayed from 1 to n, then from n to 1, and then again from 1
       to n, etc.  */
    return _("pingpong");
  }
  assert (0);
}

static void
print_tile_details (a_level *lvl)
{
  a_tile_index i;

  /* TRANS: This is the header of an array (output by heroeslvl -pi) so
     the position of these word is important.

     SPRITE is the address of the sprite used to render the tile.  OVERLAY
     is the address of a sprite that should be displayed on top
     of the players (e.g. trees).  ANIM-TYPE is the kind of animation used
     for the tile, FRM the number of frame to display, DEL the delay
     between each frame.  */
  printf (_("\
%sTILE  Y  X    TYPE      SPRITE    OVERLAY  ANIM-TYPE FRM DEL\n"),
	  options.indent);
  for (i = 0; i < lvl->tile_count; ++i) {
    unsigned int o, c, d;
    an_anim_kind k;

    printf ("%s%4u %2u %2u %-10s 0x%08x", options.indent, i,
	    TILE_INDEX_TO_COORD_Y (lvl, i),
	    TILE_INDEX_TO_COORD_X (lvl, i),
	    type_names[lvl_tile_type (lvl, i)],
	    lvl_tile_sprite_offset (lvl, i));

    o = lvl_tile_sprite_overlay_offset (lvl, i);
    if (o)
      printf (" 0x%08x", o);
    else
      printf ("           ");

    lvl_animation_info (lvl, i, &c, &d, &k);
    printf (" %-10s", anim_kind_to_str (k));
    if (k)
      printf (" %2u %2u", c, d);
    else
      printf ("      ");

    puts ("");
  }
}

static void
process (const char *filename)
{
  int err;
  a_level lvl;
  bool load_full;

  /* Sometime we don't need to load the full level, only the header is
     needed to print the requested information.  */
  if (options.print_types
      | options.print_walls
      | options.print_directions
      | options.print_tunnels
      | options.print_tile_details)
    load_full = true;
  else
    load_full = false;

  err = lvl_load_file (filename, &lvl, load_full);
  if (err) {
    error (0, err, _("cannot load %s"), filename);
    exit_status = 3;
  } else {
    if (options.print_filename)
      printf (_("File:\t%s\n"), filename);
    if (options.print_header)
      print_header (&lvl);
    if (options.print_type_keys)
      print_type_keys ();
    if (options.print_types)
      print_square_types (&lvl);
    if (options.print_walls)
      print_square_walls (&lvl);
    if (options.print_directions)
      print_square_directions (&lvl);
    if (options.print_tunnels)
      print_tunnels (&lvl);
    if (options.print_tile_details)
      print_tile_details (&lvl);
  }
  lvl_free (&lvl);
}

int
main (int argc, char *argv[])
{
  mtrace (); /* GNU libc's malloc debugging facility */
  program_name = argv[0];

  setlocale (LC_ALL, "");
  /* FIXME: Use $(locale-dir) as in Heroes.  */
  bindtextdomain (PACKAGE, PREFIX "/" FORWARD_RELATIVE_LOCALEDIR);
  textdomain (PACKAGE);

  decode_switches (argc, argv);
  if (optind == argc) {
    error (0, 0, _("Missing filename."));
    usage (1);
  }

  /* Process all files.  */
  for (; optind < argc; ++optind) {
    process (argv[optind]);
  }

  return exit_status;
}
