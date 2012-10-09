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
