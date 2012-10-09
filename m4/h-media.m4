AC_DEFUN([heroes_MEDIALIB_SELECTION], [

  user_selection_list_vkm=''
  user_selection_list_s=''
  user_selection_list_j=''

  heroes_CHECK_DISPLAY_LIB
  heroes_CHECK_JOYSTICK_LIB
  heroes_CHECK_SOUND_LIB

  # These libraries do not need any checking.
  adl_LIBALT_OK([dums], [none], [], [])
  adl_LIBALT_OK([dumj], [none], [], [])
  adl_LIBALT_OK([stdm], [standard main], [], [])
  # There is a trick here.  Usually our static library are added
  # to LIBALT_LOCAL_LDADD which is listed before LIBALT_LDADD (the system
  # libraries) on the link line.  However, SDL_main and Allegro are special:
  # they refer to OUR code (practically they call our main, which has been
  # renamed to SDL_main() or _mangled_main()). So in addition to adding
  # sys/libhallm.a to LIBALT_LOCAL_LDADD (done automatically by adl_LIBALT_OK),
  # we will also list it in LIBALT_LDADD (requested below).  The 'APPEND' means
  # the libraries should be appended instead of being prepended, since we
  # want them to be listed after -lSDL_main or -lallegro.
  adl_LIBALT_OK([sdlm], [SDL_main], [],
		[sys/libhsdlm.a], [sdlvkm], [APPEND])
  adl_LIBALT_OK([allm], [Allegro mangled main], [],
		[sys/libhallm.a], [allvkm], [APPEND])

  # Heroes can run without display, but usually this is not what
  # the user wants :), so this "feature" is enabled only if
  # --disable-display is supplied.
  AC_ARG_ENABLE([display],
	        [AC_HELP_STRING([--disable-display],
                                [don't draw anything on screen])])
  if test "${enable_display-yes}" = no; then
    user_selection_list_vkm="dumvkm $user_selection_list_vkm"
    adl_LIBALT_OK([dumvkm], [none], [], [])
  fi

  # If the user has no preference, we'll use our own preference list.

  if test -z "$user_selection_list_vkm"; then
     selection_list_vkm="ggivkm sdlvkm allvkm dumvkm"
  else
     selection_list_vkm="$user_selection_list_vkm dumvkm"
  fi

  if test -z "$user_selection_list_s"; then
     # Keep sdls first.  It won't be used if sdlvkm is not selected,
     # but otherwise it should be prefered over miks.
     selection_list_s="sdls miks dums"
  else
     selection_list_s="$user_selection_list_s dums"
  fi

  if test -z "$user_selection_list_j"; then
     # Keep sdlj first.  It won't be used if sdlvkm is not selected,
     # but otherwise it should be prefered over giij.
     # Keep allj second for the same reason.
     selection_list_j="sdlj allj giij dumj"
  else
     selection_list_j="$user_selection_list_j dumj"
  fi

  adl_LIBALT_EITHER([$selection_list_vkm], [video-keyboard-mouse library],
	            [selection_vkm], [media/libh])
  adl_LIBALT_EITHER([$selection_list_s], [sound library], [selection_s],
		    [media/libh])
  adl_LIBALT_EITHER([$selection_list_j], [joystick library], [selection_j],
	            [media/libh])
  adl_LIBALT_EITHER([sdlm allm stdm], [startup library], [selection_m],
		    [sys/libh])
])
