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

# path for config.h and system.h
INCLUDES = -I.. -I$(srcdir) -I../intl -I$(top_srcdir)/src

noinst_LIBRARIES= libheroes.a

# These files are always compiled
libheroes_a_SOURCES= \
  basename.c \
  basename.h \
  cast.c \
  cast.h \
  dirname.c \
  dirname.h \
  error.c \
  error.h \
  filelock.c \
  filelock.h \
  fstrcmp.c \
  fstrcmp.h \
  getshline.c \
  getshline.h \
  gnugetopt.h \
  hash.c \
  hash.h \
  isdir.c \
  isdir.h \
  stripslash.c \
  unlocked-io.h \
  w_dirent.h \
  xalloc.h \
  xmalloc.c \
  xstrdup.c \
  xstrduplwr.c \
  xstrduplwr.h

# Additional files to compile, when functions are missing.
libheroes_a_LIBADD = @LIBOBJS@ @ALLOCA@
