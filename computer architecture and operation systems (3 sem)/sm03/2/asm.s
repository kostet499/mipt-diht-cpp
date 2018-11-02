
    .text
    .global solve

solve:

    str   r4, [sp, #-4]!
    str   r5, [sp, #-4]!
    str   r6, [sp, #-4]!
                       /* r0 -> value of 'A'         */
                       /* r1 -> value of 'B'         */
                       /* r2 -> value of 'C'         */
                       /* r3 -> value of 'D'         */
                       /* r4 -> value of expression  */
                       /* r5 -> current value of 'x' */
                       /* r6 -> current answer       */
    mov r5, #254
    b      cycle_start  

    cycle_start:
    cmp    r5, #-1
    beq    cycle_finish

    mov    r4, r0      /*  -> A                      */
    mul    r4, r4, r5  /*  -> Ax                     */
    add    r4, r4, r1  /*  -> Ax   + B               */
    mul    r4, r4, r5  /*  -> Ax^2 + Bx              */
    add    r4, r4, r2  /*  -> Ax^2 + Bx   + C        */
    mul    r4, r4, r5  /*  -> Ax^3 + Bx^2 + Cx       */
    add    r4, r4, r3  /*  -> Ax^3 + Bx^2 + Cx + D   */

    sub    r5, r5, #1
    
    cmp    r4, #0
    beq    update_answer 
    
    b      cycle_start

    update_answer:
    add    r5, r5, #1
    mov    r6, r5
    sub    r5, r5, #1
    b      cycle_start

    cycle_finish:
    mov    r0, r6
    
    ldr    r6, [sp], #4
    ldr    r5, [sp], #4
    ldr    r4, [sp], #4
       
    bx     lr
