global start

section .text
start:
	push rax
	push rbx

    	call print

	mov qword [rbp - 18], rax
	mov rax, qword [rbp - 18]
	
print:
	pop rax
	push rsi
	add rsi, 9
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
