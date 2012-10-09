1 {
 i\
\  .---------------------------------------------------------------------.\
\  | The following information has been extracted automatically from the |\
\  | Texinfo documentation to help you install Heroes.  You should read  |\
\  | the full manual for more complete documentation.                    |\
\  `---------------------------------------------------------------------'\
\
Summary\
*******\
\
  * Overview of Heroes\
    - License\
    - Getting Heroes\
    - Reporting bugs, sending suggestions or contributions\
\
  * Installation of Heroes\
    - Needed Libraries\
    - configure generic instructions\
    - configure options for Heroes\
    - Building a relocatable package\
    - Machines\

}

/^Getting Heroes/ {
 i\
License\
=======\
\
   The full game (that is: source code, pictures, sound-tracks, and levels)\
is distributed under the terms of the GNU General Public License version 2.\
See the file COPYING for details.\

}

/^Overview of Heroes/,/^Mailing lists/ {
 /^Mailing lists/d
 p
}

/^Installation of Heroes/ {
 i\
Reporting bugs, sending suggestions or contributions\
====================================================\
\
   We welcome bug reports or suggestions for Heroes (both program or\
documentation).  Please email them to <heroes-bugs@lists.sourceforge.net>\
or <heroes-discuss@lists.sourceforge.net>.  Please, see the Texinfo\
documentation of Heroes for guidelines regarding bug reporting.\

}

/^Installation of Heroes/,/^Running Heroes/ {
 /^Running Heroes/d
 s/To be filled./Read the file INSTALL if you are not familiar with configure scripts./
 p
}
