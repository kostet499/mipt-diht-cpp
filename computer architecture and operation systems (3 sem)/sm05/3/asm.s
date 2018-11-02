    .text
		.global main
main:
	push	{lr}
	push	{r9}
	mov		r9, #0
	b		loop_reader_begin
	loop_reader_begin:
		ldr 	r0, =stdin			/* reset stream address for fgetc */
		ldr		r0, [r0]			/* set stream */
		bl		fgetc
		cmp		r0, #-1				/* while EOF is not reached */
		beq		loop_reader_end
		push	{r0}
		add		r9, r9, #1
		b		loop_reader_begin
	loop_reader_end:
		b		loop_writer_begin
		loop_writer_begin:		
			sub		r9, r9, #1
			cmp		r9, #-1
			beq		loop_writer_end
			pop		{r0}
			ldr 	r1, =stdout		/* reset stream address for fputc */
			ldr		r1, [r1]		/* set stream */
			bl		fputc
			b		loop_writer_begin	
		loop_writer_end:
			pop		{r9}
			pop		{pc}
			bx		lr
