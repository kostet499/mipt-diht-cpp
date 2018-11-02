	.data
	scanf_fmt:
	.asciz "%d"
	.align 2
	printf_fmt:
	.asciz "%d\n"
	.align 2
	num1:
	.word 0
	num2:
	.word 0

	.text
	.global main

main:
	push 	{ip, lr}
	ldr 	r1, =num1
	ldr 	r0, =scanf_fmt
	bl 		scanf
	ldr 	r1, =num2
	ldr 	r0, =scanf_fmt
	bl 		scanf
	ldr 	r2, =num1
	ldr 	r0, [r2]
	ldr 	r2, =num2
	ldr 	r1, [r2]
	add 	r1, r1, r0
	ldr 	r0, =printf_fmt
	bl 		printf
	pop 	{ip, pc}
