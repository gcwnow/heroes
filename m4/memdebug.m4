dnl Some macros to configure with malloc() debugging libraries.
dnl
dnl Main functions:
dnl  adl_WITH_MTRACE		handle the --with-mtrace option
dnl  adl_WITH_DMALLOC		handle the --with-dmalloc option
dnl  adl_WITH_EFENCE		handle the --with-efence option
dnl  adl_WITH_MPATROL		handle the --with-mpatrol option
dnl  adl_ENABLE_MEM_DEBUG	handle the --enable-mem-debug option
dnl				(i.e. use mpatrol, mtrace, dmalloc
dnl                              or efence, whatever the first found is)
dnl
dnl Auxiliary functions:
dnl  adl_CHECK_MTRACE		actually check for mtrace availability
dnl  adl_CHECK_DMALLOC		actually check for dmalloc availability
dnl  adl_CHECK_EFENCE		actually check for efence availability
dnl  adl_CHECK_MPATROL		actually check for mpatrol availability
dnl  adl_CHECK_MEMORY_USAGE	actually check gcc -fcheck-memory-usage
dnl                             (can be used with mpatrol, currently
dnl                              not called)
dnl  adl_CHECK_MPATROL_LIBS     actually check for libraries needed by
dnl				  mpatrol.
dnl
dnl Written by Alexandre Duret-Lutz <duret_g@epita.fr>


AC_PREREQ([2.49])  dnl for AC_HELP_STRING

dnl adl_CHECK_MTRACE([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for mtrace (GNU libc malloc debugging functions).
dnl
dnl This will define HAVE_MTRACE if mcheck.h and mtrace() exists.
dnl
dnl Use this macro in conjunction with the following code.
dnl #if HAVE_MTRACE
dnl # include <mcheck.h>
dnl #else
dnl # define mtrace()
dnl # define muntrace()
dnl #endif
dnl and put the call to mtrace and muntrace where appropriate.

AC_DEFUN([adl_CHECK_MTRACE],
 [test_failed='no'
  # should we check for mtrace?
  if test "$with_mtrace" = no; then
    test_failed='yes'
  else
    AC_CHECK_HEADER([mcheck.h],
     [AC_CHECK_FUNCS([mtrace],[$1],[test_failed=yes])],
     [test_failed=yes])
    ifelse([$2],,
     [if test $test_failed = 'yes'; then
       AC_MSG_ERROR([Cannot find mtrace])
      fi])
  fi
  ifelse([$2],,,
   [if test "$test_failed" = yes; then
     $2
    fi])])


dnl adl_WITH_MTRACE ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for dmalloc on user request.

AC_DEFUN([adl_WITH_MTRACE],
 [AC_ARG_WITH([mtrace],
  [AC_HELP_STRING([--with-mtrace],
   [use GNU libc's mtrace for malloc () debugging])])
  if test "$with_mtrace" = yes; then
   adl_CHECK_MTRACE
  fi])


dnl adl_CHECK_DMALLOC ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for dmalloc.
dnl
dnl This macro defines HAVE_DMALLOC_H when the dmalloc.h header is found.
dnl dmalloc.h is not mandatory to use dmalloc, but it is better to
dnl include it when available.  Here is how:
dnl
dnl /* keep this header at the end of the include list, because it may
dnl    define some macros to change the declaration of malloc functions */
dnl #if HAVE_DMALLOC_H
dnl # define DMALLOC_FUNC_CHECK
dnl # include <dmalloc.h>
dnl #endif

AC_DEFUN([adl_CHECK_DMALLOC],
 [test_failed='no'
  # should we check for dmalloc?
  if test "${with_dmalloc}" = no; then
    test_failed='yes'
  else
  # see if dmalloc is usable
    AC_CHECK_LIB([dmalloc],[malloc],
     [$1
      LIBS="-ldmalloc $LIBS"
      AC_CHECK_HEADERS([dmalloc.h])],
     [ifelse([$2],,[AC_MSG_ERROR([Cannot find dmalloc])],[test_failed='yes'])])
  fi
  ifelse([$2],,,
   [if test "$test_failed" = yes; then
     $2
    fi])])


dnl adl_WITH_DMALLOC ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for dmalloc on user request.

AC_DEFUN([adl_WITH_DMALLOC],
 [AC_ARG_WITH([dmalloc],
  [AC_HELP_STRING([--with-dmalloc],
   [link with dmalloc (a malloc() debugger)])])
  if test "$with_dmalloc" = yes; then
   adl_CHECK_DMALLOC
  fi])


dnl adl_CHECK_EFENCE ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for efence.

AC_DEFUN([adl_CHECK_EFENCE],
 [test_failed='no'
  # should we check for efence?
  if test "${with_efence}" = no; then
    test_failed='yes'
  else
  # see if efence is usable
    AC_CHECK_LIB([efence],[malloc],
     [$1
      LIBS="-lefence $LIBS"],
     [ifelse([$2],,[AC_MSG_ERROR([Cannot find Electric Fence])],
      [test_failed='yes'])])
  fi
  ifelse([$2],,,
   [if test "$test_failed" = yes; then
     $2
    fi])])


dnl adl_WITH_EFENCE ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for efence on user request.

AC_DEFUN([adl_WITH_EFENCE],
 [AC_ARG_WITH([efence],
  [AC_HELP_STRING([--with-efence],
   [link with efence (a malloc() debugger)])])
  if test "$with_efence" = yes; then
   adl_CHECK_EFENCE
  fi])



dnl adl_CHECK_MEMORY_USAGE
dnl Check wheter $CC supports the -fcheck-memory-usage option.
AC_DEFUN([adl_CHECK_MEMORY_USAGE],
[if test -n "$GCC"; then
  AC_CACHE_CHECK([wether $CC supports -fcheck-memory-usage],
		 [ac_cv_check_memory_usage],
  [changequote(,)dnl
  cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[]) { return argv[argc-1] == 0; }
EOF
  changequote([,])dnl
  cf_save_CFLAGS="$CFLAGS"
  CFLAGS="$cf_save_CFLAGS -fcheck-memory-usage"
    if AC_TRY_EVAL(ac_compile); then
      ac_cv_check_memory_usage=yes
    else
      ac_cv_check_memory_usage=no
    fi
  rm -f conftest*
  CFLAGS="$cf_save_CFLAGS"])
fi
if test "$ac_cv_check_memory_usage" = yes; then
  CFLAGS="${CFLAGS} -fcheck-memory-usage"
fi])

dnl adl_CHECK_MPATROL_LIBS ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for libraries needed by mpatrol
AC_DEFUN([adl_CHECK_MPATROL_LIBS],
[AC_CACHE_CHECK([for library needed by mpatrol], [ac_cv_mpatrol_libs],
[ac_cv_mpatrol_libs=missing
adl_save_LIBS=$LIBS
for lib_try in '' '-lelf' '-lbfd -liberty'  '-lbfd -liberty -lcl'; do
  LIBS="$adl_save_LIBS -lmpatrol $lib_try"
  AC_TRY_LINK_FUNC([malloc],[ac_cv_mpatrol_libs="-lmpatrol $lib_try"
break])
done
LIBS="$adl_save_LIBS"])
if test "$ac_cv_mpatrol_libs" != missing; then
  LIBS="$LIBS $ac_cv_mpatrol_libs"
  $1
ifelse([$2],,,[else
  $2])
fi])

dnl adl_CHECK_MPATROL ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for mpatrol.
dnl
dnl This macro defines HAVE_MPATROL_H when the mpatrol.h header is found.
dnl mpatrol.h is not mandatory to use mpatrol, but it is better to
dnl include it when available.  Here is how:
dnl
dnl /* keep this header at the end of the include list, because it may
dnl    define some macros to change the declaration of malloc functions */
dnl #if HAVE_MPATROL_H
dnl # include <mpatrol.h>
dnl #endif

AC_DEFUN([adl_CHECK_MPATROL],
 [test_failed='no'
  # should we check for mpatrol?
  if test "${with_mpatrol}" = no; then
    test_failed='yes'
  else
    # see if mpatrol is usable
    adl_CHECK_MPATROL_LIBS([$1
dnl   CHECK_MEMORY_USAGE is disabled, because it causes much troubles
dnl   with asm statements in system headers.
dnl   adl_CHECK_MEMORY_USAGE
      AC_CHECK_HEADERS([mpatrol.h])],
     [ifelse([$2],,[AC_MSG_ERROR([Cannot find mpatrol])],[test_failed='yes'])])
  fi
  ifelse([$2],,,
   [if test "$test_failed" = yes; then
     $2
    fi])])


dnl adl_WITH_MPATROL ([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Check for mpatrol on user request.

AC_DEFUN([adl_WITH_MPATROL],
 [AC_ARG_WITH([mpatrol],
  [AC_HELP_STRING([--with-patrol],
   [link with mpatrol (a malloc() debugger)])])
  if test "$with_mpatrol" = yes; then
   adl_CHECK_MPATROL
  fi])


dnl adl_ENABLE_MEM_DEBUG
dnl Check for dmalloc or efence on request, use the first library found.

AC_DEFUN([adl_ENABLE_MEM_DEBUG],
 [# don't test anything if --with-dmalloc or --with-efence
  # was given (assume that adl_WITH_DMALLOC or adl_WITH_EFENCE
  # has already run).
  if test "${with_mpatrol-no}" = no &&
     test "${with_mtrace-no}" = no &&
     test "${with_dmalloc-no}" = no &&
     test "${with_efence-no}" = no; then
    AC_ARG_ENABLE([mem-debug],
    [AC_HELP_STRING([--enable-mem-debug],
     [link with any malloc() debugger available])])
    if test "$enable_mem_debug" = yes; then
      adl_CHECK_MPATROL(,[dnl
	adl_CHECK_MTRACE(,[dnl
	  adl_CHECK_DMALLOC(,[dnl
	    adl_CHECK_EFENCE])])])
    fi
  fi])
