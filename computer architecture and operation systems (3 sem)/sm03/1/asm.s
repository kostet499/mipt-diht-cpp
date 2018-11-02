
    .text
    .global summ

summ:

    cycle_start:
    cmp    r1, #0
    beq    cycle_finish
    ldr    r4, [r2]
    add    r0, r0, r4
    add    r2, r2, #4
    sub    r1, r1, #1
    b      cycle_start

    cycle_finish:   
    bx     lr
