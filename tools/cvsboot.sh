#! /bin/sh
# Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>
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

verb=':'
opts=''

usage ()
{
  echo "Usage: cvsboot.sh [-h] [-v] [-V]"
}

die ()
{
  echo ${1+"$@"}
  exit 1
}

saferun ()
{
  $verb $@
  $@ || die "*** Error '${1+\"$@\"}' exited with bad status."
}

opts="$@"

while test $# -gt 0 ; do
  case "${1}" in
    -h | --h*)
      usage
      exit 0 ;;
    -v | --v* | -V | --V*)
      verb='echo'
      shift ;;
    *)
      usage
      exit 1;;
  esac
done

saferun tools/upgettext.sh $opts
saferun tools/genpotfiles.sh $opts
saferun tools/genm4mam.sh $opts
saferun tools/autogen.sh $opts
