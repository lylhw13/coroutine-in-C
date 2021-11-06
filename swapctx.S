.globl swapctx
    .type swapctx, @function
swapctx:
    movl 4(%esp), %eax     ; first para ctx1
    movl %esp, 28(%eax)
    movl %ebp, 24(%eax)
    movl %esi, 20(%eax)
    movl %edi, 16(%eax)
    movl %edx, 12(%eax)
    movl %ecx, 8(%eax)
    movl %ebx, 4(%eax)

    movl  8(%esp), %eax     ; second para ctx2
    movl  4(%eax), %ebx
    movl  8(%eax), %ecx
    movl 12(%eax), %edx
    movl 16(%eax), %edi 
    movl 20(%eax), %esi
    movl 24(%eax), %ebp
    movl 28(%eax), %esp

    ret