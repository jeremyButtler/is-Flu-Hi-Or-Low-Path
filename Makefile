PREFIX=/usr/local/bin

CC=cc

CFLAGS= \
   -O3 \
   -std=c89 \
   -Wall \
   -Wextra \
   -static

MACCFLAGS=\
   -O3

DEBUGFLAGS=\
   -ggdb\
   -g \
   -O0\
   -Wall\
   -Wextra \
   -std=c89\
   -static

SOURCE=\
   memwater/alnSetST.c \
   memwater/seqST.c \
   memwater/memwater.c \
   haStart.c \
   haPath.c \
   getHaPath.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -o getHaPath;

mac:
	$(CC) $(MACCFLAGS) $(SOURCE) -o getHaPath;

#pythonlocal:
#	CC="$(CC)" make -C pythonPkg pythonlocal;
#python:
#	CC="$(CC)" make -C pythonPkg python;

check:
	$(CC) $(DEBUGFLAGS) $(SOURCE) -o getHaPath;

install:
	mv getHaPath $(PREFIX);
	chmod a+x $(PREFIX)/getHaPath;
