
    .text
    .global check_int

check_int:

	push 	{r1, r2, r3}
	ldr		r3, =511
	lsr		r2, r0, #24
	cmp		r2, #0
	beq		return_success
	
	mov 	r1, #1

	cycle:
		lsr 	r2, r0, #31
		cmp 	r2, #1
		beq 	end_cycle
		lsl 	r0, r0, #1
		lsl 	r1, r1, #1
		cmp 	r1, r3
		bgt 	return_success
		b 		cycle

	end_cycle:
		lsl		r0, r0, #24
		cmp		r0, #0
		beq 	return_success
		b		return_failure
	
	return_success:
		mov		r0, #1
		b		function_exit
		
	return_failure:
		mov		r0, #0
		b		function_exit
		
	function_exit:
		pop 	{r1, r2, r3}
		bx     	lr
