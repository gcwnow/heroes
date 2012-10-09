dnl We need opendir, readdir and closedir.  This checks only for
dnl readdir and assumes that opendir and closedir are present if readdir is.
dnl Under plain Win32 (i.e. not Mingw or Cygwin) these functions are not
dnl available, but can be emulated with lib/w_dirent.c.  It's a bit odd to
dnl perform that test here, since "plain Win32" users are unlikely to
dnl run configure... Hence this code only documents when w_dirent.c should
dnl be built.
dnl
dnl Written by Alexandre Duret-Lutz <duret_g@epita.fr>

AC_DEFUN([adl_FUNC_READDIR],
 [AC_CHECK_FUNC([readdir],
  [AC_DEFINE([HAVE_READDIR],1,[Define if you have readdir and friends.])],
   [AC_CHECK_HEADER([windows.h],
    [# build w_dirent if readdir is absent and windows.h exists
     AC_LIBOBJ(w_dirent)],
    [AC_MSG_ERROR([Could not find `readdir'.])])])])
