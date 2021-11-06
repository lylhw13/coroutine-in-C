# coroutine

have one pending list
don't need lock for only one coroutine is running at one time




ct_run
    add

ct_yield
    add itself to the list
    make_ctx


ct_resume 
    remove itself from the list
    swap_ctx

libco is combine epoll with coroutine
like libev
timeout list
ready list
suspend list

here we just want to display how to write coroutine

stackful

stackless