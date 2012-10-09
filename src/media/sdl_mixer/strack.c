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
#include <SDL_mixer.h>
#include "strack.h"
#include "prefs.h"
#include "argv.h"
#include "musicfiles.h"
#include "debugmsg.h"
#include "errors.h"

static char sound_initialized = 0;
static char sound_track_loaded = 0;
static char sound_track_playing = 0;

static Mix_Music *module = NULL;

static int audio_rate = 0;
static Uint16 audio_format;
static int audio_channels;
static int audio_buffers = 0;

void
adjust_volume (void)
{
  if (opt.music) {
    Mix_VolumeMusic ((13 - opt.music_volume) * MIX_MAX_VOLUME / 13);
    dmsg (D_SOUND_TRACK, "set volume to %d/%d",
	  (13 - opt.music_volume) * MIX_MAX_VOLUME / 13, MIX_MAX_VOLUME);
  } else {
    Mix_VolumeMusic (0);
    dmsg (D_SOUND_TRACK, "set volume to 0/%d", MIX_MAX_VOLUME);
  }
}

void
halve_volume (void)
{
  if (opt.music) {
    Mix_VolumeMusic ((13 - opt.music_volume) * MIX_MAX_VOLUME / 13 / 2);
    dmsg (D_SOUND_TRACK, "set volume to %d/%d",
	  (13 - opt.music_volume) * MIX_MAX_VOLUME / 13 / 2, MIX_MAX_VOLUME);
  } else {
    Mix_VolumeMusic (0);
    dmsg (D_SOUND_TRACK, "set volume to 0/%d", MIX_MAX_VOLUME);
  }
}

extern void init_SDL (void);

int
init_sound_engine (void)
{
  if (nosound) {
    nosfx = 1;
    return 0;
  }

  if (!audio_rate)
    audio_rate = (hqmix ? 44100 : 22050);
  audio_format = (bits8 ? AUDIO_S8 : AUDIO_S16);
  audio_channels = (mono ? 1 : 2);
  if (!audio_buffers)
    /* Use small values for audio buffer to reduce the duration between
       the moment where a sample is mixed and the moment where it is heard. */
    audio_buffers = (hqmix ? 2048 : 1024);

  init_SDL ();
  /* Open the audio device */
  dmsg (D_SOUND_TRACK,
	"opening audio at %d Hz %d bit %s, %d bytes audio buffer",
	audio_rate,
	(audio_format&0xFF),
	(audio_channels > 1) ? "stereo" : "mono",
	audio_buffers);
  if (Mix_OpenAudio (audio_rate, audio_format, audio_channels, audio_buffers)
      < 0) {
    wmsg (_("Couldn't open audio: %s\n"
	    "Disabling sound output (use -S to suppress this message)."),
	  SDL_GetError());
    nosfx = nosound = 1;
  } else {
    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
    dmsg (D_SOUND_TRACK,
	  "opened audio at %d Hz %d bit %s, %d bytes audio buffer",
	  audio_rate,
	  (audio_format&0xFF),
	  (audio_channels > 1) ? "stereo" : "mono",
	  audio_buffers);

    sound_initialized = 1;
  }
  adjust_volume ();

  return 0;
}

void
uninit_sound_engine (void)
{
  if (sound_initialized) {
    unload_soundtrack ();
    Mix_CloseAudio ();
    dmsg (D_SOUND_TRACK, "closed audio");
  }
}

void
load_soundtrack (char *ptr)
{
  if (nosound)
    return;
  dmsg (D_SOUND_TRACK|D_FILE,"loading sound-track: %s", ptr);
  module = Mix_LoadMUS(ptr);
  if (!module) {
    wmsg (_("Could not load %s, reason: %s"), ptr, SDL_GetError ());
  } else
    sound_track_loaded = 1;
}

void
unload_soundtrack (void)
{
  if (nosound)
    return;
  if (sound_track_playing) {
    dmsg (D_SOUND_TRACK, "halt sound track playing");
    Mix_HaltMusic ();
    sound_track_playing = 0;
  }
  if (sound_track_loaded) {
    dmsg (D_SOUND_TRACK, "unload sound track");
    Mix_FreeMusic (module);
    module = NULL;
    sound_track_loaded = 0;
  }
}

void
play_soundtrack (void)
{
  if (nosound)
    return;
  if (sound_track_loaded) {
    dmsg (D_SOUND_TRACK, "start playing sound track");
    Mix_PlayMusic (module, -1);
    sound_track_playing = 1;
  }
}

void
print_drivers_list (void)
{
  wmsg (_("Heroes has been compiled with SDL_mixer,"
	  " there is no driver list available."));
}

void
decode_sound_options (char *arg, const char *argv0)
{
  if (arg) {
    char *buf = xstrdup (arg);
    optarg = strtok (buf, " \t:=,;");
    while (optarg) {
      if (!strcasecmp (optarg, "freq")) {
	optarg = strtok (0, " \t:=,;");
	if (optarg)
	  audio_rate = atol (optarg);
	else
	  wmsg (_("%s: missing parameter for 'freq'"), argv0);
      } else if (!strcasecmp (optarg, "buffers")) {
	optarg = strtok (0, " \t:=,;");
	if (optarg)
	  audio_buffers = atol (optarg);
	else
	  wmsg (_("%s: missing parameter for `buffers'"), argv0);
      } else
	wmsg (_("%s: recognized sound options "
		"are freq=nnn and buffers=nnn"), argv0);
      optarg = strtok (0, " \t:=,;");
    }
    free (buf);
  }
}
