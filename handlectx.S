#include "ucontext_i.h"

.globl swapctx
    .type swapctx, @function
swapctx:

#if defined(__i386__)
    /* save registers to first parameter */
    movl 4(%esp), %eax     
    movl %ebx, oEBX(%eax)
    movl %ecx, oECX(%eax)
    movl %edx, oEDX(%eax)
    movl %edi, oEDI(%eax)
    movl %esi, oESI(%eax)
    movl %ebp, oEBP(%eax)
    movl %esp, oESP(%eax)
    
    /* save rip */
    movl (%esp), %ecx 
    movl %ecx, oEIP(%eax)

    /* setup registers from second parameter */
    movl  8(%esp), %eax 

    movl oESP(%eax), %esp
    /* setup rip */
    movl oEIP(%eax), %ecx
    movl %ecx, (%esp)  

    movl oEBX(%eax), %ebx
    movl oECX(%eax), %ecx
    movl oEDX(%eax), %edx
    movl oEDI(%eax), %edi 
    movl oESI(%eax), %esi
    movl oEBP(%eax), %ebp

    /* clear rax to indicate success */
    xorl %eax, %eax
    ret
    
#elif defined(__x86_64__)
    /* save registers to first parameter */
    movq %rbx, oRBX(%rdi)
    movq %rbp, oRBP(%rdi)
    movq %r12, oR12(%rdi)
    movq %r13, oR13(%rdi)
    movq %r14, oR14(%rdi)
    movq %r15, oR15(%rdi)

    movq %rdi, oRDI(%rdi)
    movq %rsi, oRSI(%rsi)
    movq %rdx, oRDX(%rdi)
    movq %rcx, oRCX(%rdi)
    movq %r8, oR8(%rdi)
    movq %r9, oR9(%rdi)

    /* save rip */
    movq (%rsp), %rcx
    movq %rcx, oRIP(%rdi)

    movq %rsp, oRSP(%rdi)


    /* setup registers from second parameter */
    movq oRBX(%rsi), %rbx
    movq oRBP(%rsi), %rbp
    movq oR12(%rsi), %r12
    movq oR13(%rsi), %r13 
    movq oR14(%rsi), %r14
    movq oR15(%rsi), %r15

    movq oRSP(%rsi), %rsp
    /* setup rip */
    movq oRIP(%rsi), %rcx
    movq %rcx, (%rsp)

    movq oRDI(%rsi), %rdi
    movq oRDX(%rsi), %rdx
    movq oRCX(%rsi), %rcx 
    movq oR8(%rsi), %r8 
    movq oR9(%rsi), %r9 
 
    movq %rsi, %r10
    movq oRSI(%r10), %rsi

    /* clear rax to indicate success */
    xorl %eax, %eax
    ret

#endif