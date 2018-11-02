	.intel_syntax noprefix

	.text
	    .global	summ


summ:
	push ebx
	mov	ecx, [esp+8]
	summator_loop:
		mov	ebx, ecx
		dec ebx		
        mov	edx, [esp+12]
		mov	eax, [edx+4*ebx]
		mov	edx, [esp+16]
		add	eax, [edx+4*ebx]
		mov	edx, [esp+20]
		mov	[edx+4*ebx], eax
		dec	ecx
		jnz	summator_loop 
	pop ebx
	ret
