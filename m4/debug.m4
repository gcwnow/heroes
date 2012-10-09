AC_DEFUN([adl_ENABLE_DEBUG],
 [AC_ARG_ENABLE([debug],
  [AC_HELP_STRING([--enable-debug],[enable debugging symbols])])
  case "${enable_debug}" in
    yes)
      AC_DEFINE([DEBUG],1,[Define if you want debugging code.])
      if test -n "$GCC"; then
        CFLAGS="$CFLAGS -ggdb3"
      else
        if test "${ac_cv_prog_cc_g}" = yes; then
          CFLAGS="$CFLAGS -g"
        fi
      fi
      ;;
    no)
      ;;
    *)
      if test "${ac_cv_prog_cc_g}" = yes; then
        CFLAGS="$CFLAGS -g"
      fi
      ;;
  esac])
