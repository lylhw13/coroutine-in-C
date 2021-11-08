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
    char *top;
    int length;

    top = cor->sch->stack + STACK_SIZE;
    length = top - &curr;
    
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
    void *sp;
    void **para_addr, **ip_addr;

    sp = (void *)(ctx->ss_sp + ctx->ss_size - sizeof(void *));
    /* align stack and make space */
    sp = (char*)((unsigned long)sp & -16L);

    /* para in the stack */
    para_addr = (void**)(sp - sizeof(void *)*2);
    *para_addr = para1;

    /* make the esp point to the function ready to run  */
    ip_addr = (void **)(sp - sizeof(void *)*4);
    *ip_addr = fun;

    ctx->regs[oEIP/psize] = fun;
    ctx->regs[oESP/psize] = (void*)(sp - sizeof(void *)*4);
    return;
}
#elif defined(__x86_64__)

void makectx(struct context *ctx, void(*fun)(struct coroutine*cor), void *para1)
{
    void *sp;
    void **ip_addr; /* instruction pointer */

    sp = (void *)(ctx->ss_sp + ctx->ss_size - sizeof(void *));
    /* align stack and make space */
    sp = (char*)((unsigned long)sp & -16L);

    /* make the rsp point to the function ready to run  */
    ip_addr = (void **)sp;
    *ip_addr = fun;

    ctx->regs[oRDI/psize] = para1;
    ctx->regs[oRIP/psize] = fun;
    ctx->regs[oRSP/psize] = sp;

    return;
}
#endif

static void mainfun(struct coroutine*cor)
{
    cor->fun(cor);
    coroutine_free(cor);
    
    /* return the contorl to schedule */
    swapctx(&(cor->ctx), &(cor->sch->ctx));
}


void coroutine_resume(struct coroutine *cor)
{
    if(cor->status == COROUTINE_READY) {
        cor->status = COROUTINE_RUNNING;
        cor->ctx.ss_sp = cor->sch->stack;
        cor->ctx.ss_size = STACK_SIZE;

        makectx(&(cor->ctx), mainfun, cor);
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