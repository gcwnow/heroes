AC_DEFUN([heroes_DEBUG],
[AC_ARG_ENABLE([heroes-debug],
  [AC_HELP_STRING([--disable-heroes-debug],[disable tracing facility])])

  if test "x$enable_heroes_debug" != xno; then
    AC_DEFINE([USE_HEROES_DEBUG],1,[Define if you want tracing code.])
  fi])
