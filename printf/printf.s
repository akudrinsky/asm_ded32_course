global start
%macro print 1-*
    %rep %0                                        ; prologue
        %rotate -1
        xor r8, r8
        mov r8, %1
        push r8
    %endrep

    call backprint

    %rep %0                                        ; epilogue
        pop r8
    %endrep
%endmacro

section .text
start:
    print format, string, 3802, 100, '!', 127

    mov rax, 0x2000001
    mov rdi, 0
    syscall

;=======================================================
; Puts: symbol to first free space in buffer
; Entry: al - char to mov to buf
;       bl - buffer size
; Destr: r10
;=======================================================
to_buffer:
    cmp bl, 64d
    jae .make_empty
    mov r10, rbx                                   ; in order not to change rbx\bl
    and r10, 1111b

.continue:
    mov rdi, print_buffer
    add rdi, r10
    stosb
    inc bl
    ret

.make_empty:
    call from_buffer
    jmp .continue

;=======================================================
; Prints everything from buffer to stdout
; Entry: bl - bytes to print
;=======================================================
from_buffer:
    cmp bl, 0d
    je .exit

    push rsi
    mov rax, 0x2000004
    mov rdi, 1
    xor rdx, rdx
    mov dl, bl
    mov rsi, print_buffer
    syscall

    xor bl, bl
    pop rsi

.exit
    ret

;=======================================================
; Entry: r13 - base of a number (f.i. 10d)
;        rax - value to print
;=======================================================
print_dec:
    xor r14, r14                                   ; reversed value
    xor rdx, rdx
    xor bh, bh                                     ; number of digits
    mov r15, rax

.reversing_loop:                                   ; reverse digits in number
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

.printing_loop:                                    ; print them
    mov rax, r14
    and rax, 1111b
    add rax, '0'
    call to_buffer

    shr r14, 4
    dec bh
    cmp bh, 0
    jne .printing_loop

    ret

;=======================================================================\
; Entry: rax - value to print
;        cl - base (1, 3, 4 for respectively %b, %o and %h)
;        r13 - corresponding base (1, 7 or 15 for respectively %b, %o, %h)
;=======================================================================/
print_boh:
    xor r14, r14                                   ; reversed value, also we will print it
    xor bh, bh                                     ; number of digits

.reversing_loop:                                   ; reverse digits in number
    mov r15, rax                                   ; digit in rax
    and r15, r13

    shl r14, cl
    add r14, r15

    shr rax, cl
    inc bh

    cmp rax, 0
    jne .reversing_loop

.printing_loop:                                    ; print them
    mov r15, r14
    and r15, r13

    add r15, '0'

    cmp r15, '9'
    jna .digit
    add r15, 'A' - '0' - 10                        ; if digit is above 9, it is 'A', 'B'...

.digit:
    mov rax, r15
    call to_buffer

    shr r14, cl
    dec bh
    cmp bh, 0
    jne .printing_loop

    ret

;=================================\
; Entry: rsi - pointer to str
; Exit:  rdx - length of that str
; Destr: al
;=================================/
str_len:
    push rsi
    xor rdx, rdx

.loop:
    lodsb
    cmp al, 0
    je .found
    inc rdx
    jmp .loop

.found:
    pop rsi
    ret

;==================================\
; Entry: rsi - str to print
;==================================/
print_str:
    call str_len
    mov rdi, 1
    mov rax, 0x2000004
    syscall
    ret

;==================================\
; Entry: rax - char to print
; Destr: rax, rdx, rdi, r10
;==================================/
print_char:
    push rcx
    call to_buffer
    pop rcx
    ret

;====================================\
; Entry: rax - int to print (signed!)
;====================================/
sign_print:
    mov r13, 1d
    shl r13, 31d                                   ; mask to get the biggest digit (i.e. sign)
    mov r14, rax
    and r14, r13                                   ; want to get the highest binary digit

    cmp r13, r14
    jne .dec_positive

    push rax                                       ; then it is negative
    mov rax, '-'
    call to_buffer
    pop rax

    not rax
    inc rax

.dec_positive:
    mov r13, 10d
    call print_dec

    ret


;==================================\
; bl - cur size of buffer (64d max)
; Destr: rax, rdi, rsi, r9, r10
;==================================/
backprint:
    push rbp
    mov rbp, rsp
    add rbp, 16d
    mov rsi, [rbp]                            ; rbp - next printf argument (so skip old rbp and return adress)

.next:
    xor rax, rax
    mov al, [rsi]                             ; al - symbol from str
    cmp al, '%'
    jne .simple

    inc rsi

    lodsb                                     ; which class?
    cmp al, '%'
    je .percent

    push rax
    call from_buffer
    pop rax

    add rbp, 8d                               ; now it is definitely a parameter ('%' but not '%%')

    mov r10, jump_table
    jmp [r10 + rax * 8]

.char:
    mov rax, [rbp]
    call print_char
    jmp .next

.str
    push rsi
    mov rsi, [rbp]
    call print_str
    pop rsi
    jmp .next

.dec:
    mov rax, [rbp]
    call sign_print
    jmp .next

.uns:
    mov r13, 10d
    mov rax, [rbp]
    call print_dec
    jmp .next

.binary:
    mov rcx, 1d
    mov r13, 1d
    mov rax, [rbp]
    call print_boh
    jmp .next

.ox:
    mov rcx, 3d
    mov r13, 111b
    mov rax, [rbp]
    call print_boh
    jmp .next

.hex:
    mov rcx, 4d
    mov r13, 1111b
    mov rax, [rbp]
    call print_boh
    jmp .next

.percent:
    mov rax, '%'
    call print_char
    jmp .next

.simple:
    cmp al, 0
    je .exit

    call to_buffer
    inc rsi

    jmp .next

.exit:
    call from_buffer
    pop rbp
    ret

section .data
print_buffer:	resb 64d
format:		db "I %s %x %d%%%c%b", 10, 0
string:		db "love", 0
symbol:		db '?'

; (98) b (0) c (0) d (10) o (3) s (1) u (2) x
jump_table:
times 98 dq backprint.next
dq          backprint.binary
dq          backprint.char
dq          backprint.dec
times 10 dq backprint.next
dq          backprint.ox
times 3  dq backprint.next
dq          backprint.str
times 2  dq backprint.next
dq          backprint.hex
times 2  dq backprint.next          ; in case user doesn't know that %y, %z matter nothing
