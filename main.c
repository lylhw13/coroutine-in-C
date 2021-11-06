#include "coroutine.h"

struct args{
    int n;
};

void fun(struct coroutine*cor, void *para)
{
    struct args *parg = (struct args *)para;
    int start = parg->n;
    int i;
    for (i = 0; i< 3; ++i) {
        printf("%d\n", start + i);
        coroutine_yield(cor);
    }
}

int main()
{
    struct schedule *sch;
    sch = schedule_init();

    struct args arg1= {0};
    struct args arg2 = {100};

    coroutine_new(sch, fun, &arg1);
    coroutine_new(sch, fun, &arg2);
    schedule_run(sch);

    return 0;
}