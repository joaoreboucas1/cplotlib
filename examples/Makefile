CC=clang
PYTHONPATH=/home/joao/miniconda3
CFLAGS=-Wall -Wextra -Wl,-rpath=${PYTHONPATH}/lib
LDFLAGS=-L${PYTHONPATH}/lib
INCLUDE=-I${PYTHONPATH}/include/python3.12 -I../
CLIBS=-lpython3.12 -lm

all: example1

example1: example1.c
	$(CC) $(CFLAGS) example1.c -o example1 $(LDFLAGS) $(INCLUDE) $(CLIBS)