from pwn import *

p = remote("localhost", 32111)

p.sendline()
p.recvuntil(b"GO!\n\n")
keys = list(b"hjkl")

for i in range(1000):
    p.send(chr(keys[[x.strip() for x in p.recvline().split(b"|")[1:-1]].index(b"X")]).encode())

p.interactive()
