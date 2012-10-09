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
