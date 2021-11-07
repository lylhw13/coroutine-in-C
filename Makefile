CC = gcc
CFLAGS = -g
CFLAGS32 = -m32 -g
LIBS = 

PROM = main.out main32.out

all: $(PROM)

main.out: ucontext_i.h coroutine.h coroutine.c handlectx.S main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

main32.out: ucontext_i.h coroutine.h coroutine.c handlectx.S main.c
	$(CC) $(CFLAGS32) $^ -o $@ $(LIBS)

clean:
	rm $(PROM)