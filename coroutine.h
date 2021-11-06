#ifndef COROUTINE_H
#define COROUTINE_H

#define COROUTINE_READY     0 /* need initialize */
#define COROUTINE_RUNNING   1/* can't be resume */
#define COROUTINE_SUSPEND   2
#define COROUTINE_DEAD      3          

typedef struct ctx {
    void* regs[7];

}ctx_t;

typedef struct schedule {

}schedule_t;

#endif