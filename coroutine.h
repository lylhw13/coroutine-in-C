#ifndef COROUTINE_H
#define COROUTINE_H

#include <stddef.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define COROUTINE_READY     0
#define COROUTINE_RUNNING   1
#define COROUTINE_SUSPEND   2

#define STACK_SIZE 1024*1024

struct coroutine;
typedef void (*coroutine_fun)(struct coroutine *cor);


typedef struct context {

#if defined(__i386__)
    void* regs[9];
#elif defined(__x86_64__)
    void* regs[17];
#endif
    void * ss_sp;
    size_t ss_size;

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

extern struct schedule* schedule_init(void);
extern void schedule_run(struct schedule *sch);
extern void schedule_free(struct schedule *sch);

extern void coroutine_new(struct schedule* sch, coroutine_fun fun, void *args);
extern void coroutine_yield(struct coroutine *cor);
extern void coroutine_resume(struct coroutine *cor);
extern void coroutine_free(struct coroutine *cor);
extern void swapctx(struct context*ctx1, struct context *ctx2);

#define LOGD(...) fprintf(stderr, __VA_ARGS__)

#endif