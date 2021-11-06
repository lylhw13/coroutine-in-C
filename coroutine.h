#ifndef COROUTINE_H
#define COROUTINE_H

#include <stddef.h>
#include <sys/queue.h>

#define COROUTINE_READY     0
#define COROUTINE_RUNNING   1
#define COROUTINE_SUSPEND   2
#define COROUTINE_DEAD      3       

#define STACK_SIZE 1024*1024

typedef void *(*coroutine_fun)(void *);

typedef struct context {
    void* regs[7];

}context_t;

struct schedule;

typedef struct coroutine {
    coroutine_fun fun;
    void *args;
    struct context ctx;
    struct schedule *sch;
    char *stack;
    ssize_t size;
    int status;
    STAILQ_ENTRY(coroutine) entries;
} coroutine_t;

STAILQ_HEAD(listhead, coroutine);

typedef struct schedule {
    char stack[STACK_SIZE];
    struct listhead head;
    struct context ctx;
}schedule_t;

extern void schedule_init(void);
#endif