;===============================================================================
; > check_password
;===============================================================================

            default rel

            extern  _MurmurHash64B
            global  check_password

            extern  get_string

            section .text

;###############################################################################
;# Checks user password
;# ENTRY: RDI <- user password hash
;# EXIT:  ZF <- 0 if correct
;# DESTR: RAX RDI RSI RDX
;###############################################################################

check_password:

            push    rbp
            mov     rbp, rsp
            sub     rsp, BUFFER_SIZE + 16

            mov     qword [rbp - 8], rsp
            mov     qword [rbp - 16], rdi

            mov     al, 0
            mov     rdi, rsp
            mov     rcx, BUFFER_SIZE
            rep     stosb

            mov     rsi, rsp
            call    get_string

            xor     qword [rbp - 8], rsp
            jne     .stack_smashed

            mov     rsi, rsp
            mov     rdi, rsp
            mov     rsi, BUFFER_SIZE
            mov     rdx, HASH_SEED

            call    _MurmurHash64B

            mov     rdi, qword [rbp - 16]

            add     rsp, BUFFER_SIZE + 16
            pop     rbp

            xor     rax, rdi
            ret

.stack_smashed:

            mov     rax, 0x2000001
            mov     rdi, 10
            syscall
            

;-------------------------------------------------------------------------------

            section .data

HASH_SEED equ 0xffaabbcc
BUFFER_SIZE equ 256

;###############################################################################

