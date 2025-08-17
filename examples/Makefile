CC=clang
PYTHONPATH=/usr/
CFLAGS=-Wall -Wextra -Wl,-rpath=${PYTHONPATH}/lib
LDFLAGS=-L${PYTHONPATH}/lib -L../
INCLUDE=-I${PYTHONPATH}/include/python3.12 -I../
CLIBS=-lpython3.12 -lm

all: example1

example1: example1.c
	$(CC) $(CFLAGS) example1.c -o example1 $(INCLUDE) $(LDFLAGS) $(CLIBS)

clean:
	rm example1