#include "coroutine.h"

void schedule_init(void)
{
    struct schedule *sch = malloc(sizeof(struct schedule));

}

void schedule_run(void)
{
    return;
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
    cor->status = COROUTINE_READY;
    STAILQ_INSERT_TAIL(&(sch->head), cor, entries);
}

void coroutine_yield(struct coroutine *cor)
{
    char *sp;
    /*
     * ret = sp - 16
     * ebp
     * esp
     *  
     */

}

void coroutine_resume()
{

}