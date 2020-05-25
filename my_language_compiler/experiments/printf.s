global start

section .text
start:
    pop rax

    mov r13, 1d
    shl r13, 31d
    mov r14, rax
    and r14, r13
    cmp r13, r14
    jne .dec_positive

    push rax
    mov rax, '-'
    call to_buffer
    pop rax

    not rax
    inc rax

.dec_positive:
    mov r13, 10d
    xor r14, r14
    xor rdx, rdx
    xor bh, bh
    mov r15, rax

.reversing_loop:
    mov rax, r15
    div r13d

    shl r14, 4
    add r14, rdx

    mov r15, rax
    inc bh

    cmp r15, r13
    jae .reversing_loop

    shl r14, 4
    add r14, r15
    inc bh

.printing_loop:
    mov rax, r14
    and rax, 1111b
    add rax, '0'
    call to_buffer

    shr r14, 4
    dec bh
    cmp bh, 0
    jne .printing_loop

