.globl	main
 main: 
	pushq 	%rbp
	movq 	%rsp, %rbp
	movl 	$41, -4(%rbp)
	movl	-4(%rbp), %eax
	popq 	%rbp
 	ret
