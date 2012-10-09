dnl The Autoconf maintainers keep moving undocumented functions around,
dnl so let's use what we find, to stay compatible with different version
dnl of Autoconf...
AC_DEFUN([MY_TR_SH],[ifdef([AS_TR_SH],[AS_TR_SH($@)],[AC_TR_SH($@)])])
AC_DEFUN([MY_TR_CPP],[ifdef([AS_TR_CPP],[AS_TR_CPP($@)],[AC_TR_CPP($@)])])
AC_DEFUN([MY_VAR_PUSHDEF],
	 [ifdef([AS_VAR_PUSHDEF],[AS_VAR_PUSHDEF($@)],[AC_VAR_PUSHDEF($@)])])
AC_DEFUN([MY_VAR_POPDEF],
	 [ifdef([AS_VAR_POPDEF],[AS_VAR_POPDEF($@)],[AC_VAR_POPDEF($@)])])
AC_DEFUN([MY_VAR_SET],[ifdef([AS_VAR_SET],[AS_VAR_SET($@)],[AC_VAR_SET($@)])])
