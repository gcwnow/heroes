dnl @synopsis adl_COMPUTE_STANDARD_RELATIVE_PATHS
dnl
dnl Here is the standard hierarchy of paths,
dnl as defined by the GNU Coding Standards:
dnl
dnl    prefix
dnl   	  exec_prefix
dnl   	     bindir
dnl   	     libdir
dnl   	     libexecdir
dnl   	     sbindir
dnl   	  datadir
dnl   	  sysconfdir
dnl   	  sharestatedir
dnl   	  localstatedir
dnl   	  infodir
dnl   	  lispdir
dnl   	  includedir
dnl   	  oldincludedir
dnl   	  mandir
dnl
dnl This macro will setup a set of variables of the form
dnl 'xxx_forward_relative_path' and 'xxx_backward_relative_path'
dnl where xxx is one of the above directories.  The latter variable
dnl is set to the relative path to go from xxx to its parent
dnl directory, while the former hold the other way.
dnl
dnl For instance `bindir_relative_path' will contains the value to
dnl add to $exec_prefix to reach the $bindir directory (usually 'bin'),
dnl and `bindir_backward_relative_path' the value to append to $bindir
dnl to reach the $exec_prefix directory (usually '..').
dnl
dnl This macro requires adl_COMPUTE_RELATIVE_PATHS which itself
dnl requires adl_NORMALIZE_PATH.
dnl
dnl @author Alexandre Duret-Lutz <duret_g@epita.fr>
dnl @version $Id: $
AC_DEFUN([adl_COMPUTE_STANDARD_RELATIVE_PATHS],
## These calls need to be on separate lines for aclocal to work!
[adl_COMPUTE_RELATIVE_PATHS(dnl
adl_STANDARD_RELATIVE_PATH_LIST)])

dnl adl_STANDARD_RELATIVE_PATH_LIST
dnl ===============================
dnl A list of standard paths, ready to supply to adl_COMPUTE_RELATIVE_PATHS.
AC_DEFUN([adl_STANDARD_RELATIVE_PATH_LIST],
[pushdef([TRIPLET],
[$][1:$][2:$][2_forward_relative_path $]dnl
[2:$][1:$][2_backward_relative_path])dnl
TRIPLET(prefix, exec_prefix) dnl
TRIPLET(exec_prefix, bindir) dnl
TRIPLET(exec_prefix, libdir) dnl
TRIPLET(exec_prefix, libexecdir) dnl
TRIPLET(exec_prefix, sbindir) dnl
TRIPLET(prefix, datadir) dnl
TRIPLET(prefix, sysconfdir) dnl
TRIPLET(prefix, sharestatedir) dnl
TRIPLET(prefix, localstatedir) dnl
TRIPLET(prefix, infodir) dnl
TRIPLET(prefix, lispdir) dnl
TRIPLET(prefix, includedir) dnl
TRIPLET(prefix, oldincludedir) dnl
TRIPLET(prefix, mandir) dnl
popdef([TRIPLET])])
