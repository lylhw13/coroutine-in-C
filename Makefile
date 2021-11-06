CC = gcc
CFLAGS = -m32 -g
LIBS = 

PROM = main.out

all: $(PROM)

main.out: coroutine.h coroutine.c handlectx.S main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm $(PROM)