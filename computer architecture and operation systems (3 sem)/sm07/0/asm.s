	.intel_syntax noprefix

	.text
		.global calc
		.global vector_sum


calc:
	push		ebp
	mov			ebp, esp
	sub			esp, 8
	movsd		xmm0, [ebp + 16]
	addsd		xmm0, [ebp + 8]
	cvtsi2sd	xmm1, [ebp + 32]
	addsd		xmm1, [ebp + 24]
	divsd		xmm0, xmm1
	movsd		[esp], xmm0
	fld			QWORD PTR [esp]
	mov			esp, ebp
	pop			ebp
	ret


vector_sum:
	push	ebp
	mov		ebp, esp
	push	edi
	mov		edi, [ebp + 12]
	push	esi
	mov		esi, [ebp + 16]
	push	ebx
	mov		ebx, 0
	push	eax
	mov		eax, [ebp + 20]
	push	ecx
	mov		ecx, [ebp + 8]
	summator_loop:
		movaps 	xmm0, [esi + ebx * 4]
		movaps	xmm1, [edi + ebx * 4]
		addps	xmm0, xmm1
		movaps	[eax + ebx * 4], xmm0
		add		ebx, 4
		cmp		ebx, ecx
		jl		summator_loop
	pop		ecx
	pop		eax
	pop 	ebx
	pop 	esi
	pop		edi
	mov		esp, ebp
	pop		ebp
	ret

