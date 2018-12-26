    .intel_syntax noprefix

    .text
        .global dot_product


dot_product:

	push        ebp
	mov         ebp, esp

    push        ecx
	mov         ecx, [ebp + 8]
	push        esi
	mov         esi, [ebp + 12]
	push        edi
	mov         edi, [ebp + 16]
	push        eax
	mov         eax, 0
	cvtsi2ss    xmm0, eax
	/*
	    ecx  = N
	    esi  = addr of A
	    edi  = arrd of B
	    eax  = counter
	    xmm0 = answer
	*/
    jmp         .loop

    .loop:

        movups      xmm2, [esi + eax*4]
        movups      xmm1, [edi + eax*4]

        dpps        xmm1, xmm2, 241
        movss       xmm3, xmm1
        addss       xmm0, xmm3

        add         eax, 4
        cmp         ecx, eax
        jg         .loop
        jmp         .finalize

    .finalize:

        pop         eax
        pop         edi
        pop         esi
        pop         ecx

        sub         esp, 4
        movss       DWORD PTR [esp], xmm0
        fld         DWORD PTR [esp]

        mov		    esp, ebp
        pop		    ebp
        ret
