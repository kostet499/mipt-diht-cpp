	.text
	.global classify

classify:

	push	{r1, r2, r3, r4, r5, r6}
	
	ldr		r5, =0x7FF	 /* constant value to check E */
	
	ldr		r2, [r0]     /* load last 32 bits of M*/
	ldr		r1, [r0, #4] /* load S, E and first 20 bits of M */

	lsr		r3, r1, #31  /* get S */
	
	cmp		r3, #0
	beq		handle_unsigned
	b		handle_signed

	handle_unsigned:
		mov		r3, r1
	    lsr		r6, r3, #20
		cmp		r6, #0
		beq		return_PlusZero
		mov		r4, r3
		lsr		r3, r3, #20	/* get E  */
		cmp		r3, r5
		beq		return_PlusInf
		b		return_PlusRegular

	handle_signed:
		lsl		r3, r3, #31	  
		eor		r3, r3, r1      /* get r1 without S  */
		lsr		r6, r3, #20
		cmp		r6, #0
		beq		return_MinusZero
		mov		r4, r3
		lsr		r3, r3, #20	/* get E  */
		cmp		r3, r5
		beq		return_MinusInf
		b		return_MinusRegular		

	/* PlusZero = 0x00 */
	return_PlusZero:
		cmp		r2, #0
		bne		return_PlusDenormal
        mov		r4, r3 
		lsr		r4, r4, #20
		lsl		r4, r4, #20
		eor		r4, r3, r4
		cmp		r4, #0
		bne		return_PlusDenormal
		mov     r0, #0x00  
        b       return_success

	/* MinusZero = 0x01 */
	return_MinusZero:
		cmp		r2, #0
		bne		return_MinusDenormal
	    mov		r4, r3
		lsr		r4, r4, #20
		lsl		r4, r4, #20
		eor		r4, r3, r4
		cmp		r4, #0
		bne		return_MinusDenormal
		mov     r0, #0x01  
        b       return_success

	/* PlusInf = 0xF0 */
	return_PlusInf:
		cmp		r2, #0
		bne		return_SignalingNaN
        mov     r3, r4
        lsr     r4, r4, #20
        lsl     r4, r4, #20
        eor     r4, r3, r4
        cmp     r4, #0
		bne		return_SignalingNaN
        mov     r0, #0xF0  
        b       return_success

	/* MinusInf = 0xF1 */
	return_MinusInf:
		cmp		r2, #0
		bne		return_QuietNaN
        mov     r3, r4
        lsr     r4, r4, #20
        lsl     r4, r4, #20
        eor     r4, r3, r4
        cmp     r4, #0
        bne     return_QuietNaN
        mov     r0, #0xF1 
        b       return_success

	/* PlusRegular = 0x10 */
	return_PlusRegular:
        mov     r0, #0x10
        b       return_success

	/* MinusRegular  = 0x11 */
	return_MinusRegular:
        mov     r0, #0x11
        b       return_success

	/* PlusDenormal = 0x20 */
	return_PlusDenormal:
        mov     r0, #0x20
        b       return_success
	
	/* MinusDenormal = 0x21 */
	return_MinusDenormal:
        mov     r0, #0x21
        b       return_success

	/* SignalingNaN = 0x30 */
	return_SignalingNaN:
        mov     r0, #0x30
        b       return_success

	/* QuietNaN = 0x31 */
	return_QuietNaN:
		mov		r0, #0x31	
		b		return_success

	return_success:
		pop		{r1, r2, r3, r4, r5, r6}
		bx 		lr	
