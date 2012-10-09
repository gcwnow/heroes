AC_DEFUN([heroes_aux_OBSOPT],
[if test -n "$MY_TR_SH($1)"; then
  AC_MSG_ERROR([--$1 is an obsolete option[]ifelse([$2],,,[,
you should use --$2 instead])])
fi])

AC_DEFUN([heroes_aux_ERROPT],
[if test -n "$MY_TR_SH($1)"; then
  AC_MSG_ERROR([--$1 is an illegal option[]ifelse([$2],,,[,
you probably want to use --$2])])
fi])
AC_DEFUN([heroes_CHECK_OBSOLETE_OPTIONS],
[# check for opsolete or erroneous options
heroes_aux_ERROPT([with-SDL],[with-sdl])
dnl It's not possible to test for the next one:
dnl because both sides maps to the same shell variable :o(
dnl heroes_aux_OBSOPT([with-sdl_mixer],[with-sdl-mixer])
heroes_aux_OBSOPT([with-GGI],[with-ggi])
heroes_aux_OBSOPT([with-GII],[with-gii])
])
