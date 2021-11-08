#include "coroutine.h"

struct args{
    int n;
};

void fun(struct coroutine *cor)
{
    int i, index;
    struct args *parg;

    parg = (struct args *)(cor->args);
    index = parg->n;

    for (i = 0; i< 3; ++i) {
        printf("i %d\n", index + i);
        coroutine_yield(cor);
    }
}

int main()
{
    struct schedule *sch;
    sch = schedule_init();

    struct args arg1 = {100};
    struct args arg2 = {200};
    struct args arg3 = {300};

    coroutine_new(sch, fun, &arg1);
    coroutine_new(sch, fun, &arg2);
    coroutine_new(sch, fun, &arg3);

    schedule_run(sch);

    return 0;
}