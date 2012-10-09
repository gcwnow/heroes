# aclocal.m4 generated automatically by aclocal 1.6 -*- Autoconf -*-

# Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002
# Free Software Foundation, Inc.
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# Do all the work for Automake.                            -*- Autoconf -*-

# This macro actually does too much some checks are only needed if
# your package does certain things.  But this isn't really a big deal.

# Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002
# Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 8

# There are a few dirty hacks below to avoid letting `AC_PROG_CC' be
# written in clear, in which case automake, when reading aclocal.m4,
# will think it sees a *use*, and therefore will trigger all it's
# C support machinery.  Also note that it means that autoscan, seeing
# CC etc. in the Makefile, will ask for an AC_PROG_CC use...


AC_PREREQ([2.52])

# Autoconf 2.50 wants to disallow AM_ names.  We explicitly allow
# the ones we care about.
m4_pattern_allow([^AM_[A-Z]+FLAGS$])dnl

# AM_INIT_AUTOMAKE(PACKAGE, VERSION, [NO-DEFINE])
# AM_INIT_AUTOMAKE([OPTIONS])
# -----------------------------------------------
# The call with PACKAGE and VERSION arguments is the old style
# call (pre autoconf-2.50), which is being phased out.  PACKAGE
# and VERSION should now be passed to AC_INIT and removed from
# the call to AM_INIT_AUTOMAKE.
# We support both call styles for the transition.  After
# the next Automake release, Autoconf can make the AC_INIT
# arguments mandatory, and then we can depend on a new Autoconf
# release and drop the old call support.
AC_DEFUN([AM_INIT_AUTOMAKE],
[AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
 AC_REQUIRE([AC_PROG_INSTALL])dnl
# test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" &&
   test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi

# Define the identity of the package.
dnl Distinguish between old-style and new-style calls.
m4_ifval([$2],
[m4_ifval([$3], [_AM_SET_OPTION([no-define])])dnl
 AC_SUBST([PACKAGE], [$1])dnl
 AC_SUBST([VERSION], [$2])],
[_AM_SET_OPTIONS([$1])dnl
 AC_SUBST([PACKAGE], [AC_PACKAGE_TARNAME])dnl
 AC_SUBST([VERSION], [AC_PACKAGE_VERSION])])dnl

_AM_IF_OPTION([no-define],,
[AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
 AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package])])dnl

# Some tools Automake needs.
AC_REQUIRE([AM_SANITY_CHECK])dnl
AC_REQUIRE([AC_ARG_PROGRAM])dnl
AM_MISSING_PROG(ACLOCAL, "aclocal-${am__api_version}")
AM_MISSING_PROG(AUTOCONF, autoconf)
AM_MISSING_PROG(AUTOMAKE, "automake-${am__api_version}")
AM_MISSING_PROG(AUTOHEADER, autoheader)
AM_MISSING_PROG(MAKEINFO, makeinfo)
AM_MISSING_PROG(AMTAR, tar)
AM_PROG_INSTALL_SH
AM_PROG_INSTALL_STRIP
# We need awk for the "check" target.  The system "awk" is bad on
# some platforms.
AC_REQUIRE([AC_PROG_AWK])dnl
AC_REQUIRE([AC_PROG_MAKE_SET])dnl

_AM_IF_OPTION([no-dependencies],,
[AC_PROVIDE_IFELSE([AC_PROG_][CC],
                  [_AM_DEPENDENCIES(CC)],
                  [define([AC_PROG_][CC],
                          defn([AC_PROG_][CC])[_AM_DEPENDENCIES(CC)])])dnl
AC_PROVIDE_IFELSE([AC_PROG_][CXX],
                  [_AM_DEPENDENCIES(CXX)],
                  [define([AC_PROG_][CXX],
                          defn([AC_PROG_][CXX])[_AM_DEPENDENCIES(CXX)])])dnl
])
])

# Copyright 2002  Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA

# AM_AUTOMAKE_VERSION(VERSION)
# ----------------------------
# Automake X.Y traces this macro to ensure aclocal.m4 has been
# generated from the m4 files accompanying Automake X.Y.
AC_DEFUN([AM_AUTOMAKE_VERSION],[am__api_version="1.6"])

# AM_SET_CURRENT_AUTOMAKE_VERSION
# -------------------------------
# Call AM_AUTOMAKE_VERSION so it can be traced.
# This function is AC_REQUIREd by AC_INIT_AUTOMAKE.
AC_DEFUN([AM_SET_CURRENT_AUTOMAKE_VERSION],
	 [AM_AUTOMAKE_VERSION([1.6])])

# Helper functions for option handling.                    -*- Autoconf -*-

# Copyright 2001, 2002  Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 2

# _AM_MANGLE_OPTION(NAME)
# -----------------------
AC_DEFUN([_AM_MANGLE_OPTION],
[[_AM_OPTION_]m4_bpatsubst($1, [[^a-zA-Z0-9_]], [_])])

# _AM_SET_OPTION(NAME)
# ------------------------------
# Set option NAME.  Presently that only means defining a flag for this option.
AC_DEFUN([_AM_SET_OPTION],
[m4_define(_AM_MANGLE_OPTION([$1]), 1)])

# _AM_SET_OPTIONS(OPTIONS)
# ----------------------------------
# OPTIONS is a space-separated list of Automake options.
AC_DEFUN([_AM_SET_OPTIONS],
[AC_FOREACH([_AM_Option], [$1], [_AM_SET_OPTION(_AM_Option)])])

# _AM_IF_OPTION(OPTION, IF-SET, [IF-NOT-SET])
# -------------------------------------------
# Execute IF-SET if OPTION is set, IF-NOT-SET otherwise.
AC_DEFUN([_AM_IF_OPTION],
[m4_ifset(_AM_MANGLE_OPTION([$1]), [$2], [$3])])

#
# Check to make sure that the build environment is sane.
#

# Copyright 1996, 1997, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 3

# AM_SANITY_CHECK
# ---------------
AC_DEFUN([AM_SANITY_CHECK],
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftest.file
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftest.file 2> /dev/null`
   if test "$[*]" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftest.file`
   fi
   rm -f conftest.file
   if test "$[*]" != "X $srcdir/configure conftest.file" \
      && test "$[*]" != "X conftest.file $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "$[2]" = conftest.file
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
AC_MSG_RESULT(yes)])

#  -*- Autoconf -*-


# Copyright 1997, 1999, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 3

# AM_MISSING_PROG(NAME, PROGRAM)
# ------------------------------
AC_DEFUN([AM_MISSING_PROG],
[AC_REQUIRE([AM_MISSING_HAS_RUN])
$1=${$1-"${am_missing_run}$2"}
AC_SUBST($1)])


# AM_MISSING_HAS_RUN
# ------------------
# Define MISSING if not defined so far and test if it supports --run.
# If it does, set am_missing_run to use it, otherwise, to nothing.
AC_DEFUN([AM_MISSING_HAS_RUN],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
test x"${MISSING+set}" = xset || MISSING="\${SHELL} $am_aux_dir/missing"
# Use eval to expand $SHELL
if eval "$MISSING --run true"; then
  am_missing_run="$MISSING --run "
else
  am_missing_run=
  AC_MSG_WARN([`missing' script is too old or missing])
fi
])

# AM_AUX_DIR_EXPAND

# Copyright 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# For projects using AC_CONFIG_AUX_DIR([foo]), Autoconf sets
# $ac_aux_dir to `$srcdir/foo'.  In other projects, it is set to
# `$srcdir', `$srcdir/..', or `$srcdir/../..'.
#
# Of course, Automake must honor this variable whenever it calls a
# tool from the auxiliary directory.  The problem is that $srcdir (and
# therefore $ac_aux_dir as well) can be either absolute or relative,
# depending on how configure is run.  This is pretty annoying, since
# it makes $ac_aux_dir quite unusable in subdirectories: in the top
# source directory, any form will work fine, but in subdirectories a
# relative path needs to be adjusted first.
#
# $ac_aux_dir/missing
#    fails when called from a subdirectory if $ac_aux_dir is relative
# $top_srcdir/$ac_aux_dir/missing
#    fails if $ac_aux_dir is absolute,
#    fails when called from a subdirectory in a VPATH build with
#          a relative $ac_aux_dir
#
# The reason of the latter failure is that $top_srcdir and $ac_aux_dir
# are both prefixed by $srcdir.  In an in-source build this is usually
# harmless because $srcdir is `.', but things will broke when you
# start a VPATH build or use an absolute $srcdir.
#
# So we could use something similar to $top_srcdir/$ac_aux_dir/missing,
# iff we strip the leading $srcdir from $ac_aux_dir.  That would be:
#   am_aux_dir='\$(top_srcdir)/'`expr "$ac_aux_dir" : "$srcdir//*\(.*\)"`
# and then we would define $MISSING as
#   MISSING="\${SHELL} $am_aux_dir/missing"
# This will work as long as MISSING is not called from configure, because
# unfortunately $(top_srcdir) has no meaning in configure.
# However there are other variables, like CC, which are often used in
# configure, and could therefore not use this "fixed" $ac_aux_dir.
#
# Another solution, used here, is to always expand $ac_aux_dir to an
# absolute PATH.  The drawback is that using absolute paths prevent a
# configured tree to be moved without reconfiguration.

# Rely on autoconf to set up CDPATH properly.
AC_PREREQ([2.50])

AC_DEFUN([AM_AUX_DIR_EXPAND], [
# expand $ac_aux_dir to an absolute path
am_aux_dir=`cd $ac_aux_dir && pwd`
])

# AM_PROG_INSTALL_SH
# ------------------
# Define $install_sh.

# Copyright 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

AC_DEFUN([AM_PROG_INSTALL_SH],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
install_sh=${install_sh-"$am_aux_dir/install-sh"}
AC_SUBST(install_sh)])

# AM_PROG_INSTALL_STRIP

# Copyright 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# One issue with vendor `install' (even GNU) is that you can't
# specify the program used to strip binaries.  This is especially
# annoying in cross-compiling environments, where the build's strip
# is unlikely to handle the host's binaries.
# Fortunately install-sh will honor a STRIPPROG variable, so we
# always use install-sh in `make install-strip', and initialize
# STRIPPROG with the value of the STRIP variable (set by the user).
AC_DEFUN([AM_PROG_INSTALL_STRIP],
[AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
# Installed binaries are usually stripped using `strip' when the user
# run `make install-strip'.  However `strip' might not be the right
# tool to use in cross-compilation environments, therefore Automake
# will honor the `STRIP' environment variable to overrule this program.
dnl Don't test for $cross_compiling = yes, because it might be `maybe'.
if test "$cross_compiling" != no; then
  AC_CHECK_TOOL([STRIP], [strip], :)
fi
INSTALL_STRIP_PROGRAM="\${SHELL} \$(install_sh) -c -s"
AC_SUBST([INSTALL_STRIP_PROGRAM])])

# serial 4						-*- Autoconf -*-

# Copyright 1999, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.


# There are a few dirty hacks below to avoid letting `AC_PROG_CC' be
# written in clear, in which case automake, when reading aclocal.m4,
# will think it sees a *use*, and therefore will trigger all it's
# C support machinery.  Also note that it means that autoscan, seeing
# CC etc. in the Makefile, will ask for an AC_PROG_CC use...



# _AM_DEPENDENCIES(NAME)
# ----------------------
# See how the compiler implements dependency checking.
# NAME is "CC", "CXX", "GCJ", or "OBJC".
# We try a few techniques and use that to set a single cache variable.
#
# We don't AC_REQUIRE the corresponding AC_PROG_CC since the latter was
# modified to invoke _AM_DEPENDENCIES(CC); we would have a circular
# dependency, and given that the user is not expected to run this macro,
# just rely on AC_PROG_CC.
AC_DEFUN([_AM_DEPENDENCIES],
[AC_REQUIRE([AM_SET_DEPDIR])dnl
AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
AC_REQUIRE([AM_MAKE_INCLUDE])dnl
AC_REQUIRE([AM_DEP_TRACK])dnl

ifelse([$1], CC,   [depcc="$CC"   am_compiler_list=],
       [$1], CXX,  [depcc="$CXX"  am_compiler_list=],
       [$1], OBJC, [depcc="$OBJC" am_compiler_list='gcc3 gcc']
       [$1], GCJ,  [depcc="$GCJ"  am_compiler_list='gcc3 gcc'],
                   [depcc="$$1"   am_compiler_list=])

AC_CACHE_CHECK([dependency style of $depcc],
               [am_cv_$1_dependencies_compiler_type],
[if test -z "$AMDEP_TRUE" && test -f "$am_depcomp"; then
  # We make a subdir and do the tests there.  Otherwise we can end up
  # making bogus files that we don't know about and never remove.  For
  # instance it was reported that on HP-UX the gcc test will end up
  # making a dummy file named `D' -- because `-MD' means `put the output
  # in D'.
  mkdir conftest.dir
  # Copy depcomp to subdir because otherwise we won't find it if we're
  # using a relative directory.
  cp "$am_depcomp" conftest.dir
  cd conftest.dir

  am_cv_$1_dependencies_compiler_type=none
  if test "$am_compiler_list" = ""; then
     am_compiler_list=`sed -n ['s/^#*\([a-zA-Z0-9]*\))$/\1/p'] < ./depcomp`
  fi
  for depmode in $am_compiler_list; do
    # We need to recreate these files for each test, as the compiler may
    # overwrite some of them when testing with obscure command lines.
    # This happens at least with the AIX C compiler.
    echo '#include "conftest.h"' > conftest.c
    echo 'int i;' > conftest.h
    echo "${am__include} ${am__quote}conftest.Po${am__quote}" > confmf

    case $depmode in
    nosideeffect)
      # after this tag, mechanisms are not by side-effect, so they'll
      # only be used when explicitly requested
      if test "x$enable_dependency_tracking" = xyes; then
	continue
      else
	break
      fi
      ;;
    none) break ;;
    esac
    # We check with `-c' and `-o' for the sake of the "dashmstdout"
    # mode.  It turns out that the SunPro C++ compiler does not properly
    # handle `-M -o', and we need to detect this.
    if depmode=$depmode \
       source=conftest.c object=conftest.o \
       depfile=conftest.Po tmpdepfile=conftest.TPo \
       $SHELL ./depcomp $depcc -c conftest.c -o conftest.o >/dev/null 2>&1 &&
       grep conftest.h conftest.Po > /dev/null 2>&1 &&
       ${MAKE-make} -s -f confmf > /dev/null 2>&1; then
      am_cv_$1_dependencies_compiler_type=$depmode
      break
    fi
  done

  cd ..
  rm -rf conftest.dir
else
  am_cv_$1_dependencies_compiler_type=none
fi
])
AC_SUBST([$1DEPMODE], [depmode=$am_cv_$1_dependencies_compiler_type])
])


# AM_SET_DEPDIR
# -------------
# Choose a directory name for dependency files.
# This macro is AC_REQUIREd in _AM_DEPENDENCIES
AC_DEFUN([AM_SET_DEPDIR],
[rm -f .deps 2>/dev/null
mkdir .deps 2>/dev/null
if test -d .deps; then
  DEPDIR=.deps
else
  # MS-DOS does not allow filenames that begin with a dot.
  DEPDIR=_deps
fi
rmdir .deps 2>/dev/null
AC_SUBST([DEPDIR])
])


# AM_DEP_TRACK
# ------------
AC_DEFUN([AM_DEP_TRACK],
[AC_ARG_ENABLE(dependency-tracking,
[  --disable-dependency-tracking Speeds up one-time builds
  --enable-dependency-tracking  Do not reject slow dependency extractors])
if test "x$enable_dependency_tracking" != xno; then
  am_depcomp="$ac_aux_dir/depcomp"
  AMDEPBACKSLASH='\'
fi
AM_CONDITIONAL([AMDEP], [test "x$enable_dependency_tracking" != xno])
AC_SUBST([AMDEPBACKSLASH])
])

# Generate code to set up dependency tracking.   -*- Autoconf -*-

# Copyright 1999, 2000, 2001, 2002 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

#serial 2

# _AM_OUTPUT_DEPENDENCY_COMMANDS
# ------------------------------
AC_DEFUN([_AM_OUTPUT_DEPENDENCY_COMMANDS],
[for mf in $CONFIG_FILES; do
  # Strip MF so we end up with the name of the file.
  mf=`echo "$mf" | sed -e 's/:.*$//'`
  if (sed 1q $mf | fgrep 'generated by automake') > /dev/null 2>&1; then
    dirpart=`AS_DIRNAME("$mf")`
  else
    continue
  fi
  grep '^DEP_FILES *= *[[^ @%:@]]' < "$mf" > /dev/null || continue
  # Extract the definition of DEP_FILES from the Makefile without
  # running `make'.
  DEPDIR=`sed -n -e '/^DEPDIR = / s///p' < "$mf"`
  test -z "$DEPDIR" && continue
  # When using ansi2knr, U may be empty or an underscore; expand it
  U=`sed -n -e '/^U = / s///p' < "$mf"`
  test -d "$dirpart/$DEPDIR" || mkdir "$dirpart/$DEPDIR"
  # We invoke sed twice because it is the simplest approach to
  # changing $(DEPDIR) to its actual value in the expansion.
  for file in `sed -n -e '
    /^DEP_FILES = .*\\\\$/ {
      s/^DEP_FILES = //
      :loop
	s/\\\\$//
	p
	n
	/\\\\$/ b loop
      p
    }
    /^DEP_FILES = / s/^DEP_FILES = //p' < "$mf" | \
       sed -e 's/\$(DEPDIR)/'"$DEPDIR"'/g' -e 's/\$U/'"$U"'/g'`; do
    # Make sure the directory exists.
    test -f "$dirpart/$file" && continue
    fdir=`AS_DIRNAME(["$file"])`
    AS_MKDIR_P([$dirpart/$fdir])
    # echo "creating $dirpart/$file"
    echo '# dummy' > "$dirpart/$file"
  done
done
])# _AM_OUTPUT_DEPENDENCY_COMMANDS


# AM_OUTPUT_DEPENDENCY_COMMANDS
# -----------------------------
# This macro should only be invoked once -- use via AC_REQUIRE.
#
# This code is only required when automatic dependency tracking
# is enabled.  FIXME.  This creates each `.P' file that we will
# need in order to bootstrap the dependency handling code.
AC_DEFUN([AM_OUTPUT_DEPENDENCY_COMMANDS],
[AC_CONFIG_COMMANDS([depfiles],
     [test x"$AMDEP_TRUE" != x"" || _AM_OUTPUT_DEPENDENCY_COMMANDS],
     [AMDEP_TRUE="$AMDEP_TRUE" ac_aux_dir="$ac_aux_dir"])
])

# Copyright 2001 Free Software Foundation, Inc.             -*- Autoconf -*-

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 2

# AM_MAKE_INCLUDE()
# -----------------
# Check to see how make treats includes.
AC_DEFUN([AM_MAKE_INCLUDE],
[am_make=${MAKE-make}
cat > confinc << 'END'
doit:
	@echo done
END
# If we don't find an include directive, just comment out the code.
AC_MSG_CHECKING([for style of include used by $am_make])
am__include="#"
am__quote=
_am_result=none
# First try GNU make style include.
echo "include confinc" > confmf
# We grep out `Entering directory' and `Leaving directory'
# messages which can occur if `w' ends up in MAKEFLAGS.
# In particular we don't look at `^make:' because GNU make might
# be invoked under some other name (usually "gmake"), in which
# case it prints its new name instead of `make'.
if test "`$am_make -s -f confmf 2> /dev/null | fgrep -v 'ing directory'`" = "done"; then
   am__include=include
   am__quote=
   _am_result=GNU
fi
# Now try BSD make style include.
if test "$am__include" = "#"; then
   echo '.include "confinc"' > confmf
   if test "`$am_make -s -f confmf 2> /dev/null`" = "done"; then
      am__include=.include
      am__quote="\""
      _am_result=BSD
   fi
fi
AC_SUBST(am__include)
AC_SUBST(am__quote)
AC_MSG_RESULT($_am_result)
rm -f confinc confmf
])

# AM_CONDITIONAL                                              -*- Autoconf -*-

# Copyright 1997, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 5

AC_PREREQ(2.52)

# AM_CONDITIONAL(NAME, SHELL-CONDITION)
# -------------------------------------
# Define a conditional.
AC_DEFUN([AM_CONDITIONAL],
[ifelse([$1], [TRUE],  [AC_FATAL([$0: invalid condition: $1])],
        [$1], [FALSE], [AC_FATAL([$0: invalid condition: $1])])dnl
AC_SUBST([$1_TRUE])
AC_SUBST([$1_FALSE])
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi
AC_CONFIG_COMMANDS_PRE(
[if test -z "${$1_TRUE}" && test -z "${$1_FALSE}"; then
  AC_MSG_ERROR([conditional \"$1\" was never defined.
Usually this means the macro was only invoked conditionally.])
fi])])

AC_DEFUN([heroes_aux_OBSOPT],
[if test -n "$MY_TR_SH($1)"; then
  AC_MSG_ERROR([--$1 is an obsolete option[]ifelse([$2],,,[,
you should use --$2 instead])])
fi])

AC_DEFUN([heroes_aux_ERROPT],
[if test -n "$MY_TR_SH($1)"; then
  AC_MSG_ERROR([--$1 is an illegal option[]ifelse([$2],,,[,
you probably want to use --$2])])
fi])
AC_DEFUN([heroes_CHECK_OBSOLETE_OPTIONS],
[# check for opsolete or erroneous options
heroes_aux_ERROPT([with-SDL],[with-sdl])
dnl It's not possible to test for the next one:
dnl because both sides maps to the same shell variable :o(
dnl heroes_aux_OBSOPT([with-sdl_mixer],[with-sdl-mixer])
heroes_aux_OBSOPT([with-GGI],[with-ggi])
heroes_aux_OBSOPT([with-GII],[with-gii])
])

dnl The Autoconf maintainers keep moving undocumented functions around,
dnl so let's use what we find, to stay compatible with different version
dnl of Autoconf...
AC_DEFUN([MY_TR_SH],[ifdef([AS_TR_SH],[AS_TR_SH($@)],[AC_TR_SH($@)])])
AC_DEFUN([MY_TR_CPP],[ifdef([AS_TR_CPP],[AS_TR_CPP($@)],[AC_TR_CPP($@)])])
AC_DEFUN([MY_VAR_PUSHDEF],
	 [ifdef([AS_VAR_PUSHDEF],[AS_VAR_PUSHDEF($@)],[AC_VAR_PUSHDEF($@)])])
AC_DEFUN([MY_VAR_POPDEF],
	 [ifdef([AS_VAR_POPDEF],[AS_VAR_POPDEF($@)],[AC_VAR_POPDEF($@)])])
AC_DEFUN([MY_VAR_SET],[ifdef([AS_VAR_SET],[AS_VAR_SET($@)],[AC_VAR_SET($@)])])

#serial 1
# This test replaces the one in autoconf.
# Currently this macro should have the same name as the autoconf macro
# because gettext's gettext.m4 (distributed in the automake package)
# still uses it.  Otherwise, the use in gettext.m4 makes autoheader
# give these diagnostics:
#   configure.in:556: AC_TRY_COMPILE was called before AC_ISC_POSIX
#   configure.in:556: AC_TRY_RUN was called before AC_ISC_POSIX

undefine([AC_ISC_POSIX])

AC_DEFUN([AC_ISC_POSIX],
  [
    dnl This test replaces the obsolescent AC_ISC_POSIX kludge.
    AC_CHECK_LIB(cposix, strerror, [LIBS="$LIBS -lcposix"])
  ]
)

# serial 2

# AM_PROG_CC_C_O
# --------------
# Like AC_PROG_CC_C_O, but changed for automake.

# Copyright 1999, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

AC_DEFUN([AM_PROG_CC_C_O],
[AC_REQUIRE([AC_PROG_CC_C_O])dnl
AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
# FIXME: we rely on the cache variable name because
# there is no other way.
set dummy $CC
ac_cc=`echo $[2] | sed ['s/[^a-zA-Z0-9_]/_/g;s/^[0-9]/_/']`
if eval "test \"`echo '$ac_cv_prog_cc_'${ac_cc}_c_o`\" != yes"; then
   # Losing compiler, so override with the script.
   # FIXME: It is wrong to rewrite CC.
   # But if we don't then we get into trouble of one sort or another.
   # A longer-term fix would be to have automake use am__CC in this case,
   # and then we could set am__CC="\$(top_srcdir)/compile \$(CC)"
   CC="$am_aux_dir/compile $CC"
fi
])

#serial 3
# Make sure _GNU_SOURCE is defined where necessary: as early as possible
# for configure-time tests, as well as for every source file that includes
# config.h.

# From Jim Meyering.

AC_DEFUN([AC__GNU_SOURCE],
[
  # Make sure that _GNU_SOURCE is defined for all subsequent
  # configure-time compile tests.
  # This definition must be emitted (into confdefs.h) before any
  # test that involves compilation.
  cat >>confdefs.h <<\EOF
#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
EOF

  # Emit this code into config.h.in.
  # The ifndef is to avoid redefinition warnings.
  AH_VERBATIM([_GNU_SOURCE], [#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif])
])

AC_DEFUN([heroes_MEDIALIB_SELECTION], [

  user_selection_list_vkm=''
  user_selection_list_s=''
  user_selection_list_j=''

  heroes_CHECK_DISPLAY_LIB
  heroes_CHECK_JOYSTICK_LIB
  heroes_CHECK_SOUND_LIB

  # These libraries do not need any checking.
  adl_LIBALT_OK([dums], [none], [], [])
  adl_LIBALT_OK([dumj], [none], [], [])
  adl_LIBALT_OK([stdm], [standard main], [], [])
  # There is a trick here.  Usually our static library are added
  # to LIBALT_LOCAL_LDADD which is listed before LIBALT_LDADD (the system
  # libraries) on the link line.  However, SDL_main and Allegro are special:
  # they refer to OUR code (practically they call our main, which has been
  # renamed to SDL_main() or _mangled_main()). So in addition to adding
  # sys/libhallm.a to LIBALT_LOCAL_LDADD (done automatically by adl_LIBALT_OK),
  # we will also list it in LIBALT_LDADD (requested below).  The 'APPEND' means
  # the libraries should be appended instead of being prepended, since we
  # want them to be listed after -lSDL_main or -lallegro.
  adl_LIBALT_OK([sdlm], [SDL_main], [],
		[sys/libhsdlm.a], [sdlvkm], [APPEND])
  adl_LIBALT_OK([allm], [Allegro mangled main], [],
		[sys/libhallm.a], [allvkm], [APPEND])

  # Heroes can run without display, but usually this is not what
  # the user wants :), so this "feature" is enabled only if
  # --disable-display is supplied.
  AC_ARG_ENABLE([display],
	        [AC_HELP_STRING([--disable-display],
                                [don't draw anything on screen])])
  if test "${enable_display-yes}" = no; then
    user_selection_list_vkm="dumvkm $user_selection_list_vkm"
    adl_LIBALT_OK([dumvkm], [none], [], [])
  fi

  # If the user has no preference, we'll use our own preference list.

  if test -z "$user_selection_list_vkm"; then
     selection_list_vkm="ggivkm sdlvkm allvkm dumvkm"
  else
     selection_list_vkm="$user_selection_list_vkm dumvkm"
  fi

  if test -z "$user_selection_list_s"; then
     # Keep sdls first.  It won't be used if sdlvkm is not selected,
     # but otherwise it should be prefered over miks.
     selection_list_s="sdls miks dums"
  else
     selection_list_s="$user_selection_list_s dums"
  fi

  if test -z "$user_selection_list_j"; then
     # Keep sdlj first.  It won't be used if sdlvkm is not selected,
     # but otherwise it should be prefered over giij.
     # Keep allj second for the same reason.
     selection_list_j="sdlj allj giij dumj"
  else
     selection_list_j="$user_selection_list_j dumj"
  fi

  adl_LIBALT_EITHER([$selection_list_vkm], [video-keyboard-mouse library],
	            [selection_vkm], [media/libh])
  adl_LIBALT_EITHER([$selection_list_s], [sound library], [selection_s],
		    [media/libh])
  adl_LIBALT_EITHER([$selection_list_j], [joystick library], [selection_j],
	            [media/libh])
  adl_LIBALT_EITHER([sdlm allm stdm], [startup library], [selection_m],
		    [sys/libh])
])

AC_DEFUN([heroes_CHECK_DISPLAY_LIB],
 [
    heroes_CHECK_GGI([adl_LIBALT_OK([ggivkm], [LibGGI], [$GGI_LDFLAGS],
				    [$GGI_LIBS])])
    heroes_CHECK_SDL([adl_LIBALT_OK([sdlvkm], [SDL dnl
$sdl_config_major_version.dnl
$sdl_config_minor_version.dnl
$sdl_config_micro_version], [], [$SDL_LIBS])],
     [# Set to `no' so that we don't check for joystick or sound support
with_sdl=no])
    heroes_CHECK_ALLEGRO([adl_LIBALT_OK([allvkm], [Allegro dnl
$allegro_config_major_version.dnl
$allegro_config_minor_version.dnl
$allegro_config_micro_version], [], [$ALLEGRO_LIBS])],
     [# Set to `no' so that we don't check for joystick support
with_allegro=no])
])

AC_DEFUN([heroes_CHECK_GGI],
[if test "${with_ggi-yes}" != no; then
   if test "${with_ggi-no}" != no; then
     user_selection_list_vkm="$user_selection_list_vkm ggivkm"
   fi
   AC_caolan_CHECK_PACKAGE([GGI],[ggiOpen],[ggi],[ggi/ggi.h],,[with_ggi=no])
 fi
 if test "${with_ggi-yes}" != no; then
   AC_adl_FIND_HEADER([ggi/keyboard.h],,
     [/usr/include/ggi/keyboard.h] dnl
     [${ggi_include-/usr/local/include}/ggi/keyboard.h],
     [#[ 	]*define[ 	][ 	]*GIIUC*],7,,
     [AC_MSG_ERROR([Keysyms macros not found: where is GGI installed?])])

   GGI_KEYBOARD_H="$ac_cv_header_path_ggi_keyboard_h"
   AC_SUBST([GGI_KEYBOARD_H])
   $1
 ifelse([$2],,,[else
   $2])
 fi
])

dnl @synopsis AC_caolan_CHECK_PACKAGE(PACKAGE, FUNCTION, LIBRARY, HEADERFILE [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
dnl Provides --with-PACKAGE, --with-PACKAGE-include and --with-PACKAGE-libdir
dnl options to configure. Supports the now standard --with-PACKAGE=DIR
dnl approach where the package's include dir and lib dir are underneath DIR,
dnl but also allows the include and lib directories to be specified seperately
dnl
dnl adds the extra -Ipath to CFLAGS if needed
dnl adds extra -Lpath to LD_FLAGS if needed
dnl searches for the FUNCTION in the LIBRARY with
dnl AC_CHECK_LIBRARY and thus adds the lib to LIBS
dnl
dnl defines HAVE_PKG_PACKAGE if it is found, (where PACKAGE in the
dnl HAVE_PKG_PACKAGE is replaced with the actual first parameter passed)
dnl note that autoheader will complain of not having the HAVE_PKG_PACKAGE and you
dnl will have to add it to acconfig.h manually
dnl
dnl @version $Id: ac_caolan_check_package.m4,v 1.5 2000/08/30 08:50:25 simons Exp $
dnl @author Caolan McNamara <caolan@skynet.ie> with fixes from Alexandre Duret-Lutz <duret_g@lrde.epita.fr>.
dnl
AC_DEFUN([AC_caolan_CHECK_PACKAGE],
[
dnl we're going to need uppercase, lowercase and user-friendly versions of the
dnl string `LIBRARY'
pushdef([UP], MY_TR_CPP([$1]))dnl
pushdef([DOWN], translit([$1], [-A-Z], [_a-z]))dnl
pushdef([DOWNALT], translit([$1], [_A-Z], [-a-z]))dnl

AC_ARG_WITH(DOWNALT,
[AC_HELP_STRING([--with-]DOWNALT[=DIR],[root directory of $1 installation])],
[with_[]DOWN=$withval
if test "${with_[]DOWN}" != yes; then
        DOWN[]_include="$withval/include"
        DOWN[]_libdir="$withval/lib"
fi]
)

AC_ARG_WITH(DOWNALT[-include],
[AC_HELP_STRING([--with-]DOWNALT[-include=DIR],
                [specify exact include dir for $1 headers])],
[DOWN[]_include="$withval"])

AC_ARG_WITH(DOWNALT[-libdir],
[AC_HELP_STRING([--with-]DOWNALT[-libdir=DIR],
                [specify exact library dir for $1 library])
AC_HELP_STRING([--without-]DOWNALT,[disables ]DOWN[ usage completely])],
[DOWN[]_libdir="$withval"])

if test "${with_[]DOWN}" != no ; then
        OLD_LIBS="$LIBS"
        OLD_LDFLAGS="$LDFLAGS"
        OLD_CFLAGS="$CFLAGS"
        OLD_CPPFLAGS="$CPPFLAGS"

        if test "${DOWN[]_libdir}" ; then
		UP[]_LDFLAGS="-L${DOWN[]_libdir}"
                LDFLAGS="$LDFLAGS ${UP[]_LDFLAGS}"
        fi
        if test "${DOWN[]_include}" ; then
		UP[]_CFLAGS="-I${DOWN[]_include}"
                CPPFLAGS="$CPPFLAGS ${UP[]_CFLAGS}"
                CFLAGS="$CFLAGS ${UP[]_CFLAGS}"
        fi

	no_good=no
	dnl pass a third argument to AC_CHECK_LIB to
	dnl prevent HAVE_LIB$3 from being defined.
        AC_CHECK_LIB([$3],[$2],[:],[no_good=yes])
        AC_CHECK_HEADER([$4],,[no_good=yes])
 	LIBS="$OLD_LIBS"
 	LDFLAGS="$OLD_LDFLAGS"
 	CPPFLAGS="$OLD_CPPFLAGS"
 	CFLAGS="$OLD_CFLAGS"
        if test "$no_good" = yes; then
dnl     broken
                ifelse([$6], , , [$6])
		UP[]_LDFLAGS=""
		UP[]_CFLAGS=""
		UP[]_LIBS=""
        else
dnl     fixed
		UP[]_LIBS="-l$3"
                ifelse([$5], , , [$5])
        fi

fi

  AC_SUBST(UP[]_CFLAGS)
  AC_SUBST(UP[]_LIBS)
  AC_SUBST(UP[]_LDFLAGS)

  popdef([UP])
  popdef([DOWN])
  popdef([DOWNALT])
])

dnl AC_adl_FIND_HEADER(HEADER-NAME, CPPFLAGS, OTHER-NAMES, REGEX, NMATCH
dnl                    ACTION-IF-YES, ACTION-IF-NOT)
dnl
dnl Search for the exact location of header `<HEADER-NAME>' which must
dnl match `NMATCH' times the regex `REGEX'.  If is header is not found,
dnl try the OTHER-NAMES list of headers.
dnl
dnl Generalized by Alexandre Duret-Lutz <duret_g@epita.fr>
dnl from a test in Zsh's configure.in (written by?)

AC_DEFUN([AC_adl_FIND_HEADER],
[# Where is <$1> located?
MY_VAR_PUSHDEF([ac_adl_Header], [ac_cv_header_path_$1])
AC_CACHE_CHECK([where $1 is located], ac_adl_Header,
[# Look at the output from the preprocessor.
# We should get lines of the form `# 1 "/usr/include/$1"'
# The backslash substitution is to persuade cygwin to cough up
# slashes rather than doubled backslashes in the path.
echo "#include <$1>" > nametmp.c
afh_file_list="`$CPP $2 nametmp.c 2> /dev/null |
sed -n 's/^#[ 	].*\"\(.*\)\"/\1/p' |
sed 's/\\\\\\\\/\//g' |
$AWK '{ if (\@S|@1 ~ \"$1\") files[[\@S|@1]] = \@S|@1 }
  END { for (var in files) print var }'`"
dnl Note: `@S|@' will be translated into `$' by autoconf.
rm -f nametmp.c
if test -z "$afh_file_list"; then
  # In case we don't get the stuff from the preprocesor, use the
  # list of standard places.
  afh_file_list="$3 /dev/null"
fi
for ASH_FILE_H in $afh_file_list
do
  # make sure it doesn't get confused by files that don't
  # define what is searched for
  ash_nmatch=`test -f $ASH_FILE_H && \
  grep '$4' $ASH_FILE_H | \
  wc -l | sed 's/[ 	]//g'`
  test "x$ash_nmatch" != x && test "$ash_nmatch" -ge $5 && break
done
MY_VAR_SET(ac_adl_Header,[$ASH_FILE_H])
if test "$ASH_FILE_H" = "/dev/null"; then
  ifelse([$7], , [:], [$7])
else
  ifelse([$6], , [:], [$6])
fi])
MY_VAR_POPDEF([ac_adl_Header])])

# adl_LIBALT_OK (HANDLE, NAME, LDFLAGS, LDADD, [DEPENDENCIES], [OPTION])
# ----------------------------------------------------------------------
AC_DEFUN([adl_LIBALT_OK], [
  adl_libalt_$1_ok=yes
  adl_libalt_$1_name="$2"
  adl_libalt_$1_LDFLAGS="$3"
  adl_libalt_$1_LDADD="$4"
  adl_libalt_$1_selected=no
  adl_libalt_$1_depend_on="$5"
  adl_libalt_$1_opt="$6"
])

# adl_LIBALT_EITHER (HANDLES, DOC, VAR, LDADD_PREFIX)
# ---------------------------------------------------
AC_DEFUN([adl_LIBALT_EITHER], [
  $3=NONE
  for i in $1; do
    if eval test "x\$adl_libalt_${i}_ok" = xyes ; then
      # make sure all dependencies for this module have already been selected.
      eval adl_libalt_tmp_dep="\$adl_libalt_${i}_depend_on"
      adl_libalt_tmp_ok=yes
      for j in $adl_libalt_tmp_dep : ; do
        if test $j = : ; then
          break
        fi
        if eval test "x\$adl_libalt_${j}_selected" != xyes ; then
          adl_libalt_tmp_ok=no
          break
        fi
      done
      if test "$adl_libalt_tmp_ok" = yes ; then
        eval adl_libalt_tmp_opt=\"\$adl_libalt_${i}_opt\"
	# The default is to prepend.
	if test x"$adl_libalt_tmp_opt" = xAPPEND; then
          eval LIBALT_LDFLAGS=\"\$LIBALT_LDFLAGS \$adl_libalt_${i}_LDFLAGS\"
          eval LIBALT_LDADD=\"\$LIBALT_LDADD \$adl_libalt_${i}_LDADD\"
          ifelse([$4],,, [LIBALT_LOCAL_LDADD="$LIBALT_LOCAL_LDADD $4$i.a"])
	else
          eval LIBALT_LDFLAGS=\"\$adl_libalt_${i}_LDFLAGS \$LIBALT_LDFLAGS\"
          eval LIBALT_LDADD=\"\$adl_libalt_${i}_LDADD \$LIBALT_LDADD\"
          ifelse([$4],,, [LIBALT_LOCAL_LDADD="$4$i.a $LIBALT_LOCAL_LDADD"])
	fi
        eval "adl_libalt_${i}_selected=yes"
        $3=$i
        break
      fi
    fi
  done
  if test "x$$3" = xNONE ; then
     AC_MSG_ERROR([No $2 found.

The supported libraries are listed in README.])
  else
     eval adl_tmp_libalt_name='$adl_libalt_'"$$3"'_name'
     AC_MSG_NOTICE([Selected $2: $adl_tmp_libalt_name])
  fi
  # Use _XLDLAGS, _XLDADS, etc. because Automake reserves _LDFLAGS, etc.
  AC_SUBST([LIBALT_XLDFLAGS], [$LIBALT_LDFLAGS])
  AC_SUBST([LIBALT_XLDADD], [$LIBALT_LDADD])
  AC_SUBST([LIBALT_LOCAL_XLDADD], [$LIBALT_LOCAL_LDADD])
])

AC_DEFUN([heroes_CHECK_SDL],
[AC_ARG_WITH([sdl],
  [AC_HELP_STRING([--with-sdl=DIR],[root directory of SDL installation])
AC_HELP_STRING([--without-sdl],[disables SDL usage completely])])

 # --with-sdl-exec-prefix should imply --with-sdl, and vice-versa

 test "${with_sdl_exec_prefix-no}" != no &&
  test "${with_sdl-yes}" = yes && with_sdl="$with_sdl_exec_prefix"

 test "${with_sdl-yes}" != yes &&
  test "${with_sdl_exec_prefix-yes}" = yes && with_sdl_exec_prefix="$with_sdl"

 if test "${with_sdl-yes}" != no; then
  if test "${with_sdl-no}" != no; then
     user_selection_list_vkm="$user_selection_list_vkm sdlvkm"
  fi

  AC_adl_PKG_GENERIC(sdl,[1.0.1],[SDL_Init],
   [AC_CHECK_FUNCS([SDL_EnableKeyRepeat])],
   [with_sdl=no])
 fi
 if test "${with_sdl-yes}" != no; then
   AC_adl_FIND_HEADER([SDL_keysym.h],[$SDL_CFLAGS],
    [/usr/include/SDL/SDL_keysym.h] dnl
    [/usr/local/include/SDL/SDL_keysym.h],
    [#[		]*define[ 	][ 	]*SDLK_*],7,,
    [AC_MSG_ERROR([Keysyms definitions not found: where is SDL installed?])])

 SDL_KEYSYM_H="$ac_cv_header_path_SDL_keysym_h"
 AC_SUBST([SDL_KEYSYM_H])
 $1
 ifelse([$2],,,[else
   $2])
 fi
])

dnl This will call AC_PATH_GENERIC but check that the library actually link.
dnl (people installing library from binary packages can have libraries
dnl that do NOT link when dependencies are not satisfied)
dnl
dnl  AC_adl_PKG_GENERIC(library,version,function,action-if-ok,action-if-not)
dnl
dnl Inspired from AC_CAOLAN_CHECK_PACKAGE.
dnl Alexandre Duret-Lutz <duret_g@epita.fr>
AC_DEFUN([AC_adl_PKG_GENERIC],[
  pushdef([UP], translit([$1], [a-z], [A-Z]))dnl

  OLD_LIBS="$LIBS"
  OLD_CFLAGS="$CFLAGS"
  jolly_good=true
  AC_PATH_GENERIC([$1],[$2],,[jolly_good=false])
  ifelse([$3],,,[if $jolly_good; then
    CFLAGS="$UP[]_CFLAGS $CFLAGS"
    LIBS="$UP[]_LIBS $LIBS"
    AC_CHECK_FUNC([$3],,[jolly_good=false])
  fi])
  if $jolly_good; then
    ifelse([$4],,,[$4])
    LIBS="$OLD_LIBS"
    CFLAGS="$OLD_CFLAGS"
  else
    LIBS="$OLD_LIBS"
    CFLAGS="$OLD_CFLAGS"
    ifelse([$5],,,[$5])
  fi

  popdef([UP])
])

dnl @synopsis AC_PATH_GENERIC(LIBRARY [, MINIMUM-VERSION [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl
dnl Runs a LIBRARY-config script and defines LIBRARY_CFLAGS and LIBRARY_LIBS
dnl
dnl The script must support `--cflags' and `--libs' args.
dnl If MINIMUM-VERSION is specified, the script must also support the
dnl `--version' arg.
dnl If the `--with-library-[exec-]prefix' arguments to ./configure are given,
dnl it must also support `--prefix' and `--exec-prefix'.
dnl (In other words, it must be like gtk-config.)
dnl
dnl For example:
dnl
dnl    AC_PATH_GENERIC(Foo, 1.0.0)
dnl
dnl would run `foo-config --version' and check that it is at least 1.0.0
dnl
dnl If so, the following would then be defined:
dnl
dnl    FOO_CFLAGS to `foo-config --cflags`
dnl    FOO_LIBS   to `foo-config --libs`
dnl
dnl At present there is no support for additional "MODULES" (see AM_PATH_GTK)
dnl (shamelessly stolen from gtk.m4 and then hacked around a fair amount)
dnl
dnl @author Angus Lees <gusl@cse.unsw.edu.au>
dnl @version $Id: generic-config.m4,v 1.2 2000/07/19 13:03:32 simons Exp $

AC_DEFUN([AC_PATH_GENERIC],
[dnl
dnl we're going to need uppercase, lowercase and user-friendly versions of the
dnl string `LIBRARY'
pushdef([UP], translit([$1], [a-z], [A-Z]))dnl
pushdef([DOWN], translit([$1], [A-Z], [a-z]))dnl

dnl
dnl Get the cflags and libraries from the LIBRARY-config script
dnl
AC_ARG_WITH(DOWN[-prefix],
[AC_HELP_STRING([--with-]DOWN[-prefix=PFX],
                [Prefix where $1 is installed (optional)])],
DOWN[]_config_prefix="$withval", DOWN[]_config_prefix="")

AC_ARG_WITH(DOWN[-exec-prefix],
[AC_HELP_STRING([--with-]DOWN[-exec-prefix=PFX],
                [Exec prefix where $1 is installed (optional)])],
DOWN[]_config_exec_prefix="$withval", DOWN[]_config_exec_prefix="")

  if test x$DOWN[]_config_exec_prefix != x ; then
     DOWN[]_config_args="$DOWN[]_config_args --exec-prefix=$DOWN[]_config_exec_prefix"
     if test x${UP[]_CONFIG+set} != xset ; then
       UP[]_CONFIG=$DOWN[]_config_exec_prefix/bin/DOWN-config
     fi
  fi
  if test x$DOWN[]_config_prefix != x ; then
     DOWN[]_config_args="$DOWN[]_config_args --prefix=$DOWN[]_config_prefix"
     if test x${UP[]_CONFIG+set} != xset ; then
       UP[]_CONFIG=$DOWN[]_config_prefix/bin/DOWN-config
     fi
  fi

  AC_PATH_PROG(UP[]_CONFIG, DOWN-config, no)
  ifelse([$2], ,
     AC_MSG_CHECKING(for $1),
     AC_MSG_CHECKING(for $1 - version >= $2)
  )
  no_[]DOWN=""
  if test "$UP[]_CONFIG" = "no" ; then
     no_[]DOWN=yes
  else
     UP[]_CFLAGS="`$UP[]_CONFIG $DOWN[]_config_args --cflags`"
     UP[]_LIBS="`$UP[]_CONFIG $DOWN[]_config_args --libs`"
     ifelse([$2], , ,[
        DOWN[]_config_major_version=`$UP[]_CONFIG $DOWN[]_config_args \
         --version | sed 's/[[^0-9]]*\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
        DOWN[]_config_minor_version=`$UP[]_CONFIG $DOWN[]_config_args \
         --version | sed 's/[[^0-9]]*\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
        DOWN[]_config_micro_version=`$UP[]_CONFIG $DOWN[]_config_args \
         --version | sed 's/[[^0-9]]*\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
        DOWN[]_wanted_major_version="regexp($2, [\<\([0-9]*\)], [\1])"
        DOWN[]_wanted_minor_version="regexp($2, [\<\([0-9]*\)\.\([0-9]*\)], [\2])"
        DOWN[]_wanted_micro_version="regexp($2, [\<\([0-9]*\).\([0-9]*\).\([0-9]*\)], [\3])"

        # Compare wanted version to what config script returned.
        # If I knew what library was being run, i'd probably also compile
        # a test program at this point (which also extracted and tested
        # the version in some library-specific way)
        if test "$DOWN[]_config_major_version" -lt \
                        "$DOWN[]_wanted_major_version" \
          -o \( "$DOWN[]_config_major_version" -eq \
                        "$DOWN[]_wanted_major_version" \
            -a "$DOWN[]_config_minor_version" -lt \
                        "$DOWN[]_wanted_minor_version" \) \
          -o \( "$DOWN[]_config_major_version" -eq \
                        "$DOWN[]_wanted_major_version" \
            -a "$DOWN[]_config_minor_version" -eq \
                        "$DOWN[]_wanted_minor_version" \
            -a "$DOWN[]_config_micro_version" -lt \
                        "$DOWN[]_wanted_micro_version" \) ; then
          # older version found
          no_[]DOWN=yes
          echo -n "*** An old version of $1 "
          echo -n "($DOWN[]_config_major_version"
          echo -n ".$DOWN[]_config_minor_version"
          echo    ".$DOWN[]_config_micro_version) was found."
          echo -n "*** You need a version of $1 newer than "
          echo -n "$DOWN[]_wanted_major_version"
          echo -n ".$DOWN[]_wanted_minor_version"
          echo    ".$DOWN[]_wanted_micro_version."
          echo "***"
          echo "*** If you have already installed a sufficiently new version, this error"
          echo "*** probably means that the wrong copy of the DOWN-config shell script is"
          echo "*** being found. The easiest way to fix this is to remove the old version"
          echo "*** of $1, but you can also set the UP[]_CONFIG environment to point to the"
          echo "*** correct copy of DOWN-config. (In this case, you will have to"
          echo "*** modify your LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf"
          echo "*** so that the correct libraries are found at run-time)"
        fi
     ])
  fi
  if test "x$no_[]DOWN" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$3], , :, [$3])
  else
     AC_MSG_RESULT(no)
     ## We won't display this warning unless the installer used --with-DOWN.
     if test "$UP[]_CONFIG" = "no" && test "${with_[]DOWN[]-no}" != no ; then
       echo "*** The DOWN-config script installed by $1 could not be found"
       echo "*** If $1 was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the UP[]_CONFIG environment variable to the"
       echo "*** full path to DOWN-config."
     fi
     UP[]_CFLAGS=""
     UP[]_LIBS=""
     ifelse([$4], , :, [$4])
  fi
  AC_SUBST(UP[]_CFLAGS)
  AC_SUBST(UP[]_LIBS)

  popdef([UP])
  popdef([DOWN])
])

AC_DEFUN([heroes_CHECK_ALLEGRO],
[## Allegro support is hidden until it is better supported.

 test "${with_allegro_exec_prefix-no}" != no &&
   test "${with_allegro-yes}" = yes && with_allegro="$with_allegro_exec_prefix"

 test "${with_allegro-yes}" != yes &&
   test "${with_allegro_exec_prefix-yes}" = yes && with_allegro_exec_prefix="$with_allegro"

 if test "${with_allegro-yes}" != no; then
  if test "${with_allegro-no}" != no; then
     user_selection_list_vkm="$user_selection_list_vkm allvkm"
  fi

  dnl I'd like to check that `install_allegro()' is present.
  dnl Unfortunally, Allegro is playing tricky game with the main()
  dnl definition, and the testing program compiled by configure to
  dnl check for `install_allegro()' is not aware of that.
  AC_adl_PKG_GENERIC(allegro, [3.9.33], [], [],
   [with_allegro=no])
 fi
 ## Also change -no to -yes.  See above.
 if test "${with_allegro-no}" != no; then
   AC_adl_FIND_HEADER([allegro.h],[$ALLEGRO_CFLAGS],
    [/usr/include/allegro.h] dnl
    [/usr/local/include/allegro.h],
    [#[		]*define[ 	][ 	]*KEY_*],7,,
    [AC_MSG_ERROR([Keysyms definitions not found: where is Allegro installed?])])

 ALLEGRO_H="$ac_cv_header_path_allegro_h"
 AC_SUBST([ALLEGRO_H])
 $1
 ifelse([$2],,,[else
   $2])
 fi
])

AC_DEFUN([heroes_CHECK_JOYSTICK_LIB],
[AC_ARG_ENABLE([joystick],
  [AC_HELP_STRING([--disable-joystick], [turn off joystick support])])

if test "${enable_joystick-yes}" = yes; then

  # -- check for Joystick support in SDL

  if test "${with_sdl-yes}" != no;  then
    if test "${with_sdl-no}" != no; then
       user_selection_list_j="$user_selection_list_j sdlj"
    fi

    OLIBS="$LIBS"
    LIBS="$LIBS $SDL_LIBS"
    AC_CHECK_FUNCS([SDL_JoystickOpen], [
adl_LIBALT_OK([sdlj], [SDL dnl
$sdl_config_major_version.dnl
$sdl_config_minor_version.dnl
$sdl_config_micro_version], [], [], [sdlvkm])])
    LIBS="$OLIBS"
  fi

  # -- Joystick support in Allegro is OK.
  if test "${with_allegro-yes}" != no; then
    if test "${with_allegro-no}" != no; then
      user_selection_list_j="$user_selection_list_j allj"
    fi
    adl_LIBALT_OK([allj], [Allegro dnl
$allegro_config_major_version.dnl
$allegro_config_minor_version.dnl
$allegro_config_micro_version], [], [], [allvkm])
  fi

  # -- check for LibGII (another way to get joystick support)

  if test "${with_gii-yes}" != no;  then
    if test "${with_gii-no}" != no; then
      user_selection_list_j="$user_selection_list_j giij"
    fi
    AC_caolan_CHECK_PACKAGE([GII], [giiOpen], [gii], [ggi/gii.h],
       [adl_LIBALT_OK([giij], [LibGII], [$GII_LDFLAGS], [$GII_LIBS])])
  fi
else
  user_selection_list_j="dumj $user_selection_list_j"
fi
])

AC_DEFUN([heroes_CHECK_SOUND_LIB],
[AC_ARG_ENABLE([sound],
               [AC_HELP_STRING([--disable-sound],[turn off sound support])])

 if test "${enable_sound-yes}" != no; then
   if test "${enable_sound-no}" != no; then
     user_selection_list_s="dums $user_selection_list_s"
   fi
   heroes_CHECK_SDL_MIXER([adl_LIBALT_OK([sdls], [SDL_mixer],
                                         [$SDL_MIXER_LDFLAGS],
					 [$SDL_MIXER_LIBS], [sdlvkm])])
   heroes_CHECK_MIKMOD([adl_LIBALT_OK([miks], [LibMikMod dnl
$libmikmod_config_major_version.dnl
$libmikmod_config_minor_version.dnl
$libmikmod_config_micro_version], [], [$LIBMIKMOD_LIBS])])
 fi])

AC_DEFUN([heroes_CHECK_SDL_MIXER],
[# don't even try to check for SDL_mixer if SDL is not installed
if test "${with_sdl-yes}" = no; then
  with_sdl_mixer=no
fi # with_sdl

if test "${with_sdl_mixer-yes}" != no; then

  if test "${with_sdl_mixer-no}" != no; then
    user_selection_list_s="$user_selection_list_s sdls"
  fi

  # temporaly put the CFLAGS for SDL in CPPFLAGS
  # so that AC_caolan_CHECK_PACKAGE can find SDL_mixer.h in the same directory.
  tmp_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$SDL_CFLAGS $CPPFLAGS"
  tmp_LIBS="$CPPLIBS"
  LIBS="$SDL_LIBS $LIBS"

  AC_caolan_CHECK_PACKAGE([SDL_mixer], [Mix_OpenAudio],
		[SDL_mixer], [SDL_mixer.h],
		[AC_DEFINE([HAVE_LIBSDL_MIXER], 1,
		  [Define if you have the SDL_mixer library installed.])],
		[with_sdl_mixer=no])

  # revert old value
  CPPFLAGS="$tmp_CPPFLAGS"
  LIBS="$tmp_LIBS"
fi # with_sdl_mixer

if test "x${with_sdl_mixer}" != xno; then
  with_sdl_mixer=yes
  $1
ifelse([$2],,,[else
  $2])
fi # with_sdl_mixer
])

AC_DEFUN([heroes_CHECK_MIKMOD],
[AC_ARG_WITH([mikmod],
 [AC_HELP_STRING([--with-mikmod=DIR],
                 [root directory of LibMikMod installation])
AC_HELP_STRING([--without-mikmod],
               [disables LibMikMod usage completely])])

 # --with-mikmod-exec-prefix should imply --with-mikmod, and vice-versa

 test "${with_mikmod_exec_prefix-no}" != no &&
  test "${with_mikmod-yes}" = yes && with_mikmod="$with_mikmod_exec_prefix"

 test "${with_mikmod-yes}" != yes &&
  test "${with_mikmod_exec_prefix-yes}" = yes &&
    with_mikmod_exec_prefix="$with_mikmod"

 if test "${with_mikmod-yes}" != no; then
   if test "${with_mikmod-no}" != no; then
     user_selection_list_s="$user_selection_list_s miks"
   fi
   AC_adl_PKG_GENERIC([LibMikMod], [3.1.7], [MikMod_Init], [],
		      [with_mikmod=no])
   # Since LibMikMod 3.1.10, libmikmod-config has moved some of
   # the libraries listed by --libs to the output of the --ldadd
   # option.  So we append them to LIBMIKMOD_LIBS here.
   if test "${with_mikmod-yes}" != no; then
     LIBMIKMOD_LIBS="$LIBMIKMOD_LIBS `$LIBMIKMOD_CONFIG $libmikmod_config_args --ldadd`"
   fi
 fi
 if test "${with_mikmod-yes}" != no; then
   ifelse([$1],,[:],[$1])
 ifelse([$2],,,[else
   $2])
 fi])

dnl Some macros to configure with malloc() debugging libraries.
dnl
dnl Main functions:
dnl  adl_WITH_MTRACE		handle the --with-mtrace option
dnl  adl_WITH_DMALLOC		handle the --with-dmalloc option
dnl  adl_WITH_EFENCE		handle the --with-efence option
dnl  adl_WITH_MPATROL		handle the --with-mpatrol option
dnl  adl_ENABLE_MEM_DEBUG	handle the --enable-mem-debug option
dnl				(i.e. use mpatrol, mtrace, dmalloc
dnl                              or efence, whatever the first found is)
dnl
dnl Auxiliary functions:
dnl  adl_CHECK_MTRACE		actually check for mtrace availability
dnl  adl_CHECK_DMALLOC		actually check for dmalloc availability
dnl  adl_CHECK_EFENCE		actually check for efence availability
dnl  adl_CHECK_MPATROL		actually check for mpatrol availability
dnl  adl_CHECK_MEMORY_USAGE	actually check gcc -fcheck-memory-usage
dnl                             (can be used with mpatrol, currently
dnl                              not called)
dnl  adl_CHECK_MPATROL_LIBS     actually check for libraries needed by
dnl				  mpatrol.
dnl
dnl Written by Alexandre Duret-Lutz <duret_g@epita.fr>


AC_PREREQ([2.49])  dnl for AC_HELP_STRING

dnl adl_CHECK_MTRACE([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for mtrace (GNU libc malloc debugging functions).
dnl
dnl This will define HAVE_MTRACE if mcheck.h and mtrace() exists.
dnl
dnl Use this macro in conjunction with the following code.
dnl #if HAVE_MTRACE
dnl # include <mcheck.h>
dnl #else
dnl # define mtrace()
dnl # define muntrace()
dnl #endif
dnl and put the call to mtrace and muntrace where appropriate.

AC_DEFUN([adl_CHECK_MTRACE],
 [test_failed='no'
  # should we check for mtrace?
  if test "$with_mtrace" = no; then
    test_failed='yes'
  else
    AC_CHECK_HEADER([mcheck.h],
     [AC_CHECK_FUNCS([mtrace],[$1],[test_failed=yes])],
     [test_failed=yes])
    ifelse([$2],,
     [if test $test_failed = 'yes'; then
       AC_MSG_ERROR([Cannot find mtrace])
      fi])
  fi
  ifelse([$2],,,
   [if test "$test_failed" = yes; then
     $2
    fi])])


dnl adl_WITH_MTRACE ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for dmalloc on user request.

AC_DEFUN([adl_WITH_MTRACE],
 [AC_ARG_WITH([mtrace],
  [AC_HELP_STRING([--with-mtrace],
   [use GNU libc's mtrace for malloc () debugging])])
  if test "$with_mtrace" = yes; then
   adl_CHECK_MTRACE
  fi])


dnl adl_CHECK_DMALLOC ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for dmalloc.
dnl
dnl This macro defines HAVE_DMALLOC_H when the dmalloc.h header is found.
dnl dmalloc.h is not mandatory to use dmalloc, but it is better to
dnl include it when available.  Here is how:
dnl
dnl /* keep this header at the end of the include list, because it may
dnl    define some macros to change the declaration of malloc functions */
dnl #if HAVE_DMALLOC_H
dnl # define DMALLOC_FUNC_CHECK
dnl # include <dmalloc.h>
dnl #endif

AC_DEFUN([adl_CHECK_DMALLOC],
 [test_failed='no'
  # should we check for dmalloc?
  if test "${with_dmalloc}" = no; then
    test_failed='yes'
  else
  # see if dmalloc is usable
    AC_CHECK_LIB([dmalloc],[malloc],
     [$1
      LIBS="-ldmalloc $LIBS"
      AC_CHECK_HEADERS([dmalloc.h])],
     [ifelse([$2],,[AC_MSG_ERROR([Cannot find dmalloc])],[test_failed='yes'])])
  fi
  ifelse([$2],,,
   [if test "$test_failed" = yes; then
     $2
    fi])])


dnl adl_WITH_DMALLOC ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for dmalloc on user request.

AC_DEFUN([adl_WITH_DMALLOC],
 [AC_ARG_WITH([dmalloc],
  [AC_HELP_STRING([--with-dmalloc],
   [link with dmalloc (a malloc() debugger)])])
  if test "$with_dmalloc" = yes; then
   adl_CHECK_DMALLOC
  fi])


dnl adl_CHECK_EFENCE ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for efence.

AC_DEFUN([adl_CHECK_EFENCE],
 [test_failed='no'
  # should we check for efence?
  if test "${with_efence}" = no; then
    test_failed='yes'
  else
  # see if efence is usable
    AC_CHECK_LIB([efence],[malloc],
     [$1
      LIBS="-lefence $LIBS"],
     [ifelse([$2],,[AC_MSG_ERROR([Cannot find Electric Fence])],
      [test_failed='yes'])])
  fi
  ifelse([$2],,,
   [if test "$test_failed" = yes; then
     $2
    fi])])


dnl adl_WITH_EFENCE ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for efence on user request.

AC_DEFUN([adl_WITH_EFENCE],
 [AC_ARG_WITH([efence],
  [AC_HELP_STRING([--with-efence],
   [link with efence (a malloc() debugger)])])
  if test "$with_efence" = yes; then
   adl_CHECK_EFENCE
  fi])



dnl adl_CHECK_MEMORY_USAGE
dnl Check wheter $CC supports the -fcheck-memory-usage option.
AC_DEFUN([adl_CHECK_MEMORY_USAGE],
[if test -n "$GCC"; then
  AC_CACHE_CHECK([wether $CC supports -fcheck-memory-usage],
		 [ac_cv_check_memory_usage],
  [changequote(,)dnl
  cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[]) { return argv[argc-1] == 0; }
EOF
  changequote([,])dnl
  cf_save_CFLAGS="$CFLAGS"
  CFLAGS="$cf_save_CFLAGS -fcheck-memory-usage"
    if AC_TRY_EVAL(ac_compile); then
      ac_cv_check_memory_usage=yes
    else
      ac_cv_check_memory_usage=no
    fi
  rm -f conftest*
  CFLAGS="$cf_save_CFLAGS"])
fi
if test "$ac_cv_check_memory_usage" = yes; then
  CFLAGS="${CFLAGS} -fcheck-memory-usage"
fi])

dnl adl_CHECK_MPATROL_LIBS ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for libraries needed by mpatrol
AC_DEFUN([adl_CHECK_MPATROL_LIBS],
[AC_CACHE_CHECK([for library needed by mpatrol], [ac_cv_mpatrol_libs],
[ac_cv_mpatrol_libs=missing
adl_save_LIBS=$LIBS
for lib_try in '' '-lelf' '-lbfd -liberty'  '-lbfd -liberty -lcl'; do
  LIBS="$adl_save_LIBS -lmpatrol $lib_try"
  AC_TRY_LINK_FUNC([malloc],[ac_cv_mpatrol_libs="-lmpatrol $lib_try"
break])
done
LIBS="$adl_save_LIBS"])
if test "$ac_cv_mpatrol_libs" != missing; then
  LIBS="$LIBS $ac_cv_mpatrol_libs"
  $1
ifelse([$2],,,[else
  $2])
fi])

dnl adl_CHECK_MPATROL ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for mpatrol.
dnl
dnl This macro defines HAVE_MPATROL_H when the mpatrol.h header is found.
dnl mpatrol.h is not mandatory to use mpatrol, but it is better to
dnl include it when available.  Here is how:
dnl
dnl /* keep this header at the end of the include list, because it may
dnl    define some macros to change the declaration of malloc functions */
dnl #if HAVE_MPATROL_H
dnl # include <mpatrol.h>
dnl #endif

AC_DEFUN([adl_CHECK_MPATROL],
 [test_failed='no'
  # should we check for mpatrol?
  if test "${with_mpatrol}" = no; then
    test_failed='yes'
  else
    # see if mpatrol is usable
    adl_CHECK_MPATROL_LIBS([$1
dnl   CHECK_MEMORY_USAGE is disabled, because it causes much troubles
dnl   with asm statements in system headers.
dnl   adl_CHECK_MEMORY_USAGE
      AC_CHECK_HEADERS([mpatrol.h])],
     [ifelse([$2],,[AC_MSG_ERROR([Cannot find mpatrol])],[test_failed='yes'])])
  fi
  ifelse([$2],,,
   [if test "$test_failed" = yes; then
     $2
    fi])])


dnl adl_WITH_MPATROL ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for mpatrol on user request.

AC_DEFUN([adl_WITH_MPATROL],
 [AC_ARG_WITH([mpatrol],
  [AC_HELP_STRING([--with-patrol],
   [link with mpatrol (a malloc() debugger)])])
  if test "$with_mpatrol" = yes; then
   adl_CHECK_MPATROL
  fi])


dnl adl_ENABLE_MEM_DEBUG
dnl Check for dmalloc or efence on request, use the first library found.

AC_DEFUN([adl_ENABLE_MEM_DEBUG],
 [# don't test anything if --with-dmalloc or --with-efence
  # was given (assume that adl_WITH_DMALLOC or adl_WITH_EFENCE
  # has already run).
  if test "${with_mpatrol-no}" = no &&
     test "${with_mtrace-no}" = no &&
     test "${with_dmalloc-no}" = no &&
     test "${with_efence-no}" = no; then
    AC_ARG_ENABLE([mem-debug],
    [AC_HELP_STRING([--enable-mem-debug],
     [link with any malloc() debugger available])])
    if test "$enable_mem_debug" = yes; then
      adl_CHECK_MPATROL(,[dnl
	adl_CHECK_MTRACE(,[dnl
	  adl_CHECK_DMALLOC(,[dnl
	    adl_CHECK_EFENCE])])])
    fi
  fi])

dnl Compile, link, and run a dummy program.
dnl This is used to ensure that further uses of AC_TRY_RUN
dnl not fail (usually due to some unloadable libraries),

AC_DEFUN([adl_RUN_CHECK],
[AC_TRY_RUN(
[main ()
{
  exit (0);
}],,
[AC_MSG_ERROR([===
Compiled programs no longer run whith this setup.  Most usually this
means one of the library checked for above is not correctly installed
(for example it can depens over some other uninstalled library, or
been installed in a location which is not looked at by your system).
You might want to check the error messages in the file 'config.log'.
If you believe this is a bug in Heroes, be sure to send the file
'config.log' with your report to <heroes-bugs@lists.sourceforge.net>.
])],[:])])


# Copyright 1996, 1997, 1998, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 1

AC_DEFUN([AM_C_PROTOTYPES],
[AC_REQUIRE([AM_PROG_CC_STDC])
AC_REQUIRE([AC_PROG_CPP])
AC_MSG_CHECKING([for function prototypes])
if test "$am_cv_prog_cc_stdc" != no; then
  AC_MSG_RESULT(yes)
  AC_DEFINE(PROTOTYPES,1,[Define if compiler has function prototypes])
  U= ANSI2KNR=
else
  AC_MSG_RESULT(no)
  U=_ ANSI2KNR=./ansi2knr
fi
# Ensure some checks needed by ansi2knr itself.
AC_HEADER_STDC
AC_CHECK_HEADERS(string.h)
AC_SUBST(U)dnl
AC_SUBST(ANSI2KNR)dnl
])


# Copyright 1996, 1997, 1999, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 1

# @defmac AC_PROG_CC_STDC
# @maindex PROG_CC_STDC
# @ovindex CC
# If the C compiler in not in ANSI C mode by default, try to add an option
# to output variable @code{CC} to make it so.  This macro tries various
# options that select ANSI C on some system or another.  It considers the
# compiler to be in ANSI C mode if it handles function prototypes correctly.
#
# If you use this macro, you should check after calling it whether the C
# compiler has been set to accept ANSI C; if not, the shell variable
# @code{am_cv_prog_cc_stdc} is set to @samp{no}.  If you wrote your source
# code in ANSI C, you can make an un-ANSIfied copy of it by using the
# program @code{ansi2knr}, which comes with Ghostscript.
# @end defmac

AC_DEFUN([AM_PROG_CC_STDC],
[AC_REQUIRE([AC_PROG_CC])
AC_BEFORE([$0], [AC_C_INLINE])
AC_BEFORE([$0], [AC_C_CONST])
dnl Force this before AC_PROG_CPP.  Some cpp's, eg on HPUX, require
dnl a magic option to avoid problems with ANSI preprocessor commands
dnl like #elif.
dnl FIXME: can't do this because then AC_AIX won't work due to a
dnl circular dependency.
dnl AC_BEFORE([$0], [AC_PROG_CPP])
AC_MSG_CHECKING([for ${CC-cc} option to accept ANSI C])
AC_CACHE_VAL(am_cv_prog_cc_stdc,
[am_cv_prog_cc_stdc=no
ac_save_CC="$CC"
# Don't try gcc -ansi; that turns off useful extensions and
# breaks some systems' header files.
# AIX			-qlanglvl=ansi
# Ultrix and OSF/1	-std1
# HP-UX 10.20 and later	-Ae
# HP-UX older versions	-Aa -D_HPUX_SOURCE
# SVR4			-Xc -D__EXTENSIONS__
for ac_arg in "" -qlanglvl=ansi -std1 -Ae "-Aa -D_HPUX_SOURCE" "-Xc -D__EXTENSIONS__"
do
  CC="$ac_save_CC $ac_arg"
  AC_TRY_COMPILE(
[#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
/* Most of the following tests are stolen from RCS 5.7's src/conf.sh.  */
struct buf { int x; };
FILE * (*rcsopen) (struct buf *, struct stat *, int);
static char *e (p, i)
     char **p;
     int i;
{
  return p[i];
}
static char *f (char * (*g) (char **, int), char **p, ...)
{
  char *s;
  va_list v;
  va_start (v,p);
  s = g (p, va_arg (v,int));
  va_end (v);
  return s;
}
int test (int i, double x);
struct s1 {int (*f) (int a);};
struct s2 {int (*f) (double a);};
int pairnames (int, char **, FILE *(*)(struct buf *, struct stat *, int), int, int);
int argc;
char **argv;
], [
return f (e, argv, 0) != argv[0]  ||  f (e, argv, 1) != argv[1];
],
[am_cv_prog_cc_stdc="$ac_arg"; break])
done
CC="$ac_save_CC"
])
if test -z "$am_cv_prog_cc_stdc"; then
  AC_MSG_RESULT([none needed])
else
  AC_MSG_RESULT([$am_cv_prog_cc_stdc])
fi
case "x$am_cv_prog_cc_stdc" in
  x|xno) ;;
  *) CC="$CC $am_cv_prog_cc_stdc" ;;
esac
])

dnl We need opendir, readdir and closedir.  This checks only for
dnl readdir and assumes that opendir and closedir are present if readdir is.
dnl Under plain Win32 (i.e. not Mingw or Cygwin) these functions are not
dnl available, but can be emulated with lib/w_dirent.c.  It's a bit odd to
dnl perform that test here, since "plain Win32" users are unlikely to
dnl run configure... Hence this code only documents when w_dirent.c should
dnl be built.
dnl
dnl Written by Alexandre Duret-Lutz <duret_g@epita.fr>

AC_DEFUN([adl_FUNC_READDIR],
 [AC_CHECK_FUNC([readdir],
  [AC_DEFINE([HAVE_READDIR],1,[Define if you have readdir and friends.])],
   [AC_CHECK_HEADER([windows.h],
    [# build w_dirent if readdir is absent and windows.h exists
     AC_LIBOBJ(w_dirent)],
    [AC_MSG_ERROR([Could not find `readdir'.])])])])

dnl @synopsis AC_FUNC_MKDIR
dnl
dnl Check whether mkdir() is mkdir or _mkdir, and whether it takes one or two
dnl arguments.
dnl
dnl This macro can define HAVE_MKDIR, HAVE__MKDIR, and MKDIR_TAKES_ONE_ARG,
dnl which are expected to be used as follow:
dnl
dnl   #if HAVE_MKDIR
dnl   # if MKDIR_TAKES_ONE_ARG
dnl   	 /* Mingw32 */
dnl   #  define mkdir(a,b) mkdir(a)
dnl   # endif
dnl   #else
dnl   # if HAVE__MKDIR
dnl   	 /* plain Win32 */
dnl   #  define mkdir(a,b) _mkdir(a)
dnl   # else
dnl   #  error "Don't know how to create a directory on this system."
dnl   # endif
dnl   #endif
dnl
dnl @version $Id$
dnl @author Written by Alexandre Duret-Lutz <duret_g@epita.fr>.

AC_DEFUN([AC_FUNC_MKDIR],
[AC_CHECK_FUNCS([mkdir _mkdir])
AC_CACHE_CHECK([whether mkdir takes one argument],
                [ac_cv_mkdir_takes_one_arg],
[AC_TRY_COMPILE([
#include <sys/stat.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif
],[mkdir (".");],
[ac_cv_mkdir_takes_one_arg=yes],[ac_cv_mkdir_takes_one_arg=no])])
if test x"$ac_cv_mkdir_takes_one_arg" = xyes; then
  AC_DEFINE([MKDIR_TAKES_ONE_ARG],1,
            [Define if mkdir takes only one argument.])
fi
])

dnl Note:
dnl =====
dnl I have not implemented the following suggestion because I don't have
dnl access to such a broken environment to test the macro.  So I'm just
dnl appending the comments here in case you have, and want to fix
dnl AC_FUNC_MKDIR that way.
dnl
dnl |Thomas E. Dickey (dickey@herndon4.his.com) said:
dnl |  it doesn't cover the problem areas (compilers that mistreat mkdir
dnl |  may prototype it in dir.h and dirent.h, for instance).
dnl |
dnl |Alexandre:
dnl |  Would it be sufficient to check for these headers and #include
dnl |  them in the AC_TRY_COMPILE block?  (and is AC_HEADER_DIRENT
dnl |  suitable for this?)
dnl |
dnl |Thomas:
dnl |  I think that might be a good starting point (with the set of recommended
dnl |  ifdef's and includes for AC_HEADER_DIRENT, of course).

#serial 5

dnl From Jim Meyering.
dnl Determine whether malloc accepts 0 as its argument.
dnl If it doesn't, arrange to use the replacement function.
dnl

AC_DEFUN([jm_FUNC_MALLOC],
[
 dnl xmalloc.c requires that this symbol be defined so it doesn't
 dnl mistakenly use a broken malloc -- as it might if this test were omitted.
 AC_DEFINE(HAVE_DONE_WORKING_MALLOC_CHECK, 1,
           [Define if the malloc check has been performed. ])

 AC_CACHE_CHECK([for working malloc], jm_cv_func_working_malloc,
  [AC_TRY_RUN([
    char *malloc ();
    int
    main ()
    {
      exit (malloc (0) ? 0 : 1);
    }
	  ],
	 jm_cv_func_working_malloc=yes,
	 jm_cv_func_working_malloc=no,
	 dnl When crosscompiling, assume malloc is broken.
	 jm_cv_func_working_malloc=no)
  ])
  if test $jm_cv_func_working_malloc = no; then
    AC_LIBOBJ(malloc)
    AC_DEFINE(malloc, rpl_malloc,
      [Define to rpl_malloc if the replacement function should be used.])
  fi
])

#serial 5

dnl From Jim Meyering.
dnl Determine whether realloc works when both arguments are 0.
dnl If it doesn't, arrange to use the replacement function.
dnl

AC_DEFUN([jm_FUNC_REALLOC],
[
 dnl xmalloc.c requires that this symbol be defined so it doesn't
 dnl mistakenly use a broken realloc -- as it might if this test were omitted.
 AC_DEFINE(HAVE_DONE_WORKING_REALLOC_CHECK, 1,
           [Define if the realloc check has been performed. ])

 AC_CACHE_CHECK([for working realloc], jm_cv_func_working_realloc,
  [AC_TRY_RUN([
    char *realloc ();
    int
    main ()
    {
      exit (realloc (0, 0) ? 0 : 1);
    }
	  ],
	 jm_cv_func_working_realloc=yes,
	 jm_cv_func_working_realloc=no,
	 dnl When crosscompiling, assume realloc is broken.
	 jm_cv_func_working_realloc=no)
  ])
  if test $jm_cv_func_working_realloc = no; then
    AC_LIBOBJ(realloc)
    AC_DEFINE(realloc, rpl_realloc,
      [Define to rpl_realloc if the replacement function should be used.])
  fi
])

#serial 1003
# Experimental replacement for the function in the latest CVS autoconf.
# Use with the error.c file in ../lib.

# Copyright 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

undefine([AC_FUNC_STRERROR_R])

# AC_FUNC_STRERROR_R
# ------------------
AC_DEFUN([AC_FUNC_STRERROR_R],
[AC_CHECK_DECLS([strerror_r])
AC_CHECK_FUNCS([strerror_r])
AC_CACHE_CHECK([whether strerror_r returns char *],
               ac_cv_func_strerror_r_char_p,
   [
    ac_cv_func_strerror_r_char_p=no
    if test $ac_cv_have_decl_strerror_r = yes; then
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([AC_INCLUDES_DEFAULT],
	[[
	  char buf[100];
	  char x = *strerror_r (0, buf, sizeof buf);
	  char *p = strerror_r (0, buf, sizeof buf);
	]])],
			ac_cv_func_strerror_r_char_p=yes)
    else
      # strerror_r is not declared.  Choose between
      # systems that have relatively inaccessible declarations for the
      # function.  BeOS and DEC UNIX 4.0 fall in this category, but the
      # former has a strerror_r that returns char*, while the latter
      # has a strerror_r that returns `int'.
      # This test should segfault on the DEC system.
      AC_RUN_IFELSE([AC_LANG_PROGRAM([AC_INCLUDES_DEFAULT
	extern char *strerror_r ();],
	[[char buf[100];
	  char x = *strerror_r (0, buf, sizeof buf);
	  exit (!isalpha (x));]])],
                    ac_cv_func_strerror_r_char_p=yes, , :)
    fi
  ])
if test $ac_cv_func_strerror_r_char_p = yes; then
  AC_DEFINE([STRERROR_R_CHAR_P], 1,
	    [Define to 1 if strerror_r returns char *.])
fi
])# AC_FUNC_STRERROR_R

#serial 4

# See if we need to emulate a missing ftruncate function using fcntl or chsize.

AC_DEFUN([jm_FUNC_FTRUNCATE],
[
  AC_CHECK_FUNCS(ftruncate, , [ftruncate_missing=yes])

  if test "$ftruncate_missing" = yes; then
    AC_CHECK_HEADERS([unistd.h])
    AC_CHECK_FUNCS([chsize])
    AC_LIBOBJ(ftruncate)
  fi
])

dnl @synopsis adl_FUNC_GETOPT_LONG
dnl
dnl Check for getopt_long support.
dnl
dnl This assume that the standard getopt.h file (from GNU libc) is
dnl available as lib/gnugetopt.h.  If needed, this file will be linked
dnl as getopt.h, but we want to default to the system's getopt.h file.
dnl (See http://sources.redhat.com/ml/automake/2000-09/msg00041.html
dnl for an explanation about why using the system's getopt.h file
dnl is important.)
dnl
dnl @version $Id$
dnl @author Alexandre Duret-Lutz <duret_g@epita.fr>

AC_PREREQ(2.49)

AC_DEFUN([adl_FUNC_GETOPT_LONG],
 [# clean out junk possibly left behind by a previous configuration
  rm -f lib/getopt.h
  # Check for getopt_long support
  AC_CHECK_HEADER([getopt.h])
  AC_CHECK_FUNCS([getopt_long],,
   [# FreeBSD has a gnugetopt library for this
    AC_CHECK_LIB([gnugetopt],[getopt_long],[AC_DEFINE([HAVE_GETOPT_LONG])],
     [# use the GNU replacement
      AC_LIBOBJ(getopt)
      AC_LIBOBJ(getopt1)
      AC_CONFIG_LINKS([lib/getopt.h:lib/gnugetopt.h])])])])

# Check for functions and headers needed by lib/filelock.c
AC_DEFUN([adl_FILELOCK], [
 AC_CHECK_HEADERS([fcntl.h sys/file.h io.h sys/locking.h])
 AC_CHECK_FUNCS([fcntl flock _locking])
])

dnl @synopsis AC_C_BIGENDIAN_CROSS
dnl
dnl Check endianess even when crosscompiling
dnl (partially based on the original AC_C_BIGENDIAN).
dnl
dnl The implementation will create a binary, and instead of running
dnl the binary it will be grep'ed for some symbols that will look
dnl different for different endianess of the binary.
dnl
dnl @version $Id: ac_c_bigendian_cross.m4,v 1.1 2000/09/09 13:29:23 simons Exp $
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AC_C_BIGENDIAN_CROSS],
[AC_CACHE_CHECK([whether byte ordering is bigendian], [ac_cv_c_bigendian],
[# See if sys/param.h defines the BYTE_ORDER macro.
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/param.h>], [
#if !BYTE_ORDER || !BIG_ENDIAN || !LITTLE_ENDIAN
 bogus endian macros
#endif],
[# It does; now see whether it defined to BIG_ENDIAN or not.
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/param.h>], [
#if BYTE_ORDER != BIG_ENDIAN
 not big endian
#endif], [ac_cv_c_bigendian=yes], [ac_cv_c_bigendian=no])],
[# It does not, try to run a simple test
AC_TRY_RUN([main () {
  /* Are we little or big endian?  From Harbison&Steele.  */
  union
  {
    long l;
    char c[sizeof (long)];
  } u;
  u.l = 1;
  exit (u.c[sizeof (long) - 1] == 1);
}], [ac_cv_c_bigendian=no], [ac_cv_c_bigendian=yes],
[ac_cv_c_bigendian="unknown (cross-compiling)"])
])])

if test "$ac_cv_c_bigendian" = "unknown (cross-compiling)"; then
  AC_MSG_CHECKING([whether byte order LOOKS bigendian])
[cat >conftest.c <<EOF
short ascii_mm[] = { 0x4249, 0x4765, 0x6E44, 0x6961, 0x6E53, 0x7953, 0 };
short ascii_ii[] = { 0x694C, 0x5454, 0x656C, 0x6E45, 0x6944, 0x6E61, 0 };
void _ascii() { char* s = (char*) ascii_mm; s = (char*) ascii_ii; }
short ebcdic_ii[] = { 0x89D3, 0xE3E3, 0x8593, 0x95C5, 0x89C4, 0x9581, 0 };
short ebcdic_mm[] = { 0xC2C9, 0xC785, 0x95C4, 0x8981, 0x95E2, 0xA8E2, 0 };
void _ebcdic() { char* s = (char*) ebcdic_mm; s = (char*) ebcdic_ii; }
int main() { _ascii (); _ebcdic (); return 0; }
EOF
] if test -f conftest.c ; then
     if ${CC-cc} conftest.c -o conftest.o && test -f conftest.o ; then
        if test `grep -l BIGenDianSyS conftest.o` ; then
           ac_cv_c_bigendian=yes
        fi
        if test `grep -l LiTTleEnDian conftest.o` ; then
           ac_cv_c_bigendian=no
        fi
     fi
   fi
   AC_MSG_RESULT([$ac_cv_c_bigendian])
fi

case "$ac_cv_c_bigendian" in
  yes)
    AC_DEFINE([WORDS_BIGENDIAN], 1, [whether byteorder is bigendian])
    BYTEORDER=4321
    ;;
  no)
    BYTEORDER=1234
    ;;
  *)
    AC_MSG_ERROR([unknown endianess - sorry])
    ;;
esac

AC_DEFINE_UNQUOTED([BYTEORDER], [$BYTEORDER], 
[Set to 1234 if the system is little-endian, 4321 if big-endian])
])

#serial 5

# Define some macros required for proper operation of code in lib/*.c
# on MSDOS/Windows systems.

# From Jim Meyering.

AC_DEFUN([jm_AC_DOS],
  [
    AC_CACHE_CHECK([whether system is Windows or MSDOS], [ac_cv_win_or_dos],
      [
        AC_TRY_COMPILE([],
        [#if !defined _WIN32 && !defined __WIN32__ && !defined __MSDOS__
neither MSDOS nor Windows
#endif],
        [ac_cv_win_or_dos=yes],
        [ac_cv_win_or_dos=no])
      ])

    if test x"$ac_cv_win_or_dos" = xyes; then
      ac_fs_accepts_drive_letter_prefix=1
      ac_fs_backslash_is_file_name_separator=1
    else
      ac_fs_accepts_drive_letter_prefix=0
      ac_fs_backslash_is_file_name_separator=0
    fi

    AH_VERBATIM(FILESYSTEM_PREFIX_LEN,
    [#if FILESYSTEM_ACCEPTS_DRIVE_LETTER_PREFIX
# define FILESYSTEM_PREFIX_LEN(Filename) \
  ((Filename)[0] && (Filename)[1] == ':' ? 2 : 0)
#else
# define FILESYSTEM_PREFIX_LEN(Filename) 0
#endif])

    AC_DEFINE_UNQUOTED([FILESYSTEM_ACCEPTS_DRIVE_LETTER_PREFIX],
      $ac_fs_accepts_drive_letter_prefix,
      [Define on systems for which file names may have a so-called
       `drive letter' prefix, define this to compute the length of that
       prefix, including the colon.])

    AH_VERBATIM(ISSLASH,
    [#if FILESYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR
# define ISSLASH(C) ((C) == '/' || (C) == '\\')
#else
# define ISSLASH(C) ((C) == '/')
#endif])

    AC_DEFINE_UNQUOTED([FILESYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR],
      $ac_fs_backslash_is_file_name_separator,
      [Define if the backslash character may also serve as a file name
       component separator.])
  ])

# Macro to add for using GNU gettext.
# Ulrich Drepper <drepper@cygnus.com>, 1995.
#
# This file can be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU Public License
# but which still want to provide support for the GNU gettext functionality.
# Please note that the actual code is *not* freely available.

# serial 9

dnl Usage: AM_WITH_NLS([TOOLSYMBOL], [NEEDSYMBOL], [LIBDIR]).
dnl If TOOLSYMBOL is specified and is 'use-libtool', then a libtool library
dnl    $(top_builddir)/intl/libintl.la will be created (shared and/or static,
dnl    depending on --{enable,disable}-{shared,static} and on the presence of
dnl    AM-DISABLE-SHARED). Otherwise, a static library
dnl    $(top_builddir)/intl/libintl.a will be created.
dnl If NEEDSYMBOL is specified and is 'need-ngettext', then GNU gettext
dnl    implementations (in libc or libintl) without the ngettext() function
dnl    will be ignored.
dnl LIBDIR is used to find the intl libraries.  If empty,
dnl    the value `$(top_builddir)/intl/' is used.
dnl
dnl The result of the configuration is one of three cases:
dnl 1) GNU gettext, as included in the intl subdirectory, will be compiled
dnl    and used.
dnl    Catalog format: GNU --> install in $(datadir)
dnl    Catalog extension: .mo after installation, .gmo in source tree
dnl 2) GNU gettext has been found in the system's C library.
dnl    Catalog format: GNU --> install in $(datadir)
dnl    Catalog extension: .mo after installation, .gmo in source tree
dnl 3) No internationalization, always use English msgid.
dnl    Catalog format: none
dnl    Catalog extension: none
dnl The use of .gmo is historical (it was needed to avoid overwriting the
dnl GNU format catalogs when building on a platform with an X/Open gettext),
dnl but we keep it in order not to force irrelevant filename changes on the
dnl maintainers.
dnl
AC_DEFUN([AM_WITH_NLS],
  [AC_MSG_CHECKING([whether NLS is requested])
    dnl Default is enabled NLS
    AC_ARG_ENABLE(nls,
      [  --disable-nls           do not use Native Language Support],
      USE_NLS=$enableval, USE_NLS=yes)
    AC_MSG_RESULT($USE_NLS)
    AC_SUBST(USE_NLS)

    BUILD_INCLUDED_LIBINTL=no
    USE_INCLUDED_LIBINTL=no
    INTLLIBS=

    dnl If we use NLS figure out what method
    if test "$USE_NLS" = "yes"; then
      AC_DEFINE(ENABLE_NLS, 1,
        [Define to 1 if translation of program messages to the user's native language
   is requested.])
      AC_MSG_CHECKING([whether included gettext is requested])
      AC_ARG_WITH(included-gettext,
        [  --with-included-gettext use the GNU gettext library included here],
        nls_cv_force_use_gnu_gettext=$withval,
        nls_cv_force_use_gnu_gettext=no)
      AC_MSG_RESULT($nls_cv_force_use_gnu_gettext)

      nls_cv_use_gnu_gettext="$nls_cv_force_use_gnu_gettext"
      if test "$nls_cv_force_use_gnu_gettext" != "yes"; then
        dnl User does not insist on using GNU NLS library.  Figure out what
        dnl to use.  If GNU gettext is available we use this.  Else we have
        dnl to fall back to GNU NLS library.
	CATOBJEXT=NONE

        dnl Add a version number to the cache macros.
        define(gt_cv_func_gnugettext_libc, [gt_cv_func_gnugettext]ifelse([$2], need-ngettext, 2, 1)[_libc])
        define(gt_cv_func_gnugettext_libintl, [gt_cv_func_gnugettext]ifelse([$2], need-ngettext, 2, 1)[_libintl])

	AC_CHECK_HEADER(libintl.h,
	  [AC_CACHE_CHECK([for GNU gettext in libc], gt_cv_func_gnugettext_libc,
	    [AC_TRY_LINK([#include <libintl.h>
extern int _nl_msg_cat_cntr;],
	       [bindtextdomain ("", "");
return (int) gettext ("")]ifelse([$2], need-ngettext, [ + (int) ngettext ("", "", 0)], [])[ + _nl_msg_cat_cntr],
	       gt_cv_func_gnugettext_libc=yes,
	       gt_cv_func_gnugettext_libc=no)])

	   if test "$gt_cv_func_gnugettext_libc" != "yes"; then
	     AC_CACHE_CHECK([for GNU gettext in libintl],
	       gt_cv_func_gnugettext_libintl,
	       [gt_save_LIBS="$LIBS"
		LIBS="$LIBS -lintl $LIBICONV"
		AC_TRY_LINK([#include <libintl.h>
extern int _nl_msg_cat_cntr;],
		  [bindtextdomain ("", "");
return (int) gettext ("")]ifelse([$2], need-ngettext, [ + (int) ngettext ("", "", 0)], [])[ + _nl_msg_cat_cntr],
		  gt_cv_func_gnugettext_libintl=yes,
		  gt_cv_func_gnugettext_libintl=no)
		LIBS="$gt_save_LIBS"])
	   fi

	   dnl If an already present or preinstalled GNU gettext() is found,
	   dnl use it.  But if this macro is used in GNU gettext, and GNU
	   dnl gettext is already preinstalled in libintl, we update this
	   dnl libintl.  (Cf. the install rule in intl/Makefile.in.)
	   if test "$gt_cv_func_gnugettext_libc" = "yes" \
	      || { test "$gt_cv_func_gnugettext_libintl" = "yes" \
		   && test "$PACKAGE" != gettext; }; then
	     AC_DEFINE(HAVE_GETTEXT, 1,
               [Define if the GNU gettext() function is already present or preinstalled.])

	     if test "$gt_cv_func_gnugettext_libintl" = "yes"; then
	       dnl If iconv() is in a separate libiconv library, then anyone
	       dnl linking with libintl{.a,.so} also needs to link with
	       dnl libiconv.
	       INTLLIBS="-lintl $LIBICONV"
	     fi

	     gt_save_LIBS="$LIBS"
	     LIBS="$LIBS $INTLLIBS"
	     AC_CHECK_FUNCS(dcgettext)
	     LIBS="$gt_save_LIBS"

	     AM_PATH_PROG_WITH_TEST(MSGFMT, msgfmt,
	       [test -z "`$ac_dir/$ac_word -h 2>&1 | grep 'dv '`"], no)dnl
	     if test "$MSGFMT" != "no"; then
	       AC_PATH_PROG(GMSGFMT, gmsgfmt, $MSGFMT)
	     fi

	     AM_PATH_PROG_WITH_TEST(XGETTEXT, xgettext,
	       [test -z "`$ac_dir/$ac_word -h 2>&1 | grep '(HELP)'`"], :)

	     CATOBJEXT=.gmo
	   fi
	])

        if test "$CATOBJEXT" = "NONE"; then
	  dnl GNU gettext is not found in the C library.
	  dnl Fall back on GNU gettext library.
	  nls_cv_use_gnu_gettext=yes
        fi
      fi

      if test "$nls_cv_use_gnu_gettext" = "yes"; then
        dnl Mark actions used to generate GNU NLS library.
        INTLOBJS="\$(GETTOBJS)"
        AM_PATH_PROG_WITH_TEST(MSGFMT, msgfmt,
	  [test -z "`$ac_dir/$ac_word -h 2>&1 | grep 'dv '`"], msgfmt)
        AC_PATH_PROG(GMSGFMT, gmsgfmt, $MSGFMT)
        AM_PATH_PROG_WITH_TEST(XGETTEXT, xgettext,
	  [test -z "`$ac_dir/$ac_word -h 2>&1 | grep '(HELP)'`"], :)
        AC_SUBST(MSGFMT)
	BUILD_INCLUDED_LIBINTL=yes
	USE_INCLUDED_LIBINTL=yes
        CATOBJEXT=.gmo
	INTLLIBS="ifelse([$3],[],\$(top_builddir)/intl,[$3])/libintl.ifelse([$1], use-libtool, [l], [])a $LIBICONV"
	LIBS=`echo " $LIBS " | sed -e 's/ -lintl / /' -e 's/^ //' -e 's/ $//'`
      fi

      dnl Test whether we really found GNU xgettext.
      if test "$XGETTEXT" != ":"; then
	dnl If it is no GNU xgettext we define it as : so that the
	dnl Makefiles still can work.
	if $XGETTEXT --omit-header /dev/null 2> /dev/null; then
	  : ;
	else
	  AC_MSG_RESULT(
	    [found xgettext program is not GNU xgettext; ignore it])
	  XGETTEXT=":"
	fi
      fi

      dnl We need to process the po/ directory.
      POSUB=po
    fi
    AC_OUTPUT_COMMANDS(
     [for ac_file in $CONFIG_FILES; do
        # Support "outfile[:infile[:infile...]]"
        case "$ac_file" in
          *:*) ac_file=`echo "$ac_file"|sed 's%:.*%%'` ;;
        esac
        # PO directories have a Makefile.in generated from Makefile.in.in.
        case "$ac_file" in */Makefile.in)
          # Adjust a relative srcdir.
          ac_dir=`echo "$ac_file"|sed 's%/[^/][^/]*$%%'`
          ac_dir_suffix="/`echo "$ac_dir"|sed 's%^\./%%'`"
          ac_dots=`echo "$ac_dir_suffix"|sed 's%/[^/]*%../%g'`
          test -n "$ac_given_srcdir" || ac_given_srcdir="$srcdir"
          case "$ac_given_srcdir" in
            .)  top_srcdir=`echo $ac_dots|sed 's%/$%%'` ;;
            /*) top_srcdir="$ac_given_srcdir" ;;
            *)  top_srcdir="$ac_dots$ac_given_srcdir" ;;
          esac
          if test -f "$ac_given_srcdir/$ac_dir/POTFILES.in"; then
            rm -f "$ac_dir/POTFILES"
            test -n "$as_me" && echo "$as_me: creating $ac_dir/POTFILES" || echo "creating $ac_dir/POTFILES"
            sed -e "/^#/d" -e "/^[ 	]*\$/d" -e "s,.*,     $top_srcdir/& \\\\," -e "\$s/\(.*\) \\\\/\1/" < "$ac_given_srcdir/$ac_dir/POTFILES.in" > "$ac_dir/POTFILES"
            test -n "$as_me" && echo "$as_me: creating $ac_dir/Makefile" || echo "creating $ac_dir/Makefile"
            sed -e "/POTFILES =/r $ac_dir/POTFILES" "$ac_dir/Makefile.in" > "$ac_dir/Makefile"
          fi
          ;;
        esac
      done])


    dnl If this is used in GNU gettext we have to set BUILD_INCLUDED_LIBINTL
    dnl to 'yes' because some of the testsuite requires it.
    if test "$PACKAGE" = gettext; then
      BUILD_INCLUDED_LIBINTL=yes
    fi

    dnl intl/plural.c is generated from intl/plural.y. It requires bison,
    dnl because plural.y uses bison specific features. It requires at least
    dnl bison-1.26 because earlier versions generate a plural.c that doesn't
    dnl compile.
    dnl bison is only needed for the maintainer (who touches plural.y). But in
    dnl order to avoid separate Makefiles or --enable-maintainer-mode, we put
    dnl the rule in general Makefile. Now, some people carelessly touch the
    dnl files or have a broken "make" program, hence the plural.c rule will
    dnl sometimes fire. To avoid an error, defines BISON to ":" if it is not
    dnl present or too old.
    AC_CHECK_PROGS([INTLBISON], [bison])
    if test -z "$INTLBISON"; then
      ac_verc_fail=yes
    else
      dnl Found it, now check the version.
      AC_MSG_CHECKING([version of bison])
changequote(<<,>>)dnl
      ac_prog_version=`$INTLBISON --version 2>&1 | sed -n 's/^.*GNU Bison .* \([0-9]*\.[0-9.]*\).*$/\1/p'`
      case $ac_prog_version in
        '') ac_prog_version="v. ?.??, bad"; ac_verc_fail=yes;;
        1.2[6-9]* | 1.[3-9][0-9]* | [2-9].*)
changequote([,])dnl
           ac_prog_version="$ac_prog_version, ok"; ac_verc_fail=no;;
        *) ac_prog_version="$ac_prog_version, bad"; ac_verc_fail=yes;;
      esac
      AC_MSG_RESULT([$ac_prog_version])
    fi
    if test $ac_verc_fail = yes; then
      INTLBISON=:
    fi

    dnl These rules are solely for the distribution goal.  While doing this
    dnl we only have to keep exactly one list of the available catalogs
    dnl in configure.in.
    for lang in $ALL_LINGUAS; do
      GMOFILES="$GMOFILES $lang.gmo"
      POFILES="$POFILES $lang.po"
    done

    dnl Make all variables we use known to autoconf.
    AC_SUBST(BUILD_INCLUDED_LIBINTL)
    AC_SUBST(USE_INCLUDED_LIBINTL)
    AC_SUBST(CATALOGS)
    AC_SUBST(CATOBJEXT)
    AC_SUBST(GMOFILES)
    AC_SUBST(INTLLIBS)
    AC_SUBST(INTLOBJS)
    AC_SUBST(POFILES)
    AC_SUBST(POSUB)

    dnl For backward compatibility. Some configure.ins may be using this.
    nls_cv_header_intl=
    nls_cv_header_libgt=

    dnl For backward compatibility. Some Makefiles may be using this.
    DATADIRNAME=share
    AC_SUBST(DATADIRNAME)

    dnl For backward compatibility. Some Makefiles may be using this.
    INSTOBJEXT=.mo
    AC_SUBST(INSTOBJEXT)

    dnl For backward compatibility. Some Makefiles may be using this.
    GENCAT=gencat
    AC_SUBST(GENCAT)
  ])

dnl Usage: Just like AM_WITH_NLS, which see.
AC_DEFUN([AM_GNU_GETTEXT],
  [AC_REQUIRE([AC_PROG_MAKE_SET])dnl
   AC_REQUIRE([AC_PROG_CC])dnl
   AC_REQUIRE([AC_CANONICAL_HOST])dnl
   AC_REQUIRE([AC_PROG_RANLIB])dnl
   AC_REQUIRE([AC_ISC_POSIX])dnl
   AC_REQUIRE([AC_HEADER_STDC])dnl
   AC_REQUIRE([AC_C_CONST])dnl
   AC_REQUIRE([AC_C_INLINE])dnl
   AC_REQUIRE([AC_TYPE_OFF_T])dnl
   AC_REQUIRE([AC_TYPE_SIZE_T])dnl
   AC_REQUIRE([AC_FUNC_ALLOCA])dnl
   AC_REQUIRE([AC_FUNC_MMAP])dnl
   AC_REQUIRE([jm_GLIBC21])dnl

   AC_CHECK_HEADERS([argz.h limits.h locale.h nl_types.h malloc.h stddef.h \
stdlib.h string.h unistd.h sys/param.h])
   AC_CHECK_FUNCS([feof_unlocked fgets_unlocked getcwd getegid geteuid \
getgid getuid mempcpy munmap putenv setenv setlocale stpcpy strchr strcasecmp \
strdup strtoul tsearch __argz_count __argz_stringify __argz_next])

   AM_ICONV
   AM_LANGINFO_CODESET
   AM_LC_MESSAGES
   AM_WITH_NLS([$1],[$2],[$3])

   if test "x$CATOBJEXT" != "x"; then
     if test "x$ALL_LINGUAS" = "x"; then
       LINGUAS=
     else
       AC_MSG_CHECKING(for catalogs to be installed)
       NEW_LINGUAS=
       for presentlang in $ALL_LINGUAS; do
         useit=no
         for desiredlang in ${LINGUAS-$ALL_LINGUAS}; do
           # Use the presentlang catalog if desiredlang is
           #   a. equal to presentlang, or
           #   b. a variant of presentlang (because in this case,
           #      presentlang can be used as a fallback for messages
           #      which are not translated in the desiredlang catalog).
           case "$desiredlang" in
             "$presentlang"*) useit=yes;;
           esac
         done
         if test $useit = yes; then
           NEW_LINGUAS="$NEW_LINGUAS $presentlang"
         fi
       done
       LINGUAS=$NEW_LINGUAS
       AC_MSG_RESULT($LINGUAS)
     fi

     dnl Construct list of names of catalog files to be constructed.
     if test -n "$LINGUAS"; then
       for lang in $LINGUAS; do CATALOGS="$CATALOGS $lang$CATOBJEXT"; done
     fi
   fi

   dnl If the AC_CONFIG_AUX_DIR macro for autoconf is used we possibly
   dnl find the mkinstalldirs script in another subdir but $(top_srcdir).
   dnl Try to locate is.
   MKINSTALLDIRS=
   if test -n "$ac_aux_dir"; then
     MKINSTALLDIRS="$ac_aux_dir/mkinstalldirs"
   fi
   if test -z "$MKINSTALLDIRS"; then
     MKINSTALLDIRS="\$(top_srcdir)/mkinstalldirs"
   fi
   AC_SUBST(MKINSTALLDIRS)

   dnl Enable libtool support if the surrounding package wishes it.
   INTL_LIBTOOL_SUFFIX_PREFIX=ifelse([$1], use-libtool, [l], [])
   AC_SUBST(INTL_LIBTOOL_SUFFIX_PREFIX)
  ])

# Search path for a program which passes the given test.
# Ulrich Drepper <drepper@cygnus.com>, 1996.
#
# This file can be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU General Public
# License or the GNU Library General Public License but which still want
# to provide support for the GNU gettext functionality.
# Please note that the actual code of the GNU gettext library is covered
# by the GNU Library General Public License, and the rest of the GNU
# gettext package package is covered by the GNU General Public License.
# They are *not* in the public domain.

# serial 2

dnl AM_PATH_PROG_WITH_TEST(VARIABLE, PROG-TO-CHECK-FOR,
dnl   TEST-PERFORMED-ON-FOUND_PROGRAM [, VALUE-IF-NOT-FOUND [, PATH]])
AC_DEFUN([AM_PATH_PROG_WITH_TEST],
[# Extract the first word of "$2", so it can be a program name with args.
set dummy $2; ac_word=[$]2
AC_MSG_CHECKING([for $ac_word])
AC_CACHE_VAL(ac_cv_path_$1,
[case "[$]$1" in
  /*)
  ac_cv_path_$1="[$]$1" # Let the user override the test with a path.
  ;;
  *)
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in ifelse([$5], , $PATH, [$5]); do
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/$ac_word; then
      if [$3]; then
	ac_cv_path_$1="$ac_dir/$ac_word"
	break
      fi
    fi
  done
  IFS="$ac_save_ifs"
dnl If no 4th arg is given, leave the cache variable unset,
dnl so AC_PATH_PROGS will keep looking.
ifelse([$4], , , [  test -z "[$]ac_cv_path_$1" && ac_cv_path_$1="$4"
])dnl
  ;;
esac])dnl
$1="$ac_cv_path_$1"
if test ifelse([$4], , [-n "[$]$1"], ["[$]$1" != "$4"]); then
  AC_MSG_RESULT([$]$1)
else
  AC_MSG_RESULT(no)
fi
AC_SUBST($1)dnl
])

#serial 2

# Test for the GNU C Library, version 2.1 or newer.
# From Bruno Haible.

AC_DEFUN([jm_GLIBC21],
  [
    AC_CACHE_CHECK(whether we are using the GNU C Library 2.1 or newer,
      ac_cv_gnu_library_2_1,
      [AC_EGREP_CPP([Lucky GNU user],
	[
#include <features.h>
#ifdef __GNU_LIBRARY__
 #if (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 1) || (__GLIBC__ > 2)
  Lucky GNU user
 #endif
#endif
	],
	ac_cv_gnu_library_2_1=yes,
	ac_cv_gnu_library_2_1=no)
      ]
    )
    AC_SUBST(GLIBC21)
    GLIBC21="$ac_cv_gnu_library_2_1"
  ]
)

#serial AM2

dnl From Bruno Haible.

AC_DEFUN([AM_ICONV],
[
  dnl Some systems have iconv in libc, some have it in libiconv (OSF/1 and
  dnl those with the standalone portable GNU libiconv installed).

  AC_ARG_WITH([libiconv-prefix],
[  --with-libiconv-prefix=DIR  search for libiconv in DIR/include and DIR/lib], [
    for dir in `echo "$withval" | tr : ' '`; do
      if test -d $dir/include; then CPPFLAGS="$CPPFLAGS -I$dir/include"; fi
      if test -d $dir/lib; then LDFLAGS="$LDFLAGS -L$dir/lib"; fi
    done
   ])

  AC_CACHE_CHECK(for iconv, am_cv_func_iconv, [
    am_cv_func_iconv="no, consider installing GNU libiconv"
    am_cv_lib_iconv=no
    AC_TRY_LINK([#include <stdlib.h>
#include <iconv.h>],
      [iconv_t cd = iconv_open("","");
       iconv(cd,NULL,NULL,NULL,NULL);
       iconv_close(cd);],
      am_cv_func_iconv=yes)
    if test "$am_cv_func_iconv" != yes; then
      am_save_LIBS="$LIBS"
      LIBS="$LIBS -liconv"
      AC_TRY_LINK([#include <stdlib.h>
#include <iconv.h>],
        [iconv_t cd = iconv_open("","");
         iconv(cd,NULL,NULL,NULL,NULL);
         iconv_close(cd);],
        am_cv_lib_iconv=yes
        am_cv_func_iconv=yes)
      LIBS="$am_save_LIBS"
    fi
  ])
  if test "$am_cv_func_iconv" = yes; then
    AC_DEFINE(HAVE_ICONV, 1, [Define if you have the iconv() function.])
    AC_MSG_CHECKING([for iconv declaration])
    AC_CACHE_VAL(am_cv_proto_iconv, [
      AC_TRY_COMPILE([
#include <stdlib.h>
#include <iconv.h>
extern
#ifdef __cplusplus
"C"
#endif
#if defined(__STDC__) || defined(__cplusplus)
size_t iconv (iconv_t cd, char * *inbuf, size_t *inbytesleft, char * *outbuf, size_t *outbytesleft);
#else
size_t iconv();
#endif
], [], am_cv_proto_iconv_arg1="", am_cv_proto_iconv_arg1="const")
      am_cv_proto_iconv="extern size_t iconv (iconv_t cd, $am_cv_proto_iconv_arg1 char * *inbuf, size_t *inbytesleft, char * *outbuf, size_t *outbytesleft);"])
    am_cv_proto_iconv=`echo "[$]am_cv_proto_iconv" | tr -s ' ' | sed -e 's/( /(/'`
    AC_MSG_RESULT([$]{ac_t:-
         }[$]am_cv_proto_iconv)
    AC_DEFINE_UNQUOTED(ICONV_CONST, $am_cv_proto_iconv_arg1,
      [Define as const if the declaration of iconv() needs const.])
  fi
  LIBICONV=
  if test "$am_cv_lib_iconv" = yes; then
    LIBICONV="-liconv"
  fi
  AC_SUBST(LIBICONV)
])

#serial AM1

dnl From Bruno Haible.

AC_DEFUN([AM_LANGINFO_CODESET],
[
  AC_CACHE_CHECK([for nl_langinfo and CODESET], am_cv_langinfo_codeset,
    [AC_TRY_LINK([#include <langinfo.h>],
      [char* cs = nl_langinfo(CODESET);],
      am_cv_langinfo_codeset=yes,
      am_cv_langinfo_codeset=no)
    ])
  if test $am_cv_langinfo_codeset = yes; then
    AC_DEFINE(HAVE_LANGINFO_CODESET, 1,
      [Define if you have <langinfo.h> and nl_langinfo(CODESET).])
  fi
])

# Check whether LC_MESSAGES is available in <locale.h>.
# Ulrich Drepper <drepper@cygnus.com>, 1995.
#
# This file can be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU General Public
# License or the GNU Library General Public License but which still want
# to provide support for the GNU gettext functionality.
# Please note that the actual code of the GNU gettext library is covered
# by the GNU Library General Public License, and the rest of the GNU
# gettext package package is covered by the GNU General Public License.
# They are *not* in the public domain.

# serial 2

AC_DEFUN([AM_LC_MESSAGES],
  [if test $ac_cv_header_locale_h = yes; then
    AC_CACHE_CHECK([for LC_MESSAGES], am_cv_val_LC_MESSAGES,
      [AC_TRY_LINK([#include <locale.h>], [return LC_MESSAGES],
       am_cv_val_LC_MESSAGES=yes, am_cv_val_LC_MESSAGES=no)])
    if test $am_cv_val_LC_MESSAGES = yes; then
      AC_DEFINE(HAVE_LC_MESSAGES, 1,
        [Define if your <locale.h> file defines LC_MESSAGES.])
    fi
  fi])

AC_DEFUN([adl_ENABLE_DEVEL],
[AC_ARG_ENABLE([devel],
	       [AC_HELP_STRING([--enable-devel],
			       [turn on useful developer options])])
 if test x"$enable_devel" = xyes; then
   enable_debug=${enable_debug-yes}
   enable_warnings=${enable_warnings-yes}
   enable_assert=${enable_assert-yes}
   enable_optimizations=${enable_optimizations--O}
 fi
])

AC_DEFUN([adl_NDEBUG],
 [AC_ARG_ENABLE([assert],
  [AC_HELP_STRING([--enable-assert],[turn on assertions])])
  if test "$enable_assert" != yes; then
    CFLAGS="$CFLAGS -DNDEBUG"
  fi])

AC_DEFUN([adl_ENABLE_GPROF],
 [AC_ARG_ENABLE([gprof],
  [AC_HELP_STRING([--enable-gprof],[enable gprof profiling])])
  if test "${enable_gprof}" = "yes" -a -n "$GCC"; then
   CFLAGS="$CFLAGS -pg"
  fi])

AC_DEFUN([adl_ENABLE_DEBUG],
 [AC_ARG_ENABLE([debug],
  [AC_HELP_STRING([--enable-debug],[enable debugging symbols])])
  case "${enable_debug}" in
    yes)
      AC_DEFINE([DEBUG],1,[Define if you want debugging code.])
      if test -n "$GCC"; then
        CFLAGS="$CFLAGS -ggdb3"
      else
        if test "${ac_cv_prog_cc_g}" = yes; then
          CFLAGS="$CFLAGS -g"
        fi
      fi
      ;;
    no)
      ;;
    *)
      if test "${ac_cv_prog_cc_g}" = yes; then
        CFLAGS="$CFLAGS -g"
      fi
      ;;
  esac])

dnl  Adapted from Akim Demaille <akim@epita.fr> ad_GCC_WARNINGS.
AC_DEFUN([ad_GCC_OPTIM],
[AC_ARG_ENABLE([optimizations],
 [AC_HELP_STRING([--disable-optimizations],
  [turn off aggressive optimizations])])
 if test -n "$GCC" -a "${enable_optimizations-yes}" = "yes"; then
  AC_CACHE_CHECK([for gcc optimization options], ac_cv_prog_gcc_opt_flags,
  [changequote(,)dnl
  cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[]) { return argv[argc-1] == 0; }
EOF
  changequote([,])dnl
  cf_save_CFLAGS="$CFLAGS"
  ac_cv_prog_gcc_opt_flags="-O3"
  for cf_opt in \
    ffast-math \
    fstrict-aliasing \
    fomit-frame-pointer
  do
    CFLAGS="$cf_save_CFLAGS $ac_cv_prog_gcc_opt_flags -$cf_opt"
    if AC_TRY_EVAL([ac_compile]); then
      ac_cv_prog_gcc_opt_flags="$ac_cv_prog_gcc_opt_flags -$cf_opt"
    fi
  done
  rm -f conftest*
  CFLAGS="$cf_save_CFLAGS $ac_cv_prog_gcc_opt_flags"])
else
  case $enable_optimizations in
    no) ;;
    *) CFLAGS="$CFLAGS $enable_optimizations" ;;
  esac
fi])

dnl Check if the compiler supports useful warning options.  There's a few that
dnl we don't use, simply because they're too noisy:
dnl
dnl -ansi (prevents declaration of functions like strdup, and because
dnl        it makes warning in system headers).
dnl -Wconversion (useful in older versions of gcc, but not in gcc 2.7.x)
dnl -Wtraditional (combines too many unrelated messages, only a few useful)
dnl -Wredundant-decls (system headers make this too noisy)
dnl -pedantic
dnl

AC_DEFUN([ad_GCC_WARNINGS],
[AC_ARG_ENABLE([warnings],
[AC_HELP_STRING([--enable-warnings], [enable compiler warnings])])
 if test "$enable_warnings" = "yes"; then
   CF_GCC_WARNINGS
   CF_GCC_MORE_WARNINGS
   AC_SUBST([MORE_WARNINGS_CFLAGS])
 fi])

AC_DEFUN([CF_GCC_WARNINGS],
[if test -n "$GCC"; then
  AC_CACHE_CHECK([for gcc warning options], ac_cv_prog_gcc_warn_flags,
  [changequote(,)dnl
  cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[]) { return argv[argc-1] == 0; }
EOF
  changequote([,])dnl
  cf_save_CFLAGS="$CFLAGS"
  ac_cv_prog_gcc_warn_flags="-W -Wall"
  for cf_opt in \
   Waggregate-return \
   Wbad-function-cast \
   Wcast-align \
   Winline \
   Wnested-externs \
   Wpointer-arith \
   Wwrite-strings
  do
    CFLAGS="$cf_save_CFLAGS $ac_cv_prog_gcc_warn_flags -$cf_opt"
    if AC_TRY_EVAL(ac_compile); then
      ac_cv_prog_gcc_warn_flags="$ac_cv_prog_gcc_warn_flags -$cf_opt"
      test "$cf_opt" = Wcast-qual && ac_cv_prog_gcc_warn_flags="$ac_cv_prog_gcc_warn_flags -DXTSTRINGDEFINES"
    fi
  done
  rm -f conftest*
  CFLAGS="$cf_save_CFLAGS"])
fi
CFLAGS="${CFLAGS} ${ac_cv_prog_gcc_warn_flags}"
])dnl

AC_DEFUN([CF_GCC_MORE_WARNINGS],
[if test -n "$GCC"; then
  AC_CACHE_CHECK([for more gcc warning options], ac_cv_prog_gcc_more_warn_flags,
  [changequote(,)dnl
  cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[]) { return argv[argc-1] == 0; }
EOF
  changequote([,])dnl
  cf_save_CFLAGS="$CFLAGS"
  ac_cv_prog_gcc_more_warn_flags=''
  for cf_opt in \
   Wmissing-declarations \
   Wmissing-prototypes \
   Wstrict-prototypes \
   Wshadow \
   Wcast-qual \
   Werror
  do
    CFLAGS="$cf_save_CFLAGS $ac_cv_prog_gcc_more_warn_flags -$cf_opt"
    if AC_TRY_EVAL(ac_compile); then
      ac_cv_prog_gcc_more_warn_flags="$ac_cv_prog_gcc_more_warn_flags -$cf_opt"
      test "$cf_opt" = Wcast-qual && ac_cv_prog_gcc_more_warn_flags="$ac_cv_prog_gcc_more_warn_flags -DXTSTRINGDEFINES"
    fi
  done
  rm -f conftest*
  CFLAGS="$cf_save_CFLAGS"])
fi
MORE_WARNINGS_CFLAGS="${ac_cv_prog_gcc_more_warn_flags}"
])dnl

AC_DEFUN([heroes_COMPUTE_PATHS],
[AC_REQUIRE([AM_WITH_NLS])dnl

localedir='${datadir}'/locale

pkgdatadir="$datadir/heroes"
adl_COMPUTE_RELATIVE_PATHS([dnl
bindir:prefix:backward_relative_bindir dnl
prefix:pkgdatadir:forward_relative_pkgdatadir dnl
prefix:localedir:forward_relative_localedir dnl
])

AC_DEFINE_UNQUOTED([BACKWARD_RELATIVE_BINDIR],
                   ["$backward_relative_bindir"],
                   [Relative path from BINDIR to PREFIX.])
AC_DEFINE_UNQUOTED([FORWARD_RELATIVE_PKGDATADIR],
                   ["$forward_relative_pkgdatadir"],
                   [Relative path from PREFIX to PKGDATADIR.])
AC_DEFINE_UNQUOTED([FORWARD_RELATIVE_LOCALEDIR],
                   ["$forward_relative_localedir"],
                   [Relative path from PREFIX to LOCALEDIR.])
AC_DEFINE_DIR([PREFIX],[prefix],[Installation prefix path.])
])

dnl @synopsis adl_COMPUTE_RELATIVE_PATHS(PATH_LIST)
dnl
dnl PATH_LIST is a space-separated list of colon-separated
dnl triplets of the form 'FROM:TO:RESULT'.  This function
dnl iterates over these triplets and set $RESULT to the
dnl relative path from $FROM to $TO.  Note that $FROM
dnl and $TO needs to be absolute filenames for this macro
dnl to success.
dnl
dnl For instance,
dnl
dnl    first=/usr/local/bin
dnl    second=/usr/local/share
dnl    adl_COMPUTE_RELATIVE_PATHS([first:second:fs second:first:sf])
dnl    # $fs is set to ../share
dnl    # $sf is set to ../bin
dnl
dnl $FROM and $TO are both eval'ed recursively and normalized,
dnl this means that you can call this macro with autoconf's dirnames
dnl like `prefix' or `datadir'.  For example:
dnl
dnl    adl_COMPUTE_RELATIVE_PATHS([bindir:datadir:bin_to_data])
dnl
dnl adl_COMPUTE_RELATIVE_PATHS should also works with DOS filenames.
dnl
dnl You may want to use this macro in order to make your package
dnl relocatable.  Instead of hardcoding $datadir into your programs
dnl just encode $bin_to_data and try to determine $bindir at run-time.
dnl
dnl This macro requires adl_NORMALIZE_PATH.
dnl
dnl @author Alexandre Duret-Lutz <duret_g@epita.fr>
dnl @version $Id: $
AC_DEFUN([adl_COMPUTE_RELATIVE_PATHS],
[for _lcl_i in $1; do
  _lcl_from=\[$]`echo "[$]_lcl_i" | sed 's,:.*$,,'`
  _lcl_to=\[$]`echo "[$]_lcl_i" | sed 's,^[[^:]]*:,,' | sed 's,:[[^:]]*$,,'`
  _lcl_result_var=`echo "[$]_lcl_i" | sed 's,^.*:,,'`
  adl_RECURSIVE_EVAL([[$]_lcl_from], [_lcl_from])
  adl_RECURSIVE_EVAL([[$]_lcl_to], [_lcl_to])
  _lcl_notation="$_lcl_from$_lcl_to"
  adl_NORMALIZE_PATH([_lcl_from],['/'])
  adl_NORMALIZE_PATH([_lcl_to],['/'])
  adl_COMPUTE_RELATIVE_PATH([_lcl_from], [_lcl_to], [_lcl_result_tmp])
  adl_NORMALIZE_PATH([_lcl_result_tmp],["[$]_lcl_notation"])
  eval $_lcl_result_var='[$]_lcl_result_tmp'
done])


dnl adl_COMPUTE_RELATIVE_PATH(FROM, TO, RESULT)
dnl ===========================================
dnl Compute the relative path to go from $FROM to $TO and set the value
dnl of $RESULT to that value.  This function work on raw filenames
dnl (for instance it will consider /usr//local and /usr/local as
dnl two distinct paths), you should really use adl_COMPUTE_REALTIVE_PATHS
dnl instead to have the paths sanitized automatically.
dnl
dnl For instance:
dnl    first_dir=/somewhere/on/my/disk/bin
dnl    second_dir=/somewhere/on/another/disk/share
dnl    adl_COMPUTE_RELATIVE_PATH(first_dir, second_dir, first_to_second)
dnl will set $first_to_second to '../../../another/disk/share'.
AC_DEFUN([adl_COMPUTE_RELATIVE_PATH],
[adl_COMPUTE_COMMON_PATH([$1], [$2], [_lcl_common_prefix])
adl_COMPUTE_BACK_PATH([$1], [_lcl_common_prefix], [_lcl_first_rel])
adl_COMPUTE_SUFFIX_PATH([$2], [_lcl_common_prefix], [_lcl_second_suffix])
$3="[$]_lcl_first_rel[$]_lcl_second_suffix"])

dnl adl_COMPUTE_COMMON_PATH(LEFT, RIGHT, RESULT)
dnl ============================================
dnl Compute the common path to $LEFT and $RIGHT and set the result to $RESULT.
dnl
dnl For instance:
dnl    first_path=/somewhere/on/my/disk/bin
dnl    second_path=/somewhere/on/another/disk/share
dnl    adl_COMPUTE_COMMON_PATH(first_path, second_path, common_path)
dnl will set $common_path to '/somewhere/on'.
AC_DEFUN([adl_COMPUTE_COMMON_PATH],
[$3=''
_lcl_second_prefix_match=''
while test "[$]_lcl_second_prefix_match" != 0; do
  _lcl_first_prefix=`expr "x[$]$1" : "x\([$]$3/*[[^/]]*\)"`
  _lcl_second_prefix_match=`expr "x[$]$2" : "x[$]_lcl_first_prefix"`
  if test "[$]_lcl_second_prefix_match" != 0; then
    if test "[$]_lcl_first_prefix" != "[$]$3"; then
      $3="[$]_lcl_first_prefix"
    else
      _lcl_second_prefix_match=0
    fi
  fi
done])

dnl adl_COMPUTE_SUFFIX_PATH(PATH, SUBPATH, RESULT)
dnl ==============================================
dnl Substrack $SUBPATH from $PATH, and set the resulting suffix
dnl (or the empty string if $SUBPATH is not a subpath of $PATH)
dnl to $RESULT.
dnl
dnl For instance:
dnl    first_path=/somewhere/on/my/disk/bin
dnl    second_path=/somewhere/on
dnl    adl_COMPUTE_SUFFIX_PATH(first_path, second_path, common_path)
dnl will set $common_path to '/my/disk/bin'.
AC_DEFUN([adl_COMPUTE_SUFFIX_PATH],
[$3=`expr "x[$]$1" : "x[$]$2/*\(.*\)"`])

dnl adl_COMPUTE_BACK_PATH(PATH, SUBPATH, RESULT)
dnl ============================================
dnl Compute the relative path to go from $PATH to $SUBPATH, knowing that
dnl $SUBPATH is a subpath of $PATH (any other words, only repeated '../'
dnl should be needed to move from $PATH to $SUBPATH) and set the value
dnl of $RESULT to that value.  If $SUBPATH is not a subpath of PATH,
dnl set $RESULT to the empty string.
dnl
dnl For instance:
dnl    first_path=/somewhere/on/my/disk/bin
dnl    second_path=/somewhere/on
dnl    adl_COMPUTE_BACK_PATH(first_path, second_path, back_path)
dnl will set $back_path to '../../../'.
AC_DEFUN([adl_COMPUTE_BACK_PATH],
[adl_COMPUTE_SUFFIX_PATH([$1], [$2], [_lcl_first_suffix])
$3=''
_lcl_tmp='xxx'
while test "[$]_lcl_tmp" != ''; do
  _lcl_tmp=`expr "x[$]_lcl_first_suffix" : "x[[^/]]*/*\(.*\)"`
  if test "[$]_lcl_first_suffix" != ''; then
     _lcl_first_suffix="[$]_lcl_tmp"
     $3="../[$]$3"
  fi
done])


dnl adl_RECURSIVE_EVAL(VALUE, RESULT)
dnl =================================
dnl Interpolate the VALUE in loop until it doesn't change,
dnl and set the result to $RESULT.
dnl WARNING: It's easy to get an infinite loop with some unsane input.
AC_DEFUN([adl_RECURSIVE_EVAL],
[_lcl_receval="$1"
$2=`(test "x$prefix" = xNONE && prefix="$ac_default_prefix"
     test "x$exec_prefix" = xNONE && exec_prefix="${prefix}"
     _lcl_receval_old=''
     while test "[$]_lcl_receval_old" != "[$]_lcl_receval"; do
       _lcl_receval_old="[$]_lcl_receval"
       eval _lcl_receval="\"[$]_lcl_receval\""
     done
     echo "[$]_lcl_receval")`])

dnl @synopsis adl_NORMALIZE_PATH(VARNAME, [REFERENCE_STRING])
dnl
dnl Perform some cleanups on the value of $VARNAME (interpreted as a path):
dnl   - empty paths are changed to '.'
dnl   - trailing slashes are removed
dnl   - repeated slashes are squeezed except a leading doubled slash '//'
dnl     (which might indicate a networked disk on some OS).
dnl
dnl REFERENCE_STRING is used to turn '/' into '\' and vice-versa:
dnl if REFERENCE_STRING contains some backslashes, all slashes and backslashes
dnl are turned into backslashes, otherwise they are all turned into slashes.
dnl
dnl This makes processing of DOS filenames quite easier, because you
dnl can turn a filename to the Unix notation, make your processing, and
dnl turn it back to original notation.
dnl
dnl   filename='A:\FOO\\BAR\'
dnl   old_filename="$filename"
dnl   # Switch to the unix notation
dnl   adl_NORMALIZE_PATH([filename], ["/"])
dnl   # now we have $filename = 'A:/FOO/BAR' and we can process it as if
dnl   # it was a Unix path.  For instance let's say that you want
dnl   # to append '/subpath':
dnl   filename="$filename/subpath"
dnl   # finally switch back to the original notation
dnl   adl_NORMALIZE_PATH([filename], ["$old_filename"])
dnl   # now $filename equals to 'A:\FOO\BAR\subpath'
dnl
dnl One good reason to make all path processing with the unix convention
dnl is that backslashes have a special meaning in many cases.  For instance
dnl
dnl   expr 'A:\FOO' : 'A:\Foo'
dnl
dnl will return 0 because the second argument is a regex in which
dnl backslashes have to be backslashed.  In other words, to have the
dnl two strings to match you should write this instead:
dnl
dnl   expr 'A:\Foo' : 'A:\\Foo'
dnl
dnl Such behavior makes DOS filenames extremely unpleasant to work with.
dnl So temporary turn your paths to the Unix notation, and revert
dnl them to the original notation after the processing.  See the
dnl macro adl_COMPUTE_RELATIVE_PATHS for a concrete example of this.
dnl
dnl REFERENCE_STRING defaults to $VARIABLE, this means that slashes
dnl will be converted to backslashes if $VARIABLE already contains
dnl some backslashes (see $thirddir below).
dnl
dnl   firstdir='/usr/local//share'
dnl   seconddir='C:\Program Files\\'
dnl   thirddir='C:\home/usr/'
dnl   adl_NORMALIZE_PATH([firstdir])
dnl   adl_NORMALIZE_PATH([seconddir])
dnl   adl_NORMALIZE_PATH([thirddir])
dnl   # $firstdir = '/usr/local/share'
dnl   # $seconddir = 'C:\Program Files'
dnl   # $thirddir = 'C:\home\usr'
dnl
dnl @author Alexandre Duret-Lutz <duret_g@epita.fr>
dnl @version $Id: $
AC_DEFUN([adl_NORMALIZE_PATH],
[case ":[$]$1:" in
# change empty paths to '.'
  ::) $1='.' ;;
# strip trailing slashes
  :*[[\\/]]:) $1=`echo "[$]$1" | sed 's,[[\\/]]*[$],,'` ;;
  :*:) ;;
esac
# squeze repeated slashes
case ifelse($2,,"[$]$1",$2) in
# if the path contains any backslashes, turn slashes into backslashes
 *\\*) $1=`echo "[$]$1" | sed 's,\(.\)[[\\/]][[\\/]]*,\1\\\\,g'` ;;
# if the path contains slashes, also turn backslashes into slashes
 *) $1=`echo "[$]$1" | sed 's,\(.\)[[\\/]][[\\/]]*,\1/,g'` ;;
esac])

dnl @synopsis AC_DEFINE_DIR(VARNAME, DIR [, DESCRIPTION])
dnl
dnl This macro defines (with AC_DEFINE) VARNAME to the expansion of the DIR
dnl variable, taking care of fixing up ${prefix} and such.
dnl
dnl Note that the 3 argument form is only supported with autoconf 2.13 and
dnl later (i.e. only where AC_DEFINE supports 3 arguments).
dnl
dnl Examples:
dnl
dnl    AC_DEFINE_DIR(DATADIR, datadir)
dnl    AC_DEFINE_DIR(PROG_PATH, bindir, [Location of installed binaries])
dnl
dnl @version $Id: ac_define_dir.m4,v 1.2 2000/07/19 13:03:32 simons Exp $
dnl @author Alexandre Oliva <oliva@lsd.ic.unicamp.br>

AC_DEFUN([AC_DEFINE_DIR], [
	ac_expanded=`(
	    test "x$prefix" = xNONE && prefix="$ac_default_prefix"
	    test "x$exec_prefix" = xNONE && exec_prefix="${prefix}"
	    eval echo \""[$]$2"\"
        )`
	ifelse($3, ,
	  AC_DEFINE_UNQUOTED($1, "$ac_expanded"),
	  AC_DEFINE_UNQUOTED($1, "$ac_expanded", $3))
])

dnl Handle the --enable-html-doc option.
dnl
dnl The following snippet goes into doc/Makefile.am
dnl ------------------------
dnl CLEANFILES = yourfile.html
dnl
dnl if MAKE_HTML
dnl hdir = @htmldir@
dnl h_DATA = yourfile.html
dnl endif
dnl
dnl SUFFIXES = .html
dnl
dnl .texi.html:
dnl 	    @cd $(srcdir) && rm -f $@ $@-[0-9] $@-[0-9][0-9]
dnl 	    cd $(srcdir) && $(MAKEINFO) --html `echo $< | sed 's,.*/,,'`
dnl ------------------------
dnl
dnl Written by Alexandre Duret-Lutz <duret_g@epita.fr>

AC_DEFUN([adl_ENABLE_HTML_DOC],
 [AC_ARG_ENABLE([html-doc],
                [AC_HELP_STRING([--enable-html-doc=DIR],
                                [build and install html documentation])])
  if test "${enable_html_doc-no}" != no; then
    if test "x$enable_html_doc" = xyes; then
      htmldir="$data_dir/doc"
    else
      htmldir="$enable_html_doc"
    fi
    AC_SUBST([htmldir])
  else
    htmldir='\<none\>'
  fi
  AM_CONDITIONAL(MAKE_HTML, [test "${enable_html_doc-no}" != no])])

AC_DEFUN([heroes_DEBUG],
[AC_ARG_ENABLE([heroes-debug],
  [AC_HELP_STRING([--disable-heroes-debug],[disable tracing facility])])

  if test "x$enable_heroes_debug" != xno; then
    AC_DEFINE([USE_HEROES_DEBUG],1,[Define if you want tracing code.])
  fi])

# Like AC_CONFIG_HEADER, but automatically create stamp file. -*- Autoconf -*-

# Copyright 1996, 1997, 2000, 2001 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

AC_PREREQ([2.52])

# serial 6

# When config.status generates a header, we must update the stamp-h file.
# This file resides in the same directory as the config header
# that is generated.  We must strip everything past the first ":",
# and everything past the last "/".

# _AM_DIRNAME(PATH)
# -----------------
# Like AS_DIRNAME, only do it during macro expansion
AC_DEFUN([_AM_DIRNAME],
       [m4_if(regexp([$1], [^.*[^/]//*[^/][^/]*/*$]), -1,
	      m4_if(regexp([$1], [^//\([^/]\|$\)]), -1,
		    m4_if(regexp([$1], [^/.*]), -1,
			  [.],
			  patsubst([$1], [^\(/\).*], [\1])),
		    patsubst([$1], [^\(//\)\([^/].*\|$\)], [\1])),
	      patsubst([$1], [^\(.*[^/]\)//*[^/][^/]*/*$], [\1]))[]dnl
])# _AM_DIRNAME


# The stamp files are numbered to have different names.
# We could number them on a directory basis, but that's additional
# complications, let's have a unique counter.
m4_define([_AM_STAMP_Count], [0])


# _AM_STAMP(HEADER)
# -----------------
# The name of the stamp file for HEADER.
AC_DEFUN([_AM_STAMP],
[m4_define([_AM_STAMP_Count], m4_incr(_AM_STAMP_Count))dnl
AS_ESCAPE(_AM_DIRNAME(patsubst([$1],
                               [:.*])))/stamp-h[]_AM_STAMP_Count])


# _AM_CONFIG_HEADER(HEADER[:SOURCES], COMMANDS, INIT-COMMANDS)
# ------------------------------------------------------------
# We used to try to get a real timestamp in stamp-h.  But the fear is that
# that will cause unnecessary cvs conflicts.
AC_DEFUN([_AM_CONFIG_HEADER],
[# Add the stamp file to the list of files AC keeps track of,
# along with our hook.
AC_CONFIG_HEADERS([$1],
                  [# update the timestamp
echo 'timestamp for $1' >"_AM_STAMP([$1])"
$2],
                  [$3])
])# _AM_CONFIG_HEADER


# AM_CONFIG_HEADER(HEADER[:SOURCES]..., COMMANDS, INIT-COMMANDS)
# --------------------------------------------------------------
AC_DEFUN([AM_CONFIG_HEADER],
[AC_FOREACH([_AM_File], [$1], [_AM_CONFIG_HEADER(_AM_File, [$2], [$3])])
])# AM_CONFIG_HEADER

