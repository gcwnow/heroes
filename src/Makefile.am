# Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>
#
# This file is part of Heroes.
#
# Heroes is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License version 2 as published by
# the Free Software Foundation.
#
# Heroes is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#

# The following definitions are not generated automatically by
# Automake because these variable are created by generic m4 macros.
# When automake is changed to use autoconf --trace, it will
# declare them automatically.
GGI_CFLAGS = @GGI_CFLAGS@
GII_CFLAGS = @GII_CFLAGS@
LIBMIKMOD_CFLAGS = @LIBMIKMOD_CFLAGS@
SDL_CFLAGS = @SDL_CFLAGS@
SDL_MIXER_CFLAGS = @SDL_MIXER_CFLAGS@
ALLEGRO_CFLAGS = @ALLEGRO_CFLAGS@
# ---

AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = heroes heroeslvl

INCLUDES = -I$(top_srcdir)/lib -I../lib -I../intl -I$(srcdir)/lvl -I$(srcdir)/media -I$(srcdir)/sys -I.. -I.

EXTRA_DIST = rsc_files.gperf debughash.gperf

AM_CFLAGS = $(MORE_WARNINGS_CFLAGS)

heroeslvl_LDADD = lvl/libhlvl.a ../lib/libheroes.a $(INTLLIBS)
heroeslvl_SOURCES = heroeslvl.c

heroes_LDADD = lvl/libhlvl.a media/libhmcommon.a ../lib/libheroes.a \
	       $(INTLLIBS) $(LIBALT_LOCAL_XLDADD) $(LIBALT_XLDADD)
heroes_LDFLAGS = $(LIBALT_XLDFLAGS)

## Depend upon Makefile because the value of LIBALT_LOCAL_LDADD might
## change from one instantiation to the other (in which case heroes
## needs to be re-linked).
heroes_DEPENDENCIES = lvl/libhlvl.a media/libhmcommon.a ../lib/libheroes.a \
	       $(LIBALT_LOCAL_XLDADD) Makefile
heroes_SOURCES = \
argv.c \
argv.h \
bonus.c \
bonus.h \
bytesex.h \
camera.c \
camera.h \
const.c \
const.h \
debugchn.def \
debugchn.h \
debugchn.tpl \
debughash.c \
debughash.h \
debugmsg.c \
debugmsg.h \
endscroll.c \
endscroll.h \
errors.c \
errors.h \
explosions.c \
explosions.h \
extras.c \
extras.h \
fader.c \
fader.h \
fastmem.h \
font.h \
fontdata.c \
fontdata.h \
gameid.c \
gameid.h \
generic_list.h \
hedlite.c \
hedlite.h \
helptext.c \
helptext.h \
hendian.c \
hendian.h \
heroes.c \
heroes.h \
intro.c \
intro.h \
items.c \
items.h \
levellst.c \
levellst.h \
locales.c \
locales.h \
menus.c \
menus.h \
misc.c \
misc.h \
musicfiles.c \
musicfiles.h \
parafmt.c \
parafmt.h \
pcx.c \
pcx.h \
pendulum.c \
pendulum.h \
people.c \
people.def \
people.h \
people.tpl \
persona.c \
persona.h \
pixelize.c \
pixelize.h \
prefs.c \
prefs.def \
prefs.h \
prefs.tpl \
readmake.c \
readmake.h \
relocate.c \
relocate.h \
render.c \
render.h \
renderdata.c \
renderdata.h \
rsc_files.c \
rsc_files.h \
rsc_files_hash.c \
rsc_files_hash.h \
savegame.c \
savegame.h \
scores.c \
scores.h \
scrtools.c \
scrtools.h \
sprglauto.c \
sprglauto.h \
sprglenz.c \
sprglenz.h \
sprite.c \
sprite.h \
spropaque.c \
spropaque.h \
sprprog.c \
sprprog.h \
sprprogwav.c \
sprprogwav.h \
sprrle.c \
sprrle.h \
sprshade.c \
sprshade.h \
sprtext.c \
sprtext.h \
sprunish.c \
sprunish.h \
sprzcol.c \
sprzcol.h \
structs.h \
system.h \
timer.c \
timer.h \
userconf.c \
userconf.h \
userdir.c \
userdir.h \
vars.h \
vars.c \
visuals.c \
visuals.h

BUILT_SOURCES = \
debugchn.h \
debughash.gperf \
prefs.c \
prefs.h \
rsc_files_hash.c

if HAVE_GPERF
rsc_files_hash.c: rsc_files.gperf
	gperf -t -L ANSI-C -G -N in_rsc_set -W rsc_set \
	      -k 1,3,7,$$ $(srcdir)/rsc_files.gperf | \
	sed -e 's/{""}/{"",0,0,0,false}/g' > rsc_files_hash.c

debughash.c: debughash.gperf
	gperf -t -L ANSI-C -G -N in_channel_set -W channel_set \
	      -k 1,3,$$ $(srcdir)/debughash.gperf | \
	sed -e 's/{""}/{"",0,0}/g' > debughash.c
endif

debughash.gperf: debugchn.def debugchn.tpl
	$(AUTOGEN) -o 'gperf=debughash.gperf' debugchn.def

debugchn.h: debugchn.def debugchn.tpl
	$(AUTOGEN) -o h debugchn.def

prefs.h: prefs.def prefs.tpl
	$(AUTOGEN) -o h prefs.def

prefs.c: prefs.def prefs.tpl
	$(AUTOGEN) -o c prefs.def

people.c: people.def people.tpl
	$(AUTOGEN) -o c people.def

MAINTAINERCLEANFILES = \
debugchn.h \
debughash.c \
debughash.gperf \
people.c \
prefs.c \
prefs.h \
rsc_files_hash.c

DISTCLEANFILES =

EXTRA_LIBRARIES =
CLEANFILES = $(EXTRA_LIBRARIES)

EXTRA_PROGRAMS = parafmt-check
parafmt_check_SOURCES = parafmt.c
parafmt_check_CFLAGS = $(MORE_WARNINGS_CFLAGS) -DTEST
parafmt_check_LDADD = ../lib/libheroes.a

EXTRA_DIST += etc/Makefile.inc
include etc/Makefile.inc
EXTRA_DIST += lvl/Makefile.inc
include lvl/Makefile.inc
EXTRA_DIST += media/Makefile.inc
include media/Makefile.inc
EXTRA_DIST += sys/Makefile.inc
include sys/Makefile.inc
