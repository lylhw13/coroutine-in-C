#include "coroutine.h"

struct schedule* schedule_init(void)
{
    struct schedule *sch = malloc(sizeof(struct schedule));
    return sch;
}

void schedule_run(struct schedule *sch)
{
    struct coroutine *ncor = STAILQ_FIRST(&(sch->head));
    STAILQ_REMOVE_HEAD(&(sch->head), entries);

    if (ncor != NULL) {
        coroutine_resume(ncor);
    }
    return;
}

void schedule_free(struct schedule *sch)
{
    struct coroutine *cor1, *cor2;
    cor1 = STAILQ_FIRST(&(sch->head));
    while (cor1 != NULL) {
        cor2 = STAILQ_NEXT(cor1, entries);
        coroutine_free(cor1);
        cor1 = cor2;
    }
    free(sch);
}

void save_regs(void)
{

}

void save_stack(void)
{

}

void swap_regs(struct context *ctx1, struct context *ctx2)
{
    /*
     * save current regs to ctx1
     * change ctx2 to current 
     */
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
    char curr;   /* the first local variable */
    char *top = cor->sch->stack + STACK_SIZE;

    /*
     * para
     * ret
     * local 
     */
    
    /*
     * ret = sp - 16
     * ebp
     * esp
     * edi
     * esi
     * ebx
     * ecx
     * edx
     */
    // cor->ctx.regs[RET] = (void*)((unsigned long)sp - 16);
    swap_regs(&(cor->ctx), &(cor->sch->ctx));


    int length = top - &curr;
    if (cor->size < length) {
        free(cor->stack);
        cor->stack = malloc(length);
        cor->size = length;
    }
    memcpy(cor->stack, &curr, length);

    cor->status = COROUTINE_SUSPEND;
    STAILQ_INSERT_TAIL(&(cor->sch->head), cor, entries);
}

void coroutine_run(struct coroutine *cor)
{
    cor->fun(cor->args);
}

void coroutine_free(struct coroutine *cor)
{
    if (cor->stack != NULL)
        free(cor->stack);
    free(cor);
}

void coroutine_resume(struct coroutine *cor)
{
    // assert()
    if(cor->status == COROUTINE_READY) {
        cor->status = COROUTINE_RUNNING;
        coroutine_run(cor);
        coroutine_free(cor);
        return;
    }
    else if (cor->status == COROUTINE_SUSPEND) {
        swap_regs(&(cor->sch->ctx), &(cor->ctx));
        memcpy(cor->sch->stack + STACK_SIZE - cor->size, cor->stack, cor->size);
        cor->status = COROUTINE_RUNNING;
        return;
    }
    exit(1);
}