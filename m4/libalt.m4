# adl_LIBALT_OK (HANDLE, NAME, LDFLAGS, LDADD, [DEPENDENCIES], [OPTION])
# ----------------------------------------------------------------------
AC_DEFUN([adl_LIBALT_OK], [
  adl_libalt_$1_ok=yes
  adl_libalt_$1_name="$2"
  adl_libalt_$1_LDFLAGS="$3"
  adl_libalt_$1_LDADD="$4"
  adl_libalt_$1_selected=no
  adl_libalt_$1_depend_on="$5"
  adl_libalt_$1_opt="$6"
])

# adl_LIBALT_EITHER (HANDLES, DOC, VAR, LDADD_PREFIX)
# ---------------------------------------------------
AC_DEFUN([adl_LIBALT_EITHER], [
  $3=NONE
  for i in $1; do
    if eval test "x\$adl_libalt_${i}_ok" = xyes ; then
      # make sure all dependencies for this module have already been selected.
      eval adl_libalt_tmp_dep="\$adl_libalt_${i}_depend_on"
      adl_libalt_tmp_ok=yes
      for j in $adl_libalt_tmp_dep : ; do
        if test $j = : ; then
          break
        fi
        if eval test "x\$adl_libalt_${j}_selected" != xyes ; then
          adl_libalt_tmp_ok=no
          break
        fi
      done
      if test "$adl_libalt_tmp_ok" = yes ; then
        eval adl_libalt_tmp_opt=\"\$adl_libalt_${i}_opt\"
	# The default is to prepend.
	if test x"$adl_libalt_tmp_opt" = xAPPEND; then
          eval LIBALT_LDFLAGS=\"\$LIBALT_LDFLAGS \$adl_libalt_${i}_LDFLAGS\"
          eval LIBALT_LDADD=\"\$LIBALT_LDADD \$adl_libalt_${i}_LDADD\"
          ifelse([$4],,, [LIBALT_LOCAL_LDADD="$LIBALT_LOCAL_LDADD $4$i.a"])
	else
          eval LIBALT_LDFLAGS=\"\$adl_libalt_${i}_LDFLAGS \$LIBALT_LDFLAGS\"
          eval LIBALT_LDADD=\"\$adl_libalt_${i}_LDADD \$LIBALT_LDADD\"
          ifelse([$4],,, [LIBALT_LOCAL_LDADD="$4$i.a $LIBALT_LOCAL_LDADD"])
	fi
        eval "adl_libalt_${i}_selected=yes"
        $3=$i
        break
      fi
    fi
  done
  if test "x$$3" = xNONE ; then
     AC_MSG_ERROR([No $2 found.

The supported libraries are listed in README.])
  else
     eval adl_tmp_libalt_name='$adl_libalt_'"$$3"'_name'
     AC_MSG_NOTICE([Selected $2: $adl_tmp_libalt_name])
  fi
  # Use _XLDLAGS, _XLDADS, etc. because Automake reserves _LDFLAGS, etc.
  AC_SUBST([LIBALT_XLDFLAGS], [$LIBALT_LDFLAGS])
  AC_SUBST([LIBALT_XLDADD], [$LIBALT_LDADD])
  AC_SUBST([LIBALT_LOCAL_XLDADD], [$LIBALT_LOCAL_LDADD])
])
