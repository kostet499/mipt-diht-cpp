
    .text
    .global sort_by_age


sort_by_age:
	
	b		start_sort	
	
	swap_persons:
		/* r4 - person #1    */
		/* r5 - person #2    */
		/* r6 - byte from #1 */
		/* r7 - byte from #2 */
		/* r8 - loop counter */
		mov 	r8, #0
		swap_loop_begin:
			cmp 	r8, #12
			beq		swap_loop_end
			ldr		r6, [r4]
			ldr		r7, [r5]
			str		r6, [r5]
			str		r7, [r4]
			add		r4, r4, #4
			add		r5, r5, #4
			add		r8, r8, #1
			b		swap_loop_begin
		swap_loop_end:
			b		after_swap

	start_sort:
		/* r2 - counter for outter loop */
		/* r3 - counter for inner  loop */
		/* r4-r8 - for swap_persons     */
		push 	{r2, r3, r4, r5, r6, r7, r8, r9}
		/* r9 - offset for structures   */
		mov		r9, #48
		mov		r2, r0
		sub		r2, r2, #1
		b 		outter_loop_begin
		outter_loop_begin:
			mov		r3, #0
			cmp 	r2, #0
			bge		inner_loop_begin
			b		outter_loop_end
			
			inner_loop_begin:
				cmp 	r3, r2
				beq		inner_loop_end
				
				mov		r4, r3
				mul		r4, r4, r9
				add		r4, r4, r1
				add		r4, r4, #4
				mov		r5, r4
				ldrb	r6, [r4]
				add		r5, r5, r9
				ldrb	r7, [r5]
				
				sub		r4, r4, #4
				sub		r5, r5, #4
				
				cmp 	r6, r7
				bgt		swap_persons
				b		after_swap
				
				after_swap:	
					add		r3, r3, #1
					b		inner_loop_begin
					
			inner_loop_end:
				sub		r2, r2, #1
				b		outter_loop_begin
				
		outter_loop_end:
			pop 	{r2, r3, r4, r5, r6, r7, r8, r9}
			bx		lr		

