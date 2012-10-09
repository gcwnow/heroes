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

AUTOMAKE_OPTIONS = 1.4b gnits dist-bzip2
ACLOCAL_AMFLAGS = -I m4

# Some files, such as AUTHORS or THANKS, usually automatically added
# by automake are listed here because they don't exist on the CVS
# but are generated from a rule in this Makefile (and automake
# is not smart enough to handle this :o)).
EXTRA_DIST = ANNOUNCE AUTHORS BUGS ChangeLog.00 ChangeLog.01 \
GNUmakefile HACKING THANKS \
misc/copytile.c \
misc/heroes-1.xpm \
misc/heroes-2.xpm \
misc/heroes-3.xpm \
misc/heroes-4.xpm \
misc/heroes-5.xpm \
misc/heroes-6.xpm \
misc/heroes-7.xpm \
misc/heroes-8.xpm \
misc/01-intl.patch \
misc/05-po.patch

# lib must be built after intl
# man must be built after src
SUBDIRS = intl lib src man po doc tests

README: $(top_srcdir)/doc/heroes.texi $(top_srcdir)/tools/readme.sed
	LC_ALL=C $(MAKEINFO) --no-header -I $(top_srcdir)/doc heroes.texi | \
	sed -n -f $(top_srcdir)/tools/readme.sed > README.tmp
	mv README.tmp README

THANKS: $(top_srcdir)/src/people.def $(top_srcdir)/src/people.tpl
	$(AUTOGEN) -L $(top_srcdir)/src -o 'thanks=THANKS' $(top_srcdir)/src/people.def

AUTHORS: $(top_srcdir)/src/people.def $(top_srcdir)/src/people.tpl
	$(AUTOGEN) -L $(top_srcdir)/src -o 'authors=AUTHORS' $(top_srcdir)/src/people.def

MAINTAINERCLEANFILES = \
ABOUT-NLS \
AUTHORS \
COPYING \
INSTALL \
THANKS \
aclocal.m4 \
configure \
m4/Makefile.am \
po/POTFILES.in \
po/heroes.pot \
src/config.h.in

# additional checks to perform during distcheck.
# this is heavily inspired from Jim Meyring's setup
# in Fileutils & Shellutils & Textutils.

distcheck-hook: \
	man-distcheck \
	my-distcheck-allegro \
	my-distcheck-sdl \
	my-distcheck-ggi \
	my-distcheck-nodisplay

# Use this to make sure we don't run these programs when building
# from a virgin tgz file, below.
null_AM_MAKEFLAGS = \
  ACLOCAL=false \
  AUTOCONF=false \
  AUTOMAKE=false \
  AUTOHEADER=false \
  MAKEINFO=false \
  AUTOGEN=false

t=./=test
# build with libggi and mikmod (VPATH build)
my-distcheck-ggi:
	-rm -rf $(t)
	mkdir $(t)
	cd $(t) \
	  && ../$(distdir)/configure --disable-nls --with-ggi --with-gii \
			             --with-mikmod \
	  && $(MAKE) AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	  && $(MAKE) check
	-rm -rf $(t)

# build with Allegro and mikmod (VPATH build)
my-distcheck-allegro:
	-rm -rf $(t)
	mkdir $(t)
	cd $(t) \
	  && ../$(distdir)/configure --disable-nls --with-allegro \
			             --with-mikmod \
	  && $(MAKE) AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	  && $(MAKE) check
	-rm -rf $(t)

# build without any library (VPATH build)
my-distcheck-nodisplay:
	-rm -rf $(t)
	mkdir $(t)
	cd $(t) \
	  && ../$(distdir)/configure --disable-nls --disable-display \
				     --disable-joystick --disable-sound \
	  && $(MAKE) AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	  && $(MAKE) check
	-rm -rf $(t)

# build with libsdl and sdl_mixer (build in current directory, then
# ensure that no files were modified).
my-distcheck-sdl:
	-rm -rf $(t)
	mkdir $(t)
	GZIP=$(GZIP) $(AMTAR) -C $(t) -zxf $(distdir).tar.gz
	cd $(t)/$(distdir) \
	  && ./configure --disable-nls --with-sdl --with-sdl-mixer \
			 --enable-html-doc \
	  && $(MAKE) AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	  && $(MAKE) dvi \
	  && $(MAKE) check \
	  && $(MAKE) distclean \
	  && rm -rf $(DEPDIR)
# Removing $(DEPDIR) like this is a gross kludge to work around a bug
# in automake.  Remove that line once it's fixed.
	cd $(t) && mv $(distdir) $(distdir).new \
	  && $(AMTAR) -zxf ../$(distdir).tar.gz
	diff -ur $(t)/$(distdir) $(t)/$(distdir).new
	-rm -rf $(t)

man-distcheck:
	cd man && $(MAKE) man-distcheck

# Make sure no sticky bits go into the distribution.
# (Debian policy is to create user directories in g+s,
# we don't want to export this :o)).
dist-hook:
	-chmod -R a-s $(distdir)

EXTRA_DIST += arch/Makefile.inc m4/Makefile.inc tools/Makefile.inc
include arch/Makefile.inc
include m4/Makefile.inc
include tools/Makefile.inc
