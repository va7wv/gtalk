**************************
Gtalk DOS Source code - README.TXT   - David W. Jeske (jeske@chat.net)
**************************

This is the DOS source for a chat software product called "Gtalk".
It was originally written in July 1992. The first chat system based
on this software, Nuclear Greenhouse, went online in early August 1992.
Since then customers came and went, and so did the Gtalk systems. Now
a cult following of these systems remain.

Gtalk has since been ported to OS/2 and rewritten for UNIX. These versions
will be distributed as well if my time allows. I hope this source
code is of some use to you.

This distribution contains "Gtalk v1.9 emsj" the "ems" designator
means that this version includes support for ems. There is a text
file which unfortunatly only goes through version "1.9emsd" which
includes descriptions of what was done with each version. It is
included in the "docs" subdirectory as "VERSION.TXT".

The "src" directory contains source files
The "gtcon" direction contains the skeleton files for what was a
     console only distribution
The "gtalk" directory contains the skeleton files for a full-fledged
     verison.


NOTE: The DOS version contains a copy-protection scheme which
      used a checksum of part of the ROM of your machine and
      a timestamp. A validation checksum had to be generated
      and dropped into a file called "checksum.dat" at a later
      date I may go through the trouble of removing the code
      which checks for this. However, for now, you'll have to do
      this yourself. If you succeed in compiling this for DOS
      and getting it to work, you should be skilled enough to
      remove this checksum check.
