; data - rdi
; len - esi
; seed - edx
; k - eax
; local - ecx
; feel free to use: r9, r8, rcx (as written in cdecl)

default rel                     ; else mistakes with lea and co.
global _murmur_asm

section .text
_murmur_asm:
    xor edx, esi                ; make h variable

    cmp esi, 3
    jle .ready_switch

    movzx r9d, si               ; with extra zeroes (assume that length are no longer than 2^16)
    and ecx, 0xFFFFFFFC         ; lower bounded to devide on four

    lea r8, [rdi + r9]          ; end of loop

.loop:
    imul    eax, dword [rdi], 1540483477
    mov     ecx, eax
    shr     ecx, 24
    add     rdi, 4
    xor     ecx, eax
    imul    eax, edx, 1540483477
    imul    edx, ecx, 1540483477
    xor     edx, eax

    cmp     rdi, r8
    jle     .loop

    and esi, 3d                 ; last two digits (range 0..2)

.ready_switch:
    lea rcx, [.switch]
    ; jmp [rsi * 8 + .switch]
    jmp [rcx + rsi * 8]

.switch:
    dq .end
    dq .branch1
    dq .branch2
    dq .branch3

.branch1:
    movzx   eax, byte [rdi]
    xor     edx, eax
    imul    edx, edx, 1540483477
    jmp     .end

.branch2:
    movzx   eax, byte [rdi + 1]
    shl     eax, 8
    xor     edx, eax
    jmp     .end

.branch3:
    movzx   eax, byte [rdi + 2]
    shl     eax, 16
    xor     edx, eax

.end:
    mov     eax, edx
    shr     eax, 13
    xor     edx, eax
    imul    eax, edx, 1540483477

    mov     edx, eax
    shr     edx, 15
    xor     eax, edx

ret
