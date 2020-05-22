global start

section .text
start:
	push rax
	push rbx

    	call print

	add rsp, 16
	
	push 0
	push 1000h
	mov rax, [rbp + 100h]

	pop rax
	mov [rbp + 100h], rax

    	mov rax, 0x2000001
    	mov rdi, 0
    	syscall

print:
	pop rax
	push rsi
	add rsi, 9
  	mov ebx, 10   
.next_digit:
  	xor edx, edx       
  	div ebx
  	add dl,'0'
  	dec rsi
  	mov [rsi], dl
  	test eax, eax            
  	jnz .next_digit

	mov rax, 0x2000004
    	mov rdi, 1
    	mov rdx, 8
    	syscall

	pop rsi
    	ret
