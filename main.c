#include "coroutine.h"

// struct args{
//     int n;
// };

// void fun(struct coroutine*cor, void *para)
void fun(struct coroutine *cor)
{
    // LOGD("%s\n", __FUNCTION__);
    struct args *parg = (struct args *)(cor->args);
    int start = parg->n;
    int i;
    for (i = 0; i< 3; ++i) {
        printf("i %d\n", start + i);
        coroutine_yield(cor);
    }
    // exit(0);
}

int main()
{
    struct coroutine *cor;
    struct schedule *sch;
    sch = schedule_init();

    struct args arg1 = {0};
    struct args arg2 = {100};
    struct args arg3 = {200};

    coroutine_new(sch, fun, &arg1);
    // STAILQ_FOREACH(cor, &(sch->head), entries)
    //     LOGD("arg n is %d\n", ((struct args *)(cor->args))->n);
    coroutine_new(sch, fun, &arg2);
    // STAILQ_FOREACH(cor, &(sch->head), entries)
    //     LOGD("arg n is %d\n", ((struct args *)(cor->args))->n);
    coroutine_new(sch, fun, &arg3);
    // STAILQ_FOREACH(cor, &(sch->head), entries)
    //     LOGD("arg n is %d\n", ((struct args *)(cor->args))->n);
    schedule_run(sch);

    return 0;
}