.data
	.global R
	R: .space 4
	
	
.text
	.global calculate


calculate:
    push	{r1, r2, r3, r4, r5}
    ldr		r0, .Lexterns		/* get address of A  */
    ldr		r0, [r0]			/* load A by address */
    ldr		r1, .Lexterns+4		/* get address of B  */
    ldr		r1, [r1]			/* load B by address */
    mul		r1, r0, r1			/* r1 <- A * B       */
    ldr		r2, .Lexterns+8		/* get address of C  */
    ldr		r2, [r2]			/* load C by address */
    ldr		r3, .Lexterns+12	/* get address of D  */
    ldr		r3, [r3]			/* load D by address */
    mul		r3,	r2, r3			/* r3 <- C * D       */
    add		r4, r1, r3			/* r4 <- A*B + C*D   */
    ldr		r5, .Lexterns+16	/* get address of R  */
    str		r4, [r5]			/* R <-r4 by address */
    pop		{r1, r2, r3, r4, r5}
    bx		lr


.Lexterns:
	.word A
	.word B
	.word C
	.word D
	.word R
