from pwn import *
from re import findall

with open("./Pi125MDP.txt", "r") as f:
    PI = '3' + f.read()

# p = process(["python3", "chall.py"])
p = remote("localhost", 25555)
p.sendlineafter(b"> ", b"y")

for i in range(314):
    qn = findall(r"(\d+)", p.recvuntilS(b"> "))
    len = int(qn[0])
    offset = int(qn[1])
    p.sendline(PI[offset-1:offset-1+len].encode())

p.interactive()
