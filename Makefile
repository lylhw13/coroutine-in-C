CC = gcc
CFLAGS = -g
LIBS = 

PROM = coroutine

all: $(PROM)

coroutine: coroutine.h coroutine.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm $(PROM)