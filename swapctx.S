.globl swapctx
    .type swapctx, @function
swapctx:
    movl 4(%esp), %eax     ; first para ctx1
    movl %ebp, 28(%eax)
    movl %esp, 24(%eax)
    movl %edi, 20(%eax)
    movl %esi, 16(%eax)
    movl %ebx, 12(%eax)
    movl %ecx, 8(%eax)
    movl %edx, 4(%eax)

    movl 8(%esp), %eax     ; second para ctx2
    movl 4(%eax), %edx
    movl 8(%eax), %ecx
    movl 12(%eax), %ebx
    movl 16(%eax), %esi 
    movl 20(%eax), %edi
    movl 24(%eax), %esp
    movl 28(%eax), %ebp

    ret