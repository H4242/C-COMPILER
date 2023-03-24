.globl	main
 main: 
	pushq 	%rbp
	movq 	%rsp, %rbp
	movl 	$41, -4(%rbp)
>> err: you want to return a variable which was not declared? looool
	movl	481818928(%rbp), %eax
	popq 	%rbp
 	ret
