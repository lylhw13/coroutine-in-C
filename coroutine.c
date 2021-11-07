#include "coroutine.h"
#include "ucontext_i.h"

static int psize = sizeof(void *);

struct schedule* schedule_init(void)
{
    struct schedule *sch = malloc(sizeof(struct schedule));
    memset(sch, 0, sizeof(struct schedule));
    STAILQ_INIT(&(sch->head));
    return sch;
}

void schedule_run(struct schedule *sch)
{
    while (1) {
        struct coroutine *cor;
        cor = STAILQ_FIRST(&(sch->head));
        if (cor == NULL)
            break;
        STAILQ_REMOVE_HEAD(&(sch->head), entries);
        coroutine_resume(cor);
    }

    LOGD("%s\n", "after all");
    schedule_free(sch);
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

    int length = top - &curr;
    if (cor->size < length) {
        if (cor->stack != NULL)
            free(cor->stack);
        cor->stack = malloc(length);
        cor->size = length;
    }
    memcpy(cor->stack, &curr, length);

    cor->status = COROUTINE_SUSPEND;
    STAILQ_INSERT_TAIL(&(cor->sch->head), cor, entries);

    swapctx(&(cor->ctx), &(cor->sch->ctx));
}



void coroutine_free(struct coroutine *cor)
{
    if (cor->stack != NULL)
        free(cor->stack);
    free(cor);
}

#if defined(__i386__)
void makectx(struct context *ctx, void(*fun)(struct coroutine*cor), void *para1)
{
    /*
     * para
     * ret
     * local 
     */
    /* make the ctx to ready run */
    // LOGD("%s\n", __FUNCTION__);
    struct coroutine *cor = (struct coroutine *)para1;

    void *sp;
    sp = (char *)(cor->sch->stack + STACK_SIZE);
    /* align stack and make space */
    sp = (char*)((unsigned long)sp & -16L);

    void **para = (void**)(sp - sizeof(void *)*2);
    *para = (void*)cor;

    void **ret_addr;
    ret_addr = (void**)(sp - sizeof(void *)*4);
    *ret_addr = (void *)fun;

    ctx->regs[oESP/psize] = (void*)(sp - sizeof(void *)*4);
    return;
}
#elif defined(__x86_64__)

void makectx(struct context *ctx, void(*fun)(struct coroutine*cor), void *para1)
{
    struct coroutine *cor = (struct coroutine *)para1;

    void *sp;
    sp = (char *)(cor->sch->stack + STACK_SIZE - sizeof(void *) * 2);
    /* align stack and make space */
    sp = (char*)((unsigned long)sp & -16LL);

    void **ret_addr;
    ret_addr = (void **)sp;
    *ret_addr = (void*)fun;

    ctx->regs[oRDI/psize] = (void*)cor;     /* para */
    ctx->regs[oRIP/psize] = (void *)fun;
    ctx->regs[oRSP/psize] = (void*)sp;

    return;
}
#endif

static void mainfun(struct coroutine*cor)
{
    // cor->fun(cor, cor->args);
    cor->fun(cor);
    coroutine_free(cor);
    LOGD("%s\n", "after run");
    // exit(0);
    swapctx(&(cor->ctx), &(cor->sch->ctx));
}


void coroutine_resume(struct coroutine *cor)
{
    // LOGD("%s\n", __FUNCTION__);
    // LOGD("%s %d\n",__FUNCTION__, ((struct args*)(cor->args))->n);


    if(cor->status == COROUTINE_READY) {
        cor->status = COROUTINE_RUNNING;
        makectx(&(cor->ctx), mainfun, cor);
        // LOGD("swapctx\n");
        swapctx(&(cor->sch->ctx), &(cor->ctx));
        return;
    }
    else if (cor->status == COROUTINE_SUSPEND) {
        cor->status = COROUTINE_RUNNING;
        memcpy(cor->sch->stack + STACK_SIZE - cor->size, cor->stack, cor->size);
        swapctx(&(cor->sch->ctx), &(cor->ctx));
        return;
    }
    // exit(1);
}