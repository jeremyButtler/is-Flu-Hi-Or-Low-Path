CC=cc

python:
	CFLAGS="$(CFLAGS)" CC="$(CC)" python3 setup.py install;
	rm -r build;
	rm -r dist;
	rm -r *.*egg*;

pythonlocal:
	CFLAGS="$(CFLAGS)" CC="$(CC)" python3 setup.py build;
	pip3 install --user .;
	rm -r build;
	rm -r *.*egg*;

# This is set up for openbsd. Linux or bsd would replace
# egdb with gdb
debug:
	CC="$(CC)" python3 setup-debug.py build;
	pip install .;
	egdb -ex "r test.py" "b pyIsHiLowPathFlu.c:114" --args python3
	rm -r build;
	rm -r flue*.egg*;
