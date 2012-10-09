dnl Compile, link, and run a dummy program.
dnl This is used to ensure that further uses of AC_TRY_RUN
dnl not fail (usually due to some unloadable libraries),

AC_DEFUN([adl_RUN_CHECK],
[AC_TRY_RUN(
[main ()
{
  exit (0);
}],,
[AC_MSG_ERROR([===
Compiled programs no longer run whith this setup.  Most usually this
means one of the library checked for above is not correctly installed
(for example it can depens over some other uninstalled library, or
been installed in a location which is not looked at by your system).
You might want to check the error messages in the file 'config.log'.
If you believe this is a bug in Heroes, be sure to send the file
'config.log' with your report to <heroes-bugs@lists.sourceforge.net>.
])],[:])])
