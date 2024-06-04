import base64
from pwn import xor

enc = base64.b64decode("+hpPlyY9EZf7WVLdKgMI198cSt0gAyLmiE4Bgj19bqiwSRSkJixFtQ==")

e = [enc[i:i+8] for i in range(0, len(enc), 8)]
f = [None for _ in range(5)]

f[0] = b"greyhats"
f[2] = xor(xor(e[0], e[1]), f[0])
f[3] = xor(xor(e[3], e[4]), f[0])
f[4] = xor(xor(e[2], e[3]), f[2])
f[1] = xor(xor(e[1], e[2]), f[3])

print(b"".join(f).decode())
