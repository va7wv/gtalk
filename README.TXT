NOTES (June 2026):

I have disabled the hardware encryption key so you don't have to.
The precompiled release version in \GTALK will not run due to this
DRM mechanism that XORed the old DOS BIOS manufacturer name to match
whatever the author wanted to hardware lock the licensee to.

Use the compiled setup in \SRC.  Don't bother with anything in \GTALK
unless you have reason to.

There is more info in GTALK\DOC\README.1ST on what to do with this.
Basically:

1) Run GTALK.EXE in DOSBOX-X (mount \SRC as your C: drive in Dosbox)
2) Press escape when wait for caller screen comes up
3) Login as user "0" with "Password" (note: case sensitive)
4) type /system
5) Press "P" and set your master password.  It is currently unset.

This is NOT my work, I just made it easier to compile and run.
Borland C 3.1 is included for your convenience.  Unzip to C:\BORLANDC
or make necessary changes in the Makefile.

To compile, go to \SRC and type MAKE.  Make sure C:\BORLANDC\BIN is
in your path so that BCC.EXE can be run from \SRC.

Apparently this is public domain.

------------------------------------------------------------------------

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
