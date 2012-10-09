dnl @synopsis AC_C_BIGENDIAN_CROSS
dnl
dnl Check endianess even when crosscompiling
dnl (partially based on the original AC_C_BIGENDIAN).
dnl
dnl The implementation will create a binary, and instead of running
dnl the binary it will be grep'ed for some symbols that will look
dnl different for different endianess of the binary.
dnl
dnl @version $Id: ac_c_bigendian_cross.m4,v 1.1 2000/09/09 13:29:23 simons Exp $
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AC_C_BIGENDIAN_CROSS],
[AC_CACHE_CHECK([whether byte ordering is bigendian], [ac_cv_c_bigendian],
[# See if sys/param.h defines the BYTE_ORDER macro.
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/param.h>], [
#if !BYTE_ORDER || !BIG_ENDIAN || !LITTLE_ENDIAN
 bogus endian macros
#endif],
[# It does; now see whether it defined to BIG_ENDIAN or not.
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/param.h>], [
#if BYTE_ORDER != BIG_ENDIAN
 not big endian
#endif], [ac_cv_c_bigendian=yes], [ac_cv_c_bigendian=no])],
[# It does not, try to run a simple test
AC_TRY_RUN([main () {
  /* Are we little or big endian?  From Harbison&Steele.  */
  union
  {
    long l;
    char c[sizeof (long)];
  } u;
  u.l = 1;
  exit (u.c[sizeof (long) - 1] == 1);
}], [ac_cv_c_bigendian=no], [ac_cv_c_bigendian=yes],
[ac_cv_c_bigendian="unknown (cross-compiling)"])
])])

if test "$ac_cv_c_bigendian" = "unknown (cross-compiling)"; then
  AC_MSG_CHECKING([whether byte order LOOKS bigendian])
[cat >conftest.c <<EOF
short ascii_mm[] = { 0x4249, 0x4765, 0x6E44, 0x6961, 0x6E53, 0x7953, 0 };
short ascii_ii[] = { 0x694C, 0x5454, 0x656C, 0x6E45, 0x6944, 0x6E61, 0 };
void _ascii() { char* s = (char*) ascii_mm; s = (char*) ascii_ii; }
short ebcdic_ii[] = { 0x89D3, 0xE3E3, 0x8593, 0x95C5, 0x89C4, 0x9581, 0 };
short ebcdic_mm[] = { 0xC2C9, 0xC785, 0x95C4, 0x8981, 0x95E2, 0xA8E2, 0 };
void _ebcdic() { char* s = (char*) ebcdic_mm; s = (char*) ebcdic_ii; }
int main() { _ascii (); _ebcdic (); return 0; }
EOF
] if test -f conftest.c ; then
     if ${CC-cc} conftest.c -o conftest.o && test -f conftest.o ; then
        if test `grep -l BIGenDianSyS conftest.o` ; then
           ac_cv_c_bigendian=yes
        fi
        if test `grep -l LiTTleEnDian conftest.o` ; then
           ac_cv_c_bigendian=no
        fi
     fi
   fi
   AC_MSG_RESULT([$ac_cv_c_bigendian])
fi

case "$ac_cv_c_bigendian" in
  yes)
    AC_DEFINE([WORDS_BIGENDIAN], 1, [whether byteorder is bigendian])
    BYTEORDER=4321
    ;;
  no)
    BYTEORDER=1234
    ;;
  *)
    AC_MSG_ERROR([unknown endianess - sorry])
    ;;
esac

AC_DEFINE_UNQUOTED([BYTEORDER], [$BYTEORDER], 
[Set to 1234 if the system is little-endian, 4321 if big-endian])
])
