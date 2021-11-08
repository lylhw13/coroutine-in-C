CC = gcc
CFLAGS = -Wall

ifdef DEBUG
CFLAGS += -g
endif

LIBS = 

PROM = main.out main32.out

SRCFILE = ucontext_i.h coroutine.h coroutine.c handlectx.S main.c

all: $(PROM)

main.out: $(SRCFILE)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

main32.out: $(SRCFILE)
	$(CC) $(CFLAGS) -m32 $^ -o $@ $(LIBS)

clean:
	rm $(PROM)

# make DEBUG=1