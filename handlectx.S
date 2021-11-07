#include "ucontext_i.h"

.globl swapctx
    .type swapctx, @function
swapctx:

#if defined(__i386__)
    movl 4(%esp), %eax     
    movl %ebx, oEBX(%eax)
    movl %ecx, oECX(%eax)
    movl %edx, oEDX(%eax)
    movl %edi, oEDI(%eax)
    movl %esi, oESI(%eax)
    movl %ebp, oEBP(%eax)
    movl %esp, oESP(%eax)

    movl  8(%esp), %eax     
    movl oEBX(%eax), %ebx
    movl oECX(%eax), %ecx
    movl oEDX(%eax), %edx
    movl oEDI(%eax), %edi 
    movl oESI(%eax), %esi
    movl oEBP(%eax), %ebp
    movl oESP(%eax), %esp

    ret
#elif defined(__x86_64__)
    movq %rbx, oRBX(%rdi)

#endif