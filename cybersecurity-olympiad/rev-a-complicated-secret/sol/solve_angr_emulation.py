import angr
import logging
import z3
import tqdm

# logging.getLogger("angr").setLevel("INFO")

p = angr.Project("./a.out")

def get_disasm(proj, addr):
    block = proj.factory.block(addr)
    first_insn = block.capstone.insns[0]
    return first_insn.mnemonic + " " + first_insn.op_str

base_address = p.loader.main_object.min_addr
state = p.factory.blank_state(addr=base_address+0x36df1, add_options={angr.options.UNICORN, angr.options.ZERO_FILL_UNCONSTRAINED_MEMORY, angr.options.ZERO_FILL_UNCONSTRAINED_REGISTERS})

mgr = p.factory.simgr(state)

s = z3.Solver()
flag = [z3.BitVec(f'flag_{i}', 8) for i in range(80)]

for _ in tqdm.tqdm(range(1040)):
    eqn = []

    while True:
        state = state.step(num_inst=1).successors[0]
        x = get_disasm(p, state.addr)

        if "movzx eax, byte ptr [" in x:
            val = int(x.split(' + ')[1][:-1], 16) + state.addr + 7
            offs = val - 0x80040 - base_address
            eqn.append(f'flag[{offs}]')
        elif "cmp" in x:
            val = x.split(', ')[1]
            eqn.append(val)
            break
        elif 'sub eax, edx' == x:
            eqn.append("-")
        elif 'add eax, edx' == x:
            eqn.append("+")

    body = eqn[1:-1]
    body = str(eqn[0])+''.join([''.join(str(j) for j in body[i:i+2][::-1]) for i in range(0, len(body), 2)])+'=='+eqn[-1]
    s.add(eval(body))

        # movzx eax, byte ptr [rip + 0x74a67]
        # val = x.split(' + ')[1][:-1]
        # if not eqn:
        #     eqn += f"flag[{val}]"
        # values.append(val)
        # print(values)
        # load thing

print(s.check())
m = s.model()

