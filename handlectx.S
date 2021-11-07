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
    movq %rbp, oRBP(%rdi)
    movq %r12, oR12(%rdi)
    movq %r13, oR13(%rdi)
    movq %r14, oR14(%rdi)
    movq %r15, oR15(%rdi)

    movq %rdi, oRDI(%rdi)
    movq %rsi, oRSI(%rdi)
    movq %rdx, oRDX(%rdi)
    movq %rcx, oRCX(%rdi)
    movq %r8, oR8(%rdi)
    movq %r9, oR9(%rdi)

    movq (%rsp), %rcx
    movq %rcx, ORIP(%rdi)
    leaq 8(%rsp), %rcx
    movq %rcx, oRSP(%rdi)



    xorl %eax, %eax
    ret

#endif