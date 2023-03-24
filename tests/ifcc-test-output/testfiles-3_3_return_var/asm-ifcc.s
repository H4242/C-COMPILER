.globl	main
 main: 
	pushq 	%rbp
	movq 	%rsp, %rbp
	movl 	$41, -4(%rbp)
	movl 	$42, -8(%rbp)
	movl	-8(%rbp), %eax
	popq 	%rbp
 	ret
