global start

section .text
start:
    mov rbx, 0
    inc rax
    dec rax

    mov rax, [rip]
	
	
    call print

    mov rax, 0x2000001
    mov rdi, 0
    syscall

print:
	push rbp
	mov rbp, rsp
	add rbp, 64
	

    mov rax, rbx
    ret
