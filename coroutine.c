#include "coroutine.h"

void schedule_init(void)
{
    struct schedule *sch = malloc(sizeof(struct schedule));

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
    void *sp;   /* the first local variable */
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
    save_stack();
    STAILQ_INSERT_TAIL(&(cor->sch->head), cor, entries);
}

void coroutine_resume(struct coroutine *cor)
{
    // assert()
    if(cor->status == COROUTINE_READY) {

    }

}