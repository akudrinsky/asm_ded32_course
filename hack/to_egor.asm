global start

section .text

%macro print_str 0x2000004
		mov rax, 1
		mov rdi, 1
		mov rsi, %1
		mov rdx, %2
		syscall
%endmacro

start:
		call count_hash								; don't cheat!
		
		call read_str
		
		mov r10, rbx
		
		call count_hash

		call check

		mov rax, 0x2000001								; end
		mov rdi, 0
		syscall

;=========================================
;
; Returns: rbx - hash of password
;
;=========================================
count_hash:
		mov rsi, password
		xor rbx, rbx
		xor rax, rax
.loop:
		lodsb
		cmp al, 0d
		je .end

		xor rbx, 0x72816354
		add rbx, rax

		jmp .loop

.end:	
		and rbx, 0xFF
		ret


;=========================================
;
; Reads str from console input
;
;=========================================
read_str:
		mov r9, buffer

		mov	rax, 0x2000003							; put the read-system-call-code into register rax
		mov	rdi, 1
		mov	rsi, r9
		mov	rdx, 30d
		syscall

		ret

;=========================================
;
; Checks if password is correct
;
;=========================================
check:
		cmp r10, rbx
		jne .mistake

		mov rcx, pswrd_len
		mov rdi, buffer
		mov rsi, password
.loop:
		lodsb
		cmp al, 0
		je .all_right
		
		cmp al, [rdi]
		jne .mistake
		
		inc rdi
		
		jmp .loop
.mistake:
		print_str not_ok, not_ok_size
		ret

.all_right:
		print_str ok, ok_size
		ret


section .data
hash:		resb 10d
ok:			db "such a clever student", 0
ok_size:	equ $-ok
not_ok:		db "if you try one more time, you may succeed", 0
not_ok_size:equ $-not_ok
buffer:		resb 20d
password:	db "asm-ok", 0
pswrd_len:	equ $-password
