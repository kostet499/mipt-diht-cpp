
    .text
    .global calculate

calculate:

    ldrb     r2, [r0]
    ldrh     r3, [r0, #2]
    adds     r2, r2, r3
    ldr      r1, [r0, #4]
    adds     r2, r2, r1
    mov      r3, #0
    adc      r3, #0
    mov      r1, r3
    mov      r0, r2
    bx lr
