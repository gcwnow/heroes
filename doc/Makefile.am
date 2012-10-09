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


info_TEXINFOS = heroes.texi
EXTRA_DIST = doc.scm heroes.html

if MAKE_HTML
hdir = @htmldir@
h_DATA = heroes.html
endif

SUFFIXES = .html

.texi.html:
	@cd $(srcdir) && rm -f $@ $@-[0-9] $@-[0-9][0-9]
	cd $(srcdir) && $(MAKEINFO) --html --no-split `echo $< | sed 's,.*/,,'`

AUTOGEN_OPTS = -L $(top_srcdir)/src -S $(srcdir)/doc.scm

heroes.texi: $(top_srcdir)/src/debugchn.def $(top_srcdir)/src/debugchn.tpl $(srcdir)/doc.scm $(top_srcdir)/src/people.def $(top_srcdir)/src/people.tpl
	SOURCE_TEXI=heroes.texi $(AUTOGEN) $(AUTOGEN_OPTS) -o 'texi=tmp1.texi' $(top_srcdir)/src/debugchn.def
	SOURCE_TEXI=tmp1.texi $(AUTOGEN) $(AUTOGEN_OPTS) -o 'texi=tmp2.texi' --writable $(top_srcdir)/src/people.def
	mv tmp2.texi heroes.texi
