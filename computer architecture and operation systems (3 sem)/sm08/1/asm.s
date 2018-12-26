	.intel_syntax noprefix

	.text
		.global _start



_start:

   push     ebp
   mov      ebp, esp
   jmp      .loop

    .loop:
        mov     eax, 3
        mov     ebx, 0
        sub     esp, 4
        mov     ecx, esp
        mov     edx, 1
        int     0x80
        add     esp, 4

        cmpb     eax, 0
        je      .finalize

        mov     edx, eax
        mov     eax, 4
        mov     ebx, 1
        sub     esp, 4
        mov     ecx, esp
        int     0x80
        add     esp, 4

        jmp     .loop

    .finalize:
        leave
        mov     eax, 1
        mov     ebx, 1
        int     0x80
