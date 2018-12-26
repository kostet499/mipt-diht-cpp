	.intel_syntax noprefix
	.text
		.global my_sin
my_sin:
	push	    ebp
	mov	        ebp, esp

    movsd	    xmm0, [ebp + 8] /* xmm0 = x   */
    movsd       xmm1, xmm0      /* xmm1 = x   */
    movsd       xmm2, xmm0      /* xmm2 = x   */
    mov         eax, -1
    cvtsi2sd    xmm6, eax       /* xmm6 = -1 */
    mov         eax, 0
    cvtsi2sd    xmm7, eax       /* xmm7 = 0 */
    push        ecx
    mov         ecx, 1
    cvtsi2sd    xmm5, ecx /* xmm5 - denumerator*/
    jmp        .loop
    .loop:
        mulsd       xmm1, xmm6  /* xmm1 - numerator */
        mulsd       xmm1, xmm2
        mulsd       xmm1, xmm2
        mov         eax, ecx
        add         eax, eax
        cvtsi2sd    xmm4, eax
        mulsd       xmm5, xmm4
        inc         eax
        cvtsi2sd    xmm4, eax
        mulsd       xmm5, xmm4
        movsd       xmm3, xmm1
        divsd       xmm3, xmm5
        addsd       xmm0, xmm3
        inc         ecx
        comisd      xmm3, xmm7
        jz          .finalize
        jmp         .loop
    pop         ecx
    .finalize:
        sub         esp, 8      /* allocates place for retval */
        movsd       [esp], xmm0
        fld	        QWORD PTR [esp]
        add         esp, 8
        leave
        ret