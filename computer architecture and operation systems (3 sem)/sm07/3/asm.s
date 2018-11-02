    .intel_syntax noprefix
	
    .data
        scanf_fmt:
	        .string "%d"
        p_printf_fmt:
	        .long printf_fmt
        printf_fmt:
	        .string	"%d\n"

    .text
        .global everyday795
        .global summ


everyday795:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	ebx
	mov	ebx, ebp
	sub	ebx, 4
	push	ebx
	mov	ebx, offset scanf_fmt
	push	ebx
	call 	scanf
	add	esp, 8
	mov	ebx, [ebp-4]
	imul	ebx, [ebp+8]
	add	ebx, [ebp+12]
	push	ebx
	mov	ebx, p_printf_fmt
	push	ebx
	call	printf
	add	esp, 8
	pop	ebx
	mov	esp, ebp
	pop 	ebp
	ret

    
summ:
	push	ebx
	mov	ecx, N
	cycle:		
		dec	ecx
        mov	ebx, ecx		
        mov	edx, A
		mov	eax, [edx+4*ebx]
		mov	edx, B
		add	eax, [edx+4*ebx]
		mov	edx, R
		mov	[edx+4*ebx], eax
        inc ecx
		loop	cycle
	pop	ebx
	ret




