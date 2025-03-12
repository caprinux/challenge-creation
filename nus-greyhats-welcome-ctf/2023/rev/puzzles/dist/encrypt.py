import secrets
import base64

def xor(a:bytes, b:bytes) -> bytes:
    return bytes(i^j for i, j in zip(a, b))

with open("./flag.txt", "rb") as f:
    flag = f.read().strip()

assert len(flag) == 40
pt = [flag[i:i+8] for i in range(0, len(flag), 8)]
ct = [None for _ in range(5)]

# if our key is securely random, our encryption is naturally secure!
key = secrets.token_bytes(8)

# encryption!
ct[0] = xor(xor(pt[0], key), pt[1])
ct[1] = xor(xor(pt[1], key), pt[2])
ct[2] = xor(xor(pt[2], key), pt[3])
ct[3] = xor(xor(pt[3], key), pt[4])
ct[4] = xor(xor(pt[4], key), pt[0])

print(base64.b64encode(b"".join(ct)))

# output: +hpPlyY9EZf7WVLdKgMI198cSt0gAyLmiE4Bgj19bqiwSRSkJixFtQ==

