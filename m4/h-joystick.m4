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
