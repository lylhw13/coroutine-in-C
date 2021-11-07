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
#define COROUTINE_DEAD      3       

#define STACK_SIZE 1024*1024

struct coroutine;
typedef void (*coroutine_fun)(struct coroutine *cor);

#if defined(__i386__)
enum {
    EAX,
    EBX,
    ECX,
    EDX,
    EDI,
    ESI,
    EBP,
    ESP
};
#elif defined(__x86_64__)
enum {
    RAX,
    RBX,
    RCX,
    RDX,
    RSI,
    RDI,
    RBP,
    RSP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15
};
#endif

typedef struct context {

#if defined(__i386__)
    void* regs[8];
#elif defined(__x86_64__)
    void* regs[18];
#endif
    // char *stack;
    // size_t ss_size;
}context_t;

struct args{
    int n;
};

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