BITS 64

section .data
	prompt db 'piece together my rop chain!',0xa,'> '
	promptlen: equ $ - prompt ;; is this how you find the message length ??
	flagtxt db 'flag.txt',0x0
	lol db 0x0,0x0


section .text
	global _start

_start:
	mov rbp, rsp
	call vuln

	;; exit
	mov rax, 60
	xor rdi, rdi
	syscall
	ud2

vuln:
	push rbp
	mov rbp, rsp
	sub rsp, 0x10

	;; prompt
	mov rax, 1
	mov rdi, 1
	mov rsi, prompt
	mov edx, promptlen
	syscall

	;; input
	mov rax, 0
	mov rdi, 0
	mov rsi, rsp
	mov rdx, 0x1000
	syscall

	;; ret
	add rsp, 0x10
	leave
	ret


gadgets:
	mov rax, 1
	ret
	mov rax, 0
	ret
	mov rax, 2
	ret
	mov rdi, 0
	ret
	mov rdi, 1
	ret
	mov rsi, rsp
	ret
	mov rdx, 100
	ret
	mov rdi, flagtxt
	ret
	mov rsi, 0
	ret
	mov rdx, 0
	ret
	syscall
	ret
	mov rdi, rax
	ret
	mov rsi, lol
	ret
