CC=clang
PYTHONPATH=/home/joao/miniconda3
CFLAGS=-Wall -Wextra -Wl,-rpath=${PYTHONPATH}/lib
LDFLAGS=-L${PYTHONPATH}/lib -L../
INCLUDE=-I${PYTHONPATH}/include/python3.12 -I../
CLIBS=-lpython3.12 -lm

all: example1

example1: example1.c
	$(CC) $(CFLAGS) example1.c -o example1 $(INCLUDE) $(LDFLAGS) $(CLIBS)

example2: example2.c
	$(CC) $(CFLAGS) example2.c -o example2 $(INCLUDE) $(LDFLAGS) $(CLIBS) -lcplotlib

clean:
	rm example1