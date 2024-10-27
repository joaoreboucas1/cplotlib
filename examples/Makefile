CC=clang
CFLAGS=-Wall -Wextra -Wl,-rpath=/home/joao/miniconda3/lib/
PYTHONPATH=/home/joao/miniconda3/
LDFLAGS=-L${PYTHONPATH}/lib/
INCLUDE=-I${PYTHONPATH}/include/python3.12 -I${PYTHONPATH}/include/python3.12 -I../
CLIBS=-lpython3.12 -lpthread -ldl  -lutil -lm

all: example1

example1: example1.c
	$(CC) $(CFLAGS) example1.c -o example1 $(LDFLAGS) $(INCLUDE) $(CLIBS)