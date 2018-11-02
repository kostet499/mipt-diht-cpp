    .text
    .global main

main:
	push 	{lr}
	
	loop_begin:
		ldr 	r0, =stdin
		ldr		r0, [r0]
		bl		fgetc
		cmp		r0, #-1
		beq		loop_end
		cmp		r0, #57
		bgt		loop_begin
		b		loop_continue
		loop_continue:
			cmp		r0, #48
			blt		loop_begin
			ldr 	r1, =stdout
			ldr		r1, [r1]
			bl		fputc
			b		loop_begin
	
	loop_end:
		pop 	{pc}
