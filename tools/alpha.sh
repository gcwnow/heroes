#!/bin/sh -ex

HTDOCS_DIR=/home/groups/h/he/heroes/htdocs
WHO='Alexandre Duret-Lutz'
EMAIL='duret_g@epita.fr'
WHOCL="$WHO  <$EMAIL>"

# package name and version
eval `autoconf --trace 'AC_INIT:VERSION=$2;PACKAGE=$1'`
VERSION_=`echo $VERSION | tr . _`
PACKAGEUP=`echo ${PACKAGE}_ALPHA | tr 'a-z-' 'A-Z_'`

# make distribution
make distcheck
make dist-bzip2

# commit it, tag it
clcommit
cvs tag ${PACKAGE}_${VERSION_}

# upload it
scp $PACKAGE-$VERSION.tar.gz $PACKAGE-$VERSION.tar.bz2 shell.sf.net:$HTDOCS_DIR/dl/prereleases

# Prepare announce mail.
cat > mailtxt <<EOF
$PACKAGE $VERSION has just been uploaded.

  http://heroes.sf.net/dl/prereleases/$PACKAGE-$VERSION.tar.gz
  http://heroes.sf.net/dl/prereleases/$PACKAGE-$VERSION.tar.bz2

EOF
tools/exnews.pl NEWS >> mailtxt
cat >> mailtxt <<EOF
Enjoy.
--
$WHO
EOF

# Update web pages.
(cd ../htdocs
cvs up
perl -pi -e "s/(${PACKAGEUP}_RELEASE\()[^,]*,\s*[\d-]+/\${1}${VERSION}, `date -I`/" src/versions.m4
cat > ChangeLog.head <<EOF
`date -I`  $WHOCL

	* src/versions.m4 (${PACKAGEUP}_RELEASE): Bump to $VERSION.

EOF
mv ChangeLog ChangeLog.old
cat ChangeLog.head ChangeLog.old > ChangeLog
rm ChangeLog.head
clcommit
)

ssh shell.sf.net "cd $HTDOCS_DIR; cvs up; make -k; true"

# Mail announcement.
mail -s "$PACKAGE $VERSION uploaded" heroes-discuss@lists.sourceforge.net < mailtxt
rm mailtxt
