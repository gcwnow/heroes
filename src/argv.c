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
#include "const.h"
#include "argv.h"
#include "strack.h"
#include "video.h"
#include "misc.h"
#include "debugmsg.h"
#include "errors.h"
#include "rsc_files_hash.h"
#include "debughash.h"
#include "musicfiles.h"
#include "vars.h"

int cpuon = 1;
bool nosfx = false;
bool joyoff = false;
int devparm = 0;
bool loadulevel = false;
bool directmenu = false;
int reinitsco = 0;
int reinitopt = 0;
int reinitsav = 0;
int x10sav = 0;
int doublefx = 1;
bool swapside = true;
char* level_name;
bool mono = false;
bool bits8 = false;
bool hqmix = false;
int stretch = 1;
bool nosound = false;
bool even_lines = false;
bool showprefs = false;
bool showlevels = false;
a_check check_what = check_nothing;

static void
version (void)
{
  puts ("Heroes " VERSION "\n");
  printf (_("Copyright (C) %d  "
	    "Alexandre Duret-Lutz, Romuald Genevois,\n"
	    "Alexandre Liverneaux and Philippe Meisburger.\n"), 2002);
  puts (_("This is free software; see the source for copying conditions.  "
	  "There is NO\nwarranty; not even for MERCHANTABILITY or FITNESS"
	  " FOR A PARTICULAR PURPOSE."));
}

/* Return true if the game can exit on return, or false if some action
   has been postponed.  */
static bool
list (char *word)
{
  if (!word) {
    puts (_("Use `-lWORD' or `--list=WORD' where WORD can be:"));
    printf ("  debug                 %s\n",
	    _("display all debugging channels"));
    printf ("  levels                %s\n",
	    _("list all level files"));
    printf ("  preferences           %s\n",
	    _("print the preferences settings"));
    printf ("  resources             %s\n",
	    _("print the resources list"));
    printf ("  sound-drivers         %s\n",
	    _("print the sound driver list"));
    printf ("  sound-tracks          %s\n",
	    _("print the sound track list"));
    return true;
  }
  if (!strcasecmp (word,"resources") ||
      !strcasecmp (word,"rsc")) {
    print_rsc_files ();
  } else if (!strcasecmp (word,"levels") ||
	     !strcasecmp (word,"lvl")) {
    showlevels = true;
    return false;
  } else if (!strcasecmp (word,"sound-drivers") ||
	     !strcasecmp (word,"sd")) {
    print_drivers_list ();
  } else if (!strcasecmp (word,"debug") ||
	     !strcasecmp (word,"channels")) {
    print_debug_channels ();
  } else if (!strcasecmp (word,"sound-tracks") ||
	     !strcasecmp (word,"st")) {
    print_sound_track_list ();
  } else if (!strcasecmp (word,"preferences") ||
	     !strcasecmp (word,"prefs")) {
    showprefs = true;
    return false;
  } else if (!strcasecmp (word,"sound-tracks-stat") || /* undocumented */
	     !strcasecmp (word,"sts")) {
    print_sound_track_list_stat ();
  } else if (!strcasecmp (word,"variables") || /* undocumented yet */
	     !strcasecmp (word,"vars")) {
    var_print_all ();
  } else {
    /* Unknown WORD, print usage. */
    list (0);
  }
  return true;
}

static bool
check (char *word)
{
  if (!word) {
    puts ("Use `--check=WORD' where WORD can be:");
    puts ("intro	run the intro");
    puts ("demo		run a demo");
    return true;
  }
  if (!strcasecmp (word, "intro"))
    check_what = check_intro;
  else if (!strcasecmp (word, "demo"))
    check_what = check_demo;
  else
    /* Unknown WORD, print usage.  */
    return check (0);
  return false;
}

static void ATTRIBUTE_NORETURN
print_help (int status)
{
  if (status) {
    fprintf (stderr, _("Try '%s --help' for more information.\n"),
             program_name);
    exit (status);
  }

  printf (_("Usage: %s [OPTIONS]...\n\n"), program_name);
  puts (_("\
Heroes is a game similar to the \"Tron\" and \"Nibbles\" games of yore\n\
but includes many graphical improvements and new game features.\n"));
  puts (_("\
Mandatory arguments to long options are mandatory for short options too.\n"));
  puts (_("\
General options:\n\
  -v, --version               display version number\n\
  -h, --help                  display this help\n\
  -q, --quiet                 don't print warning messages\n\
  -Q, --really-quiet          don't even print error messages\n\
  -v, --verbose=CHANNELS      enable or disable debugging channels\n\
                                (see --list=debug for available channels)\n\
  -l, --list=WORD             show some internal information; WORD can be\n\
                                'debug', 'resources', 'sound-drivers' or\n\
                                'sound-tracks'\n"));
  puts (_("\
Sound options:\n\
  -d, --driver=N[,OPTIONS]    use Nth driver for sound output (0:autodetect)\n\
                                (see --list=sound-drivers for available Ns)\n\
  -S, --no-sound              disable sound\n\
  -X, --no-sfx                disable sound-effects\n\
  -m, --mono                  non-stereo output\n\
  -8, --8bits                 8bits sound output\n\
  -i, --high-quality          high quality mixer\n"));
  puts (_("\
Display options:\n\
  -G, --gfx-options=OPTIONS   pass OPTIONS to the display driver\n\
  -F, --full-screen           full screen mode\n\
  -2, --double                stretch the display twofold\n\
  -3, --triple                stretch the display threefold\n\
  -4, --quadruple             stretch the display fourfold\n\
  -e, --even-lines            display only even-lines\n"));
  /* TRANS: rotozoom is a graphical effect used in the demo of yore
     where the screen rotate and zoom (actually it doesn't zoom in Heroes);
     waves is another effect where the screen is agitated like some
     jelly (this was also called the `mushroom effect' in game like
     Death Rally). */
  puts (_("\
Miscellaneous options:\n\
      --cpu-off               disable computer opponents\n\
      --default-scores        restore default scores file\n\
      --default-options       restore default options file\n\
      --default-saves         restore default saves file\n\
  -s, --swap-sides            swap sides in two player mode\n\
      --no-double-fx          disable combination of rotozoom and waves\n\
  -g, --go                    skip the introduction\n\
  -J, --no-joystick           disable joystick handling\n"));
  puts (_("\
These options can be set in your file ~/.heroes/heroesrc (which is read\n\
before parsing other command line options) using a line like the following:\n\
\n\
  Options: -gs -d3,buffer=11,count=4\n"));
  puts (_("\
Visit http://heroes.sourceforge.net/ for news, documentation, and updates."));
  puts (_("Report bugs to <heroes-bugs@lists.sourceforge.net>."));
  exit (status);
}

const struct option long_options[] = {
  {"8bits",		no_argument,       NULL,	'8'},
  {"check",		required_argument, NULL,	1000},
  {"cpu-off",		no_argument,       &cpuon,	0},
  {"default-options",	no_argument,       &reinitopt,	1},
  {"default-saves",	no_argument,       &reinitsav,	1},
  {"default-scores",	no_argument,       &reinitsco,	1},
  {"devparm",		no_argument,       &devparm,	1},
  {"double",		no_argument,       NULL,	'2'},
  {"driver",		required_argument, NULL,	'd'},
  {"drivers-info",	no_argument,       NULL,	'n'},
  {"even-lines",	no_argument,       NULL,	'e'},
  {"full-screen",	no_argument,	   NULL,	'F'},
  {"gfx-options",	required_argument, NULL,	'G'},
  {"go",		no_argument,       NULL,	'g'},
  {"help",		no_argument,       NULL,	'h'},
  {"high-quality",	no_argument,       NULL,	'i'},
  {"list",		optional_argument, NULL,	'l'},
  {"load",		required_argument, NULL,	'L'},
  {"mono",		no_argument,       NULL,	'm'},
  {"no-double-fx",	no_argument,       &doublefx,	0},
  {"no-joystick",	no_argument,       NULL,	'J'},
  {"no-sfx",		no_argument,       NULL,	'X'},
  {"no-sound",		no_argument,       NULL,	'S'},
  {"quadruple",		no_argument,       NULL,	'4'},
  {"quiet",		no_argument,	   NULL,	'q'},
  {"really-quiet",	no_argument,	   NULL,	'Q'},
  {"swap-sides",	no_argument,       NULL,	's'},
  {"triple",		no_argument,       NULL,	'3'},
  {"verbose",		required_argument, NULL,	'v'},
  {"version",		no_argument,       NULL,	'v'},
  {"x10-saves",		no_argument,       &x10sav,	1},
  {NULL,		0,		   NULL,	0}
};

int
parse_argv (int argc, char **argv, const char *from_file, int from_line)
{
  int c;

  if (from_file) {
    /* Build a fake argv[0], so that the error message from getopt
       looks nice */
    char* pname;
    XMALLOC_ARRAY (pname, (strlen (from_file) + strlen (program_name)
			   + strlen (": :999999") + 1));
    sprintf (pname, "%s: %s:%d", program_name, from_file, from_line);
    argv[0] = pname;
  }

  /* Reset optind so that getopt is reinitialized. */
  optind = 0;

  for (;;) {
    int option_index = 0;

    c = getopt_long (argc, argv, "2348d:eFgG:hiJl::L:mnqQsSv::X",
		     long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c) {
    case 'v':
      if (!optarg) {
	version ();
	return -1;
      } else {
	dmsg_parse_string (optarg);
	break;
      }
    case 'h':
      print_help (0);
    case 's':
      swapside = false;
      break;
    case 'm':
      mono = true;
      break;
    case '8':
      bits8 = true;
      break;
    case 'a':
      hqmix = true;
      break;
    case 'X':
      nosfx = true;
      break;
    case 'l':
      if (list (optarg))
        return -1;
      break;
    case 'L':
      level_name = xstrdup (optarg);
      level_name = strappend (level_name, ".lvl");
      loadulevel = true;
      break;
    case 'g':
      directmenu = true;
      break;
    case 'n':
      wmsg (_("-n is an obsolete option, "
	      "you should use --list=sound-drivers"));
      print_drivers_list ();
      return -1;
    case 'd':
      decode_sound_options (optarg, from_file ? from_file : argv[0]);
      break;
    case 'G':
      set_display_params (optarg);
      break;
    case 'F':
      set_full_screen_mode ();
      break;
    case 'J':
      joyoff = true;
      break;
    case '2':
      stretch = 2;
      break;
    case '3':
      stretch = 3;
      break;
    case '4':
      stretch = 4;
      break;
    case 'e':
      even_lines = true;
      break;
    case 'i':
      hqmix = true;
      break;
    case 'S':
      nosound = true;
      break;
    case '?':
      /* getopt_long already printed an error message. */
      print_help (1);
    case 0:
      break;
    case 'Q':
      disable_emsg = true;
      /* no break: 'Q' imply 'q' */
    case 'q':
      disable_wmsg = true;
      break;
    case 1000:			/* --check */
      if (check (optarg))
        return -1;
      break;
    default:
      abort ();
    }
  }
  if (from_file)
    free (argv[0]);

  return 0;
}
