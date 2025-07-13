# coding: utf-8
from unicorn import *
import base64
from unicorn.x86_const import *
from capstone import *
import z3

cs = Cs(CS_ARCH_X86, CS_MODE_64)
cs.detail = True # Enable detailed instruction information

with open("./chall", "rb") as f:
    chal = f.read()

mu = Uc(UC_ARCH_X86, UC_MODE_64)

mu.mem_map(0x1337000, 0x100000)
mu.mem_map(0x7ffff000000, 0x100000)
mu.mem_write(0x1337000, chal)

mu.reg_write(UC_X86_REG_RBP, 0x7ffff050000)
mu.reg_write(UC_X86_REG_RSP, 0x7ffff050000)

eqn = []
eqns = []

# Define a hook_code function to print disassembled instructions
def hook_code(uc, address, size, user_data):
    global eqn
    global eqns
    # Read the instruction bytes from emulated memory
    code_bytes = uc.mem_read(address, size)

    for i in cs.disasm(code_bytes, address):
        if i.mnemonic == 'movzx' and "eax, byte ptr [" in i.op_str:
            val = int(i.op_str.split(' + ')[1][:-1], 16) + i.address + 7
            offs = val - 0x80040 - 0x1337000
            eqn.append(f'flag[{offs}]')
        elif "cmp" == i.mnemonic:
            val = i.op_str.split(', ')[1]
            eqn.append(val)
            body = eqn[1:-1]
            body = str(eqn[0])+''.join([''.join(str(j) for j in body[i:i+2][::-1]) for i in range(0, len(body), 2)])+'=='+eqn[-1]
            eqns.append(body)
            eqn = []
        elif "sub" == i.mnemonic and 'eax, edx' == i.op_str:
            eqn.append("-")
        elif "add" == i.mnemonic and 'eax, edx' == i.op_str:
            eqn.append("+")

mu.hook_add(UC_HOOK_CODE, hook_code)
mu.emu_start(0x1337000+0x36df1 , 0x1337000+0x3d60a)

s = z3.Solver()
flag = [z3.BitVec(f"flag_{i}", 8) for i in range(80)]
for eqn in eqns:
    s.add(eval(eqn))
print(s.check())
m = s.model()
f = ''
for i in flag:
    f += chr(int(m[i].as_long()))
print(base64.b64decode(f))
