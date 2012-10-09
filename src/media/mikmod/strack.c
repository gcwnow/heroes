/*------------------------------------------------------------------------.
| Copyright 1997, 1998, 2000  Alexandre Duret-Lutz <duret_g@epita.fr>     |
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
#include <mikmod.h>
#include <pthread.h>
#include "strack.h"
#include "prefs.h"
#include "argv.h"
#include "musicfiles.h"
#include "debugmsg.h"
#include "errors.h"
#include "cast.h"

static char sound_initialized = 0;
static char sound_track_loaded = 0;
static char sound_track_playing = 0;

static MODULE *module = 0;
static pthread_t polling_thread;
static pthread_mutex_t playing;	/* this mutex is used to tell the polling
				   thread that it must continue polling ... */
static int nth_driver = 0;
static char *driver_options = 0;

void
adjust_volume (void)
{
  if (opt.music)
    md_musicvolume = (13 - opt.music_volume) * 128 / 13;
  else
    md_musicvolume = 0;
  dmsg (D_SOUND_TRACK, "set volume to %d/128", md_musicvolume);
  /*
     This doesn't want to work.  I'm changing the volume of each sample
     as a work around, see event_sfX() in sfx.c.

  if (opt.sfx)
    md_sndfxvolume = (13 - opt.sfx_volume) * 128 / 13;
  else
    md_sndfxvolume = 0;
  */
}

void
halve_volume (void)
{
  md_musicvolume /= 2;
  md_sndfxvolume /= 2;
  dmsg (D_SOUND_TRACK|D_SOUND_EFFECT,"halve volume");
}

int
init_sound_engine (void)
{
  if (nosound) {
    nosfx = 1;
    return 0;
  }

  dmsg (D_SYSTEM | D_SOUND_TRACK, "initialize libMikMod");

  /* register all the drivers */
  MikMod_RegisterAllDrivers ();
  dmsg (D_SOUND_TRACK, "libMikMod driver registered");

  /* register the all module loader
     (the user can use something else than .xm) */
  MikMod_RegisterAllLoaders ();
  dmsg (D_SOUND_TRACK, "libMikMod loader registered");

  /* initialize the library */
  md_device = nth_driver;
  md_mode |= DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX;
  if (mono)
    md_mode &= ~DMODE_STEREO;
  if (bits8)
    md_mode &= ~DMODE_16BITS;
  if (hqmix)
    md_mode |= DMODE_HQMIXER;
  dmsg (D_SOUND_TRACK, "Opening audio device #%d, with %dbits, %s%s",
	nth_driver, (md_mode & DMODE_16BITS)?16:8,
	(md_mode & DMODE_STEREO)?"stereo":"mono",
	(md_mode & DMODE_HQMIXER)?"":", high quality mixer");
  if (driver_options)
    dmsg (D_SOUND_TRACK, "MikMod user options: %s", driver_options);
  if (MikMod_Init (driver_options ? driver_options : const_cast_string (""))) {
    wmsg (_("Could not initialize sound, reason: %s\n"
	    "Disabling sound output (use -S to suppress this message)."),
	  MikMod_strerror (MikMod_errno));
    nosfx = nosound = 1;
    MikMod_Exit ();
    return 0;
  }

  dmsg (D_SOUND_TRACK,"initialize MikMod thread");
  if (MikMod_InitThreads () != 1) {
    wmsg (_("Could not initialize sound, reason: "
	    "LibMikMod is not thread safe.\n"
	    "Disabling sound output (use -S to suppress this message)."));
    nosfx = nosound = 1;
    MikMod_Exit ();
    return 0;
  }

  sound_initialized = 1;

  pthread_mutex_init (&playing, 0);
  adjust_volume ();

  return 0;
}

void
uninit_sound_engine (void)
{
  if (sound_initialized) {
    unload_soundtrack ();
    MikMod_Exit ();
    dmsg (D_SOUND_TRACK, "libMikMod exited");
    sound_initialized = 0;
  }
}

void
load_soundtrack (char *ptr)
{
  if (nosound)
    return;
  dmsg (D_FILE|D_SOUND_TRACK,"loading sound track: %s", ptr);
  module = Player_Load (ptr, 16, 0);
  if (!module) {
    wmsg (_("Could not load %s, reason: %s"), ptr,
	  MikMod_strerror (MikMod_errno));
  } else {
    sound_track_loaded = 1;
  }
}

void
unload_soundtrack (void)
{
  if (nosound)
    return;
  if (sound_track_playing) {
    dmsg (D_SOUND_TRACK, "joining playing thread");
    pthread_mutex_unlock (&playing);
    pthread_join (polling_thread, 0);
    Player_Stop ();
    sound_track_playing = 0;
  }
  if (sound_track_loaded) {
    dmsg (D_SOUND_TRACK, "unloading sound track");
    Player_Free (module);
    module = 0;
    soundtrack_title = 0;
    soundtrack_author = 0;
    FREE_SPRITE0 (soundtrack_title_sprite);
    FREE_SPRITE0 (soundtrack_author_sprite);
    sound_track_loaded = 0;
  }
}

static void *
update_thread (void *arg ATTRIBUTE_UNUSED)
{
  while (pthread_mutex_trylock (&playing) == EBUSY) {
    MikMod_Update ();
    usleep (10000);
  }
  pthread_mutex_unlock (&playing);
  return 0;
}

void
play_soundtrack (void)
{
  if (nosound)
    return;
  if (!sound_track_loaded)
    return;
  dmsg (D_SOUND_TRACK, "launching sound track playing thread");
  pthread_mutex_lock (&playing);
  MikMod_SetNumVoices (-1, 6);
  /* MikMod_EnableOutput (); */
  Player_Start (module);
  pthread_create (&polling_thread, 0, update_thread, 0);
  sound_track_playing = 1;
}

void
print_drivers_list (void)
{
  char* info;
  long engineversion = MikMod_GetVersion();

  MikMod_RegisterAllDrivers ();
  printf (_("LibMikMod version %ld.%ld.%ld\n"),
	  (engineversion >> 16) & 255,(engineversion >> 8) & 255, engineversion & 255);
  info = MikMod_InfoDriver();
  printf(_("\nAvailable drivers:\n%s\n"), info);
  free (info);
}

/* This function is adapted from from Mikmod 3.1.6 */
static void
get_int (char *arg, int *value, int min, int max, const char* argv0)
{
  char *end = NULL;
  int t = min - 1;

  if (arg)
    t = strtol (arg, &end, 10);
  if (end && (!*end) && (t >= min) && (t <= max))
    *value = t;
  else
    wmsg (_("Argument '%s' out of bounds, must be between %d and %d.\n"
	    "Use '%s --help' for more information."),
	  arg?arg:"(not given)", min, max, argv0);
}

void
decode_sound_options (char* option_string, const char* argv0)
{
  /* This is adapted from Mikmod 3.1.6 */
  if (strlen (option_string) > 2) {
    char* opts = strchr (option_string, ',');
    if (opts) {
      *opts=0;

      /* numeric driver specification ? */
      if (opts - option_string <= 2)
	get_int (optarg, &nth_driver, 0, 99, argv0);
      else
	nth_driver = MikMod_DriverFromAlias(option_string);
      if (driver_options)
	free (driver_options);
      driver_options = xstrdup (opts+1);
    } else
      nth_driver = MikMod_DriverFromAlias (option_string);
  } else
    get_int(option_string, &nth_driver, 0, 99, argv0);
}
