dnl Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>
dnl
dnl This file is part of Heroes.
dnl
dnl Heroes is free software; you can redistribute it and/or modify it under
dnl the terms of the GNU General Public License version 2 as published by
dnl the Free Software Foundation.
dnl
dnl Heroes is distributed in the hope that it will be useful, but WITHOUT
dnl ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
dnl FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
dnl for more details.
dnl
dnl You should have received a copy of the GNU General Public License along
dnl with this program; if not, write to the Free Software Foundation, Inc.,
dnl 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
dnl

AC_PREREQ(2.50)

AC_INIT([heroes], [0.21], [heroes-bugs@lists.sourceforge.net])
AC_CONFIG_AUX_DIR(tools)
AC_CONFIG_SRCDIR([src/heroes.c])
AM_INIT_AUTOMAKE([heroes], [0.21])

AC_COPYRIGHT([Copyright 2000, 2001, 2002  Alexandre Duret-Lutz])

heroes_CHECK_OBSOLETE_OPTIONS

AC_CANONICAL_HOST
AC_DEFINE_UNQUOTED([HOST],["$host"],[The canonical host])

# If the user didn't supplied a CFLAGS value,
# set an empty one to prevent autoconf to stick -O2 -g here.
test -z "$CFLAGS" && CFLAGS=""

# ===================
# Check for programs.
# ===================

AC_PROG_CC
AC_ISC_POSIX
AC_PROG_CC_STDC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_AWK
AC_PROG_RANLIB
AC_PROG_LN_S

AM_MISSING_PROG([HELP2MAN], [help2man])
AM_MISSING_PROG([ZIP], [zip])
AM_MISSING_PROG([UNZIP], [unzip])
AM_MISSING_PROG([AUTOGEN], [autogen])

AC_PATH_PROG([GPERF], [gperf])
AM_CONDITIONAL(HAVE_GPERF, [test \! -z "$GPERF"])

AC_CACHE_SAVE

# Set _GNU_SOURCE before any other tests.
AC__GNU_SOURCE

# =====================
# Checks for libraries.
# =====================

dnl pass a third argument to AC_CHECK_LIB to
dnl prevent HAVE_LIBM from being defined (we don't need it).
AC_CHECK_LIB([m], [sin], [LIBS="-lm $LIBS"])
heroes_MEDIALIB_SELECTION

# ================================
# Check for any malloc() debugger.
# ================================

adl_WITH_MPATROL
adl_WITH_MTRACE
adl_WITH_DMALLOC
adl_WITH_EFENCE
adl_ENABLE_MEM_DEBUG

# ==============================================
# Now that we have all the libraries, check that
# compiled programs are still runnable.
# ==============================================

adl_RUN_CHECK

# ========================
# Checks for header files.
# ========================

AC_HEADER_STDC
AC_HEADER_DIRENT
AC_CHECK_HEADERS(
 [direct.h] dnl
 [io.h] dnl
 [memory.h] dnl
 [stdbool.h] dnl
 [stdint.h] dnl
 [string.h] dnl
 [strings.h] dnl
 [stdlib.h] dnl
 [sys/time.h] dnl
 [unistd.h] dnl
 [windows.h] dnl
)
AC_HEADER_TIME
AC_HEADER_STAT

# ==============================================================
# Checks for typedefs, structures, and compiler characteristics.
# ==============================================================

AM_C_PROTOTYPES
AC_C_CONST
AC_TYPE_UID_T     dnl This checks for gid_t too.
AC_CHECK_TYPES([ssize_t])

# =============================
# Checks for library functions.
# =============================

AC_CHECK_DECLS([malloc, realloc, calloc, free, memrchr, stpcpy, strcasecmp,
	        strnlen])
AC_CHECK_FUNCS([gettimeofday memcpy sleep getegid setgid setegid setregid])

adl_FUNC_READDIR

AC_FUNC_VPRINTF
AC_FUNC_MKDIR

AC_REPLACE_FUNCS([memrchr stpcpy strndup strcasecmp])
jm_FUNC_MALLOC

# We used to clear LIBS to work around a bug (?) on
# alphaev56-dec-osf4.0d where conftest (run by jm_FUNC_REALLOC) never
# exits when linked with `-lgii' or `-lggi'.  But today `-lggi' and
# `-lgii' are not put in LIBS any longer.
jm_FUNC_REALLOC

# for error.c
AC_CHECK_FUNCS([strerror])
AC_CHECK_DECLS([strerror])
AC_FUNC_STRERROR_R

jm_FUNC_FTRUNCATE
adl_FUNC_GETOPT_LONG
adl_FILELOCK

# ====================
# Check for endianess.
# ====================

AC_C_BIGENDIAN_CROSS

# ===============
# Filename style.
# ===============

jm_AC_DOS

# =====================
# Internationalization.
# =====================

ALL_LINGUAS="de fr it nl pt_BR"
AM_GNU_GETTEXT(,[need-ngettext])

# ===================================
# Setup the correct compiler options.
# ===================================

adl_ENABLE_DEVEL
adl_NDEBUG
adl_ENABLE_GPROF
adl_ENABLE_DEBUG
ad_GCC_OPTIM
ad_GCC_WARNINGS

# ==========================
# Directories configuration.
# ==========================

heroes_COMPUTE_PATHS

# ==============
# Misc. options.
# ==============

adl_ENABLE_HTML_DOC
heroes_DEBUG

# ============
# Output files
# ============

# Put config.h in the base directory because libintl expects it here,
# but keep src/config.h.in in the src/ directory to keep the base directory
# as clean as possible.
dnl Don't quote the following argument, it is parsed by automake.
AM_CONFIG_HEADER(config.h:src/config.h.in)

AC_CONFIG_FILES([
 Makefile
 doc/Makefile
 intl/Makefile
 lib/Makefile
 man/Makefile
 po/Makefile.in
 src/Makefile
 tests/Makefile
])
AC_OUTPUT

eval bindir="$bindir"
eval bindir="$bindir"
eval datadir="$datadir"
eval mandir="$mandir"
eval infodir="$infodir"
eval htmldir="$htmldir"

eval vkm_libalt_name='$adl_libalt_'$selection_vkm'_name'
eval j_libalt_name='$adl_libalt_'$selection_j'_name'
eval s_libalt_name='$adl_libalt_'$selection_s'_name'
eval m_libalt_name='$adl_libalt_'$selection_m'_name'

echo "
Heroes configuration
--------------------
Heroes version           : $VERSION
host                     : $host

binary install path      : $bindir
data install path        : ${datadir}/heroes
manual page install path : $mandir
info manual install path : $infodir
html doc install path    : $htmldir

compiler                 : $CC
general options
  CFLAGS                 : $CFLAGS $MORE_WARNINGS_CFLAGS
  LIBS                   : $LDFLAGS $LIBS

display, keyboard, mouse : ${vkm_libalt_name}
joystick                 : ${j_libalt_name}
sound                    : ${s_libalt_name}
main                     : ${m_libalt_name}"
