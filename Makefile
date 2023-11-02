PREFIX=/usr/local/bin

CC=cc

CFLAGS+=\
   -O3\
   --std=c99\
   -Wall\
   -static

DEBUGFLAGS+=\
   -ggdb\
   -O0\
   -Wall\
   --std=c99\
   -static

source=\
   isHiLowPathFlu.c

all:
	$(CC) $(CFLAGS) $(source) -o isHiLowPathFlu;

pythonlocal:
	CC="$(CC)" make -C pythonPkg pythonlocal;
python:
	CC="$(CC)" make -C pythonPkg python;

debug:
	$(CC) $(DEBUGFLAGS) $(source) -o hiLowDebug;
	egdb -ex "b isHiLowPathFlu.c:142" -ex "r -HA2-start 1026 -fasta UP4-2018-bar07.fasta" hiLowDebug;
	rm hiLowDebug;

install:
	mv isHiLowPathFlu $(PREFIX);
	chmod a+x $(PREFIX)/isHiLowPathFlu;
