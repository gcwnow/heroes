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
