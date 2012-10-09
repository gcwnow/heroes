dnl This will call AC_PATH_GENERIC but check that the library actually link.
dnl (people installing library from binary packages can have libraries
dnl that do NOT link when dependencies are not satisfied)
dnl
dnl  AC_adl_PKG_GENERIC(library,version,function,action-if-ok,action-if-not)
dnl
dnl Inspired from AC_CAOLAN_CHECK_PACKAGE.
dnl Alexandre Duret-Lutz <duret_g@epita.fr>
AC_DEFUN([AC_adl_PKG_GENERIC],[
  pushdef([UP], translit([$1], [a-z], [A-Z]))dnl

  OLD_LIBS="$LIBS"
  OLD_CFLAGS="$CFLAGS"
  jolly_good=true
  AC_PATH_GENERIC([$1],[$2],,[jolly_good=false])
  ifelse([$3],,,[if $jolly_good; then
    CFLAGS="$UP[]_CFLAGS $CFLAGS"
    LIBS="$UP[]_LIBS $LIBS"
    AC_CHECK_FUNC([$3],,[jolly_good=false])
  fi])
  if $jolly_good; then
    ifelse([$4],,,[$4])
    LIBS="$OLD_LIBS"
    CFLAGS="$OLD_CFLAGS"
  else
    LIBS="$OLD_LIBS"
    CFLAGS="$OLD_CFLAGS"
    ifelse([$5],,,[$5])
  fi

  popdef([UP])
])
