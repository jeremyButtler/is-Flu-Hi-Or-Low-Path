PREFIX=/usr/local/bin

CC=cc

CFLAGS+=\
   -O3\
   --std=c89\
   -Wall\
   -static

MACCFLAGS=\
   -O3

DEBUGFLAGS+=\
   -ggdb\
   -O0\
   -Wall\
   --std=c89\
   -static

SOURCE=\
   isHiLowPathFlu.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -o isHiLowPathFlu;

mac:
	$(CC) $(MACCFLAGS) $(SOURCE) -o isHiLowPathFlu;

pythonlocal:
	CC="$(CC)" make -C pythonPkg pythonlocal;
python:
	CC="$(CC)" make -C pythonPkg python;

debug:
	$(CC) $(DEBUGFLAGS) $(source) -o hiLowDebug;
	#egdb -ex "b isHiLowPathFlu.c:303" -ex "r -fasta pythonPkg/sequences.fasta" hiLowDebug;
	egdb -ex "b isHiLowPathFlu.c:303" -ex "r -fasta pythonPkg/UP4-2018-bar07-NT.fasta" hiLowDebug;
	rm hiLowDebug;

install:
	mv isHiLowPathFlu $(PREFIX);
	chmod a+x $(PREFIX)/isHiLowPathFlu;
