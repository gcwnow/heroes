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
