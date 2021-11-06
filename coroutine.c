#include "coroutine.h"

void schedule_init(void)
{
    struct schedule *sch = malloc(sizeof(struct schedule));

}

void coroutine_new(struct schedule* sch, coroutine_fun fun, void *args)
{
    struct coroutine *cor = malloc(sizeof(struct coroutine));
    memset(cor, 0, sizeof(struct coroutine));
    cor->fun = fun;
    cor->args = args;
    cor->sch = sch;
    cor->stack = NULL;
    cor->size = 0;
    cor->statuc = COROUTINE_READY;
}