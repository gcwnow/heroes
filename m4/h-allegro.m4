AC_DEFUN([heroes_CHECK_ALLEGRO],
[## Allegro support is hidden until it is better supported.
##AC_ARG_WITH([allegro],
##  [AC_HELP_STRING([--with-allegro=DIR],[root directory of Allegro installation])
##AC_HELP_STRING([--without-allegro],[disables Allegro usage completely])])

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
