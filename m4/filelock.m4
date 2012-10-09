# Check for functions and headers needed by lib/filelock.c
AC_DEFUN([adl_FILELOCK], [
 AC_CHECK_HEADERS([fcntl.h sys/file.h io.h sys/locking.h])
 AC_CHECK_FUNCS([fcntl flock _locking])
])
