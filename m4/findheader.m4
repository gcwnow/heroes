dnl AC_adl_FIND_HEADER(HEADER-NAME, CPPFLAGS, OTHER-NAMES, REGEX, NMATCH
dnl                    ACTION-IF-YES, ACTION-IF-NOT)
dnl
dnl Search for the exact location of header `<HEADER-NAME>' which must
dnl match `NMATCH' times the regex `REGEX'.  If is header is not found,
dnl try the OTHER-NAMES list of headers.
dnl
dnl Generalized by Alexandre Duret-Lutz <duret_g@epita.fr>
dnl from a test in Zsh's configure.in (written by?)

AC_DEFUN([AC_adl_FIND_HEADER],
[# Where is <$1> located?
MY_VAR_PUSHDEF([ac_adl_Header], [ac_cv_header_path_$1])
AC_CACHE_CHECK([where $1 is located], ac_adl_Header,
[# Look at the output from the preprocessor.
# We should get lines of the form `# 1 "/usr/include/$1"'
# The backslash substitution is to persuade cygwin to cough up
# slashes rather than doubled backslashes in the path.
echo "#include <$1>" > nametmp.c
afh_file_list="`$CPP $2 nametmp.c 2> /dev/null |
sed -n 's/^#[ 	].*\"\(.*\)\"/\1/p' |
sed 's/\\\\\\\\/\//g' |
$AWK '{ if (\@S|@1 ~ \"$1\") files[[\@S|@1]] = \@S|@1 }
  END { for (var in files) print var }'`"
dnl Note: `@S|@' will be translated into `$' by autoconf.
rm -f nametmp.c
if test -z "$afh_file_list"; then
  # In case we don't get the stuff from the preprocesor, use the
  # list of standard places.
  afh_file_list="$3 /dev/null"
fi
for ASH_FILE_H in $afh_file_list
do
  # make sure it doesn't get confused by files that don't
  # define what is searched for
  ash_nmatch=`test -f $ASH_FILE_H && \
  grep '$4' $ASH_FILE_H | \
  wc -l | sed 's/[ 	]//g'`
  test "x$ash_nmatch" != x && test "$ash_nmatch" -ge $5 && break
done
MY_VAR_SET(ac_adl_Header,[$ASH_FILE_H])
if test "$ASH_FILE_H" = "/dev/null"; then
  ifelse([$7], , [:], [$7])
else
  ifelse([$6], , [:], [$6])
fi])
MY_VAR_POPDEF([ac_adl_Header])])
