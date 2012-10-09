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
