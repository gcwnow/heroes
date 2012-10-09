#! /bin/sh
# Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>
#
# This file is part of Heroes.
#
# Heroes is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your
# option) any later version.
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
# ======================================================================
#
# 1) I use this to cross compile Heroes for mingw32 and build a package.
# 2) This script assumes too much.
#

# system to compile for
BUILDNAME="i586-mingw32msvc"

# path where the package should unpack
PREFIX="/heroes"

# prefix for cross-compiled SDL
SDLPREFIX="/usr/local/$BUILDNAME"

# configuration options
CONFIGURE_OPTS="--host=$BUILDNAME --enable-html-doc=$PREFIX/doc \
--prefix=$PREFIX --with-sdl=$SDLPREFIX"

# top level directory
TOPSRC_DIR=`cd ../..; pwd`

# configure script location
CONFIGURE="$TOPSRC_DIR/configure"

# temporary paths used to build and install the file before
# they are packaged package
BUILDDIR="$PWD/=build"
DESTDIR="$PWD/=inst"

cross_conf ()
{
  mkdir -p $BUILDDIR
  (cd $BUILDDIR && $CONFIGURE $CONFIGURE_OPTS)
}

cross_build ()
{
  (cd $BUILDDIR && make)
}

cross_install ()
{
  mkdir -p $DESTDIR
  # Using install-strip like this in a cross-compiling setup will
  # only work if Heroes has been packaged with Automake 1.5d or
  # later.
  (cd $BUILDDIR && make DESTDIR="$DESTDIR" install-strip)
  cp dest-readme-lib $DESTDIR/$PREFIX/doc/README-LIB
  cp dest-readme-txt $DESTDIR/$PREFIX/README.TXT
  cp dest-run-bat $DESTDIR/$PREFIX/run.bat
  for f in ABOUT-NLS \
	   BUGS \
	   ChangeLog \
	   ChangeLog.00 \
	   COPYING \
	   NEWS \
	   THANKS \
	   TODO ; do
    test -f $TOPSRC_DIR/$f && cp -f $TOPSRC_DIR/$f $DESTDIR/$PREFIX/doc/
  done
  (cd $DESTDIR/$PREFIX/doc && zip -9 -m ChangeLog.zip ChangeLog ChangeLog.00)
  rm -rf $DESTDIR/$PREFIX/info
  rm -rf $DESTDIR/$PREFIX/man
  rm -rf $DESTDIR/$PREFIX/lib # charset.alias is hardcoded under Windows.
}

cross_pack ()
{
  # package name and version
  eval `cd $TOPSRC_DIR && autoconf --trace 'AC_INIT:VERSION=$2;PACKAGE=$1'`
  # zip file to create
  ZIPFILE="`pwd`/$PACKAGE-$VERSION-XXX.mingw32.zip"

  # use .$PREFIX instead of ./$PREFIX because $PREFIX already has a
  # leading slash and zip doesn't handle `.//' as expected (it just
  # strip the leading `./' part).
  cd $DESTDIR && zip -9 -r $ZIPFILE .$PREFIX
}

cross_clean ()
{
  rm -rf $BUILDDIR $DESTDIR
}

dispatch ()
{
  case $1 in
    conf*)
      cross_conf
      ;;
    make|build*)
      cross_build
      ;;
    inst*)
      cross_install
      ;;
    pack*|pkg)
      cross_pack
      ;;
    *clean*)
      cross_clean
      ;;
    all)
      dispatch conf &&
      dispatch make &&
      dispatch inst &&
      dispatch pkg  &&
      dispatch clean
      ;;
    *)
      echo 'config  -- configure the source'
      echo 'make    -- build the source'
      echo 'install -- install in temporary directory'
      echo 'pack    -- package the installed files'
      echo 'clean   -- clean up everything but the packaged file'
      echo 'all     -- run all the above in order'
      ;;
  esac
}

test $# -lt 1 && dispatch help

for i; do
  dispatch $i
done
