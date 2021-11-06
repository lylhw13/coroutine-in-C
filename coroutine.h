#ifndef COROUTINE_H
#define COROUTINE_H

#include <stddef.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <string.h>

#define COROUTINE_READY     0
#define COROUTINE_RUNNING   1
#define COROUTINE_SUSPEND   2
#define COROUTINE_DEAD      3       

#define STACK_SIZE 1024*1024

typedef void *(*coroutine_fun)(void *);

enum {
    RET,
    EBP,
    ESP,
    EDI,
    ESI,
    EBX,
    ECX,
    EDX
};

typedef struct context {
    void* regs[8];

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
extern void coroutine_resume(struct coroutine *cor);
extern void coroutine_free(struct coroutine *cor);
extern void swapctx(struct context*ctx1, struct context *ctx2);
#endif