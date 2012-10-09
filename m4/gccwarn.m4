dnl Check if the compiler supports useful warning options.  There's a few that
dnl we don't use, simply because they're too noisy:
dnl
dnl -ansi (prevents declaration of functions like strdup, and because
dnl        it makes warning in system headers).
dnl -Wconversion (useful in older versions of gcc, but not in gcc 2.7.x)
dnl -Wtraditional (combines too many unrelated messages, only a few useful)
dnl -Wredundant-decls (system headers make this too noisy)
dnl -pedantic
dnl

AC_DEFUN([ad_GCC_WARNINGS],
[AC_ARG_ENABLE([warnings],
[AC_HELP_STRING([--enable-warnings], [enable compiler warnings])])
 if test "$enable_warnings" = "yes"; then
   CF_GCC_WARNINGS
   CF_GCC_MORE_WARNINGS
   AC_SUBST([MORE_WARNINGS_CFLAGS])
 fi])

AC_DEFUN([CF_GCC_WARNINGS],
[if test -n "$GCC"; then
  AC_CACHE_CHECK([for gcc warning options], ac_cv_prog_gcc_warn_flags,
  [changequote(,)dnl
  cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[]) { return argv[argc-1] == 0; }
EOF
  changequote([,])dnl
  cf_save_CFLAGS="$CFLAGS"
  ac_cv_prog_gcc_warn_flags="-W -Wall"
  for cf_opt in \
   Waggregate-return \
   Wbad-function-cast \
   Wcast-align \
   Winline \
   Wnested-externs \
   Wpointer-arith \
   Wwrite-strings
  do
    CFLAGS="$cf_save_CFLAGS $ac_cv_prog_gcc_warn_flags -$cf_opt"
    if AC_TRY_EVAL(ac_compile); then
      ac_cv_prog_gcc_warn_flags="$ac_cv_prog_gcc_warn_flags -$cf_opt"
      test "$cf_opt" = Wcast-qual && ac_cv_prog_gcc_warn_flags="$ac_cv_prog_gcc_warn_flags -DXTSTRINGDEFINES"
    fi
  done
  rm -f conftest*
  CFLAGS="$cf_save_CFLAGS"])
fi
CFLAGS="${CFLAGS} ${ac_cv_prog_gcc_warn_flags}"
])dnl

AC_DEFUN([CF_GCC_MORE_WARNINGS],
[if test -n "$GCC"; then
  AC_CACHE_CHECK([for more gcc warning options], ac_cv_prog_gcc_more_warn_flags,
  [changequote(,)dnl
  cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[]) { return argv[argc-1] == 0; }
EOF
  changequote([,])dnl
  cf_save_CFLAGS="$CFLAGS"
  ac_cv_prog_gcc_more_warn_flags=''
  for cf_opt in \
   Wmissing-declarations \
   Wmissing-prototypes \
   Wstrict-prototypes \
   Wshadow \
   Wcast-qual \
   Werror
  do
    CFLAGS="$cf_save_CFLAGS $ac_cv_prog_gcc_more_warn_flags -$cf_opt"
    if AC_TRY_EVAL(ac_compile); then
      ac_cv_prog_gcc_more_warn_flags="$ac_cv_prog_gcc_more_warn_flags -$cf_opt"
      test "$cf_opt" = Wcast-qual && ac_cv_prog_gcc_more_warn_flags="$ac_cv_prog_gcc_more_warn_flags -DXTSTRINGDEFINES"
    fi
  done
  rm -f conftest*
  CFLAGS="$cf_save_CFLAGS"])
fi
MORE_WARNINGS_CFLAGS="${ac_cv_prog_gcc_more_warn_flags}"
])dnl
