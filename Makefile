CC=clang
PYTHONPATH=/home/joao/miniconda3
CFLAGS=-Wall -Wextra -Wl,-rpath=${PYTHONPATH}/lib
LDFLAGS=-L${PYTHONPATH}/lib
INCLUDE=-I${PYTHONPATH}/include/python3.12 -I../
CLIBS=-lpython3.12 -lm

all: static

static: cplotlib.h
	@ printf '// This file was created automatically by the Cplotlib Makefile\n#define CPLOTLIB_IMPLEMENTATION\n#include "cplotlib.h"\n' > cplotlib.c
	$(CC) $(CFLAGS) -c cplotlib.c -o cplotlib.o $(INCLUDE) $(LDFLAGS) $(CLIBS)
	ar -rcs libcplotlib.a cplotlib.o
	@ echo "Cleaning temporary files"
	rm cplotlib.c
	rm cplotlib.o