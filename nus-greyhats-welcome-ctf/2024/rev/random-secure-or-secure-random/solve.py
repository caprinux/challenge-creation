from math import ceil
from pwn import *
import ctypes

lib = ctypes.CDLL("libc.so.6")
lib.srand(lib.time(0))
p = process("./challenge")

p.recvuntil(b"flag: ")
enc = bytes.fromhex(p.recvline().strip().decode())
p.close()
enc = [enc[i:i+4] for i in range(0, len(enc), 4)]
print(enc)

for i in range(len(enc)):
    enc[i] = xor(enc[i], lib.rand().to_bytes(4, 'little'))

print(b"".join(enc).decode())
