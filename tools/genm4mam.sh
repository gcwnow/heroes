#! /bin/sh
# Copyright 2000  Alexandre Duret-Lutz <duret_g@epita.fr>
#
# This file is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

# This file is called `genm4mam.sh' because it used to
# generate `m4/Makefile.am'.  But it now generate `m4/Makefile.inc'.

set -e

mam='m4/Makefile.tmp'
mamreal='m4/Makefile.inc'

rm -f "$mam"
echo '## This file was generated by tools/genm4mam.sh' > "$mam"
echo "## on `date`" >> "$mam"
echo "EXTRA_DIST += m4/README \\" >> "$mam"
echo m4/*.m4 | sed 's/ / \\@/g' | tr '@' '\n' >> "$mam"

rm -f $mamreal
mv $mam $mamreal
