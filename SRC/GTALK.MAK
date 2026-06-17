.AUTODEPEND

#		*Translator Definitions*
CC = bcc +GTALK.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = C:\PROG\BORLANDC\LIB
INCLUDEPATH = C:\PROG\BORLANDC\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 com.obj \
 command.obj \
 gt.obj \
 task.obj \
 user.obj \
 video.obj

#		*Explicit Rules*
gtalk.exe: gtalk.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/P-/L$(LIBPATH) @&&|
c0l.obj+
com.obj+
command.obj+
gt.obj+
task.obj+
user.obj+
video.obj
gtalk
		# no map file
emu.lib+
mathl.lib+
cl.lib
|


#		*Individual File Dependencies*
com.obj: gtalk.cfg ..\..\..\test\com.c 
	$(CC) -c ..\..\..\test\com.c

command.obj: gtalk.cfg ..\..\..\test\command.c 
	$(CC) -c ..\..\..\test\command.c

gt.obj: gtalk.cfg ..\..\..\test\gt.c 
	$(CC) -c ..\..\..\test\gt.c

task.obj: gtalk.cfg ..\..\..\test\task.c 
	$(CC) -c ..\..\..\test\task.c

user.obj: gtalk.cfg ..\..\..\test\user.c 
	$(CC) -c ..\..\..\test\user.c

video.obj: gtalk.cfg ..\..\..\test\video.c 
	$(CC) -c ..\..\..\test\video.c

#		*Compiler Configuration File*
gtalk.cfg: gtalk.mak
  copy &&|
-ml
-A
-G
-O
-Ol
-vi-
-w-ret
-w-nci
-w-inl
-wpin
-wamb
-wamp
-w-par
-wasm
-wcln
-w-cpt
-wdef
-w-dup
-w-pia
-wsig
-wnod
-w-ill
-w-sus
-wstv
-wucp
-wuse
-w-ext
-w-ias
-w-ibc
-w-pre
-w-nst
-I$(INCLUDEPATH)
-L$(LIBPATH)
| gtalk.cfg


