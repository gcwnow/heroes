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
moreverb=''

usage ()
{
  echo "Usage: autogen.sh [-h] [-v] [-V]"
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

generate ()
{
  test -f configure.ac || test -f configure.in ||
    die "Cannot find configure.in in current directory (`pwd`)."

  if test -f Makefile.am; then
    # use the same aclocal flags as in Makefile.am, if specified
    aclocalflags=`sed -n -e '/ACLOCAL_AMFLAGS/{s/.*=\(.*\)$/\1/
p
}' Makefile.am`
  fi

  # generate aclocal.m4
  saferun aclocal $aclocalflags ${moreverb}

  # generate configure
  saferun autoconf ${moreverb}

  if grep AC_CONFIG_HEADER configure.* >/dev/null 2>/dev/null; then
    # generate src/config.h.in
    saferun autoheader ${moreverb}
  fi

  # generate **/Makefile.in and add mssing files
  # Use --force-missing, or it's possible that we'll use
  # some obsolete files (for instance if the Automake location
  # changer and the symlink still point to the old).
  saferun automake --force-missing --add-missing ${moreverb}
}

while test $# -gt 0 ; do
  case "${1}" in
    -h | --h*)
      usage
      exit 0 ;;
    -v | --v*)
      verb='echo'
      shift ;;
    -V | --V*)
      verb='echo'
      moreverb='--verbose'
      shift ;;
    *)
      usage
      exit 1;;
  esac
done

targets=`find . \( -name 'configure.ac' -o -name 'configure.in' \) -print`
test -n "$targets" || die "Cannot find any configure.in or configure.ac."
set $targets
for i; do
  dir=`expr "x$i" : 'x\(.*\)/[^/]*' \| '.' : '.'`
  test $# -gt 1 && echo "=== configuring '$dir' ==="
  (cd $dir && generate)
done
