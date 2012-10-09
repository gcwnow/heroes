# Copyright 2000  Alexandre Duret-Lutz <duret_g@epita.fr>
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

man_MANS = heroes.6 heroeslvl.6
EXTRA_DIST = $(man_MANS)
MAINTAINERCLEANFILES = $(man_MANS)

heroes.6 : $(srcdir)/../src/argv.c $(top_srcdir)/configure.ac
	@if test -f $(top_builddir)/src/heroes; then \
	  echo "Updating man page heroes.6"; \
	  $(HELP2MAN) -n 'a game like Nibbles or Tron, just better' \
	    -s6 $(top_builddir)/src/heroes | sed -e 's/ FSF//' > heroes.6; \
	else \
	  echo "WARNING: The man page heroes.6 cannot be updated yet."; \
	  echo "         Retry once the corresponding executable is built."; \
	fi

heroeslvl.6 : $(srcdir)/../src/heroeslvl.c $(top_srcdir)/configure.ac
	@if test -f $(top_builddir)/src/heroeslvl; then \
	  echo "Updating man page heroeslvl.6"; \
	  $(HELP2MAN) -N -n "inspect Heroes' level files" \
	    -s6 $(top_builddir)/src/heroeslvl | sed -e 's/ FSF//' > heroeslvl.6; \
	else \
	  echo "WARNING: The man page heroeslvl.6 cannot be updated yet."; \
	  echo "         Retry once the corresponding executable is built."; \
	fi

# Make sure the manual pages show a correct version before
# releasing a distribution.  This target is called by the top
# level Makefile on `make distcheck'.
man-distcheck: $(man_MANS)
	@for i in $(man_MANS); do \
	  if sed 15q $$i | fgrep -e "$(VERSION)" > /dev/null; then :; else \
	    echo "$$i shows wrong version; not releasing" 1>&2; \
	    exit 1; \
	  fi \
	done
