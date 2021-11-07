CC = gcc
# CFLAGS = -m32 -g
CFLAGS = -g
LIBS = 

PROM = main.out

all: $(PROM)

main.out: ucontext_i.h coroutine.h coroutine.c handlectx.S main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm $(PROM)