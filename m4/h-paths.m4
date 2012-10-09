AC_DEFUN([heroes_COMPUTE_PATHS],
[AC_REQUIRE([AM_WITH_NLS])dnl

## With gettext <= 0.10.35, localedir used to be defined as
##   localedir='${prefix}'"/$DATADIRNAME/locale"
## DATADIRNAME being set to share (or lib) by AM_GNU_GETTEXT.  So this
## path had nothing to do with datadir and when Heroes was configured
## with --prefix=/usr --bindir=/usr/games --datadir=/usr/share/games as
## the FHS requests for games, locale data would go into
## /usr/share/locale.
##
## In gettext >= 0.10.36, localedir is defined as
##   localedir='${datadir}'/locale
## Thus with the same configuration locales will go into
## /usr/share/games/locale.
##
## The FHS is fuzzy enough to let both setups be seen as correct.
##
## Another important point is that there is no way to change the
## locales installation path.  `make localedir=/somewhere/else install'
## will NOT work because we need to know the real $localedir path at
## configure time (this macro compute a relative path between $prefix
## and $localedir).
##
## Ideally there should be a --localedir option to configure to set
## this path.  Both Akim Demaille (Autoconf) and Bruno Haible (Gettext)
## are aware of this problem.  Gettext needs Autoconf support before
## changing its macros and Makefiles, but Autoconf 2.50 is about to be
## released and won't include that change.  So this is unlikely to be
## solved anytime soon.  Since such change will probably involve an
## update of the GNU Standards, Bruno said he will contact RMS.
##
## By the meantime, if people do need to be able to change the localedir
## path it should be possible to add a option such as
## --enable-localedir=DIR or --with-localedir=DIR.  This requires
## going through the trouble of patching gettext's m4 macro and Makefiles.
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
