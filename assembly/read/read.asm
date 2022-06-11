; Guessing Game, Guess a Number Between 1 and 10

%define MAX_BUF 256

section .data
    EXIT_SUCCESS equ 0
    SYS_EXIT equ 60

    guesstion db 'Pick a Number Between 1 and 10: ', 0
    guesstion_len equ $ - guesstion

    againstion db 'Play Again 0 (no) or 1 (yes): ', 0
    againstion_len equ $ - againstion

    newline db 0xa


    
section .bss
    buf resb MAX_BUF



section .text
    global _start

_start:

read_loop:
    mov rax,1    ; 1 is write syscall
    mov rdi, 1    ; 1 is fd for Std Out
    mov rsi, guesstion    ; Reference to first byte of string
    mov rdx, guesstion_len    ; Length of String
    syscall

    mov rax, 0    ; 0 is read syscall
    mov rdi, 0    ; 0 is fd for Std In
    mov rsi, buf    ; Reference to first byte of buffer
    mov rdx, MAX_BUF    ; Buffer Size
    syscall

    mov al, byte [buf]
    cmp al, 0x30    ; First Byte ASCII is a Digit >= 0
    jl read_loop
    
    cmp ax, 0x39    ; First Byte ASCII is a Digit <= 9
    jg read_loop
    
    mov al, byte [buf + 1] ; See if 2nd byte is new line, if so then move on
    cmp al, 0xa    ; Because when I enter a single digit like 7 it puts \n in 2nd byte
    jz read_quit 

    cmp al, 0x30    ; First Byte ASCII is a Digit >= 0
    jl read_loop
    
    cmp ax, 0x39    ; First Byte ASCII is a Digit <= 9
    jg read_loop


read_quit:
    mov al, [buf + 1]
    cmp al, 0x30    ; See if I have a 2nd Digit or just 1 Digit, to determine end of number
    jl null_terminate_1

    cmp al, 0x39    ; See if I have a 2nd Digit or just 1 Digit, to determine end of number
    jg null_terminate_1 

null_terminate_2:
    mov al, 0
    mov [buf + 2], al    ; NULL Terminate buffer for safety
    jmp after_terminate
    
null_terminate_1:
    mov al, 0
    mov [buf + 1], al    ; NULL Terminate buffer for Safety



after_terminate:
    mov rax,1    ; 1 is write syscall
    mov rdi, 1    ; 1 is fd for Std Out
    mov rsi, buf    ; Reference to first byte of string
    mov rdx, 2    ; Only first 2 bytes are the number for guess, and single digit is null trmntd
    syscall

    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall


last:
    mov rax, SYS_EXIT
    mov rdi, EXIT_SUCCESS
    syscall

