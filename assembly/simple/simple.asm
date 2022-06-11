; Mostly from the Assembly Language Programming Programming book by Ed Jorgensen

section .data

EXIT_SUCCESS equ 0
SYS_EXIT equ 60


bVar1 db 17

wVar1 dw 17000
wVar2 dw 9000
wResult dw 0


section .text
global _start
_start:

	mov al, byte[bVar1]

	mov ax, word[wVar1]
	add ax, word[wVar2]
	mov word[wResult], ax

last:
	mov rax, SYS_EXIT
	mov rdi, EXIT_SUCCESS
	syscall

