##                                                        -*- Makefile -*-
## Copyright 2001  Alexandre Duret-Lutz <duret_g@epita.fr>
##
## This file is part of Heroes.
##
## Heroes is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License version 2 as published by
## the Free Software Foundation.
##
## Heroes is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##

## These libraries are tagged as `EXTRA' because we don't want to build
## all of them everytime.  Only a subset of them (determined at
## configure time) will be built.  Practically, the requested libraries
## are simply listed in heroes_DEPENDENCIES.

EXTRA_LIBRARIES += \
  sys/libhstdm.a \
  sys/libhsdlm.a \
  sys/libhallm.a

EXTRA_DIST += \
  sys/README \
  sys/main.h

sys_libhstdm_a_SOURCES = \
  sys/std/main.c

## Use sys/std/main.c too.  Only the CFLAGS differs (they will
## usually include something like -Dmain=SDL_main).
sys_libhsdlm_a_CFLAGS = \
  $(MORE_WARNINGS_CFLAGS) $(SDL_CFLAGS)
sys_libhsdlm_a_SOURCES = \
  sys/std/main.c

# Don't use $(MORE_WARNINGS_CFLAGS) with Allegro.  There are too many
# warnings in the Allegro header files!
sys_libhallm_a_CFLAGS = \
  $(WARNINGS_CFLAGS) $(ALLEGRO_CFLAGS)
sys_libhallm_a_SOURCES = \
  sys/allegro/main.c
