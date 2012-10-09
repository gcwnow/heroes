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
