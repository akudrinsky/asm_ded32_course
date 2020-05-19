; data - rdi
; len - esi
; seed - edx
; k - eax
; h - edx
; local - ecx
; feel free to use: r9, r8
global _murmur_real

_murmur_real:
	xor     edx, esi
    cmp     esi, 3
    jle     .little

    sub     esi, 4							; len - 4
    mov     r9d, esi					
    shr     r9d, 2
    mov     eax, r9d						; (len - 4) / 4
    lea     r8, [rdi + 4 + rax * 4]			; end of array

.loop:
    imul    eax, dword [rdi], 1540483477
    add     rdi, 4
    mov     ecx, eax
    shr     ecx, 24
    xor     ecx, eax
    imul    eax, edx, 1540483477
    imul    edx, ecx, 1540483477
    xor     edx, eax
    cmp     rdi, r8
    jne     .loop

    neg     r9d
    lea     esi, [rsi + r9 * 4]

.switch:
    cmp     esi, 2
    je      .second
    cmp     esi, 3
    je      .third
    cmp     esi, 1
    je      .first

.end:
    mov     eax, edx
    shr     eax, 13
    xor     edx, eax
    imul    eax, edx, 1540483477
    mov     edx, eax
    shr     edx, 15
    xor     eax, edx
    ret

.third:
    movzx   eax, byte [r8 + 2]
    shl     eax, 16
    xor     edx, eax

.second:
    movzx   eax, byte [r8 + 1]
    shl     eax, 8
    xor     edx, eax

.first:
    movzx   eax, byte [r8]
    xor     edx, eax
    imul    edx, edx, 1540483477
    jmp     .end

.little:
    mov     r8, rdi
    jmp     .switch

