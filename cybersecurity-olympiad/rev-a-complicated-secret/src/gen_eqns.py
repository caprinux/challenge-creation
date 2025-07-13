import random
import z3
import base64

operands = ["+", "-", "*"]
operands = ["+", "-"]
flag = list(base64.b64encode(b"ICO{callcallcallcallcallcallcallcallcallcallcallangrcallme}"))
eqns = []

for j in range(80): # (num of set of eqns)
    shuf_flag = [i for i in range(len(flag))]
    random.shuffle(shuf_flag)
    n = 6
    while len(shuf_flag) >= n:
        ids = [shuf_flag.pop() for _ in range(n) if shuf_flag]
        ops = random.choices(operands, k=len(ids))
        eqn = ""
        for i in range(len(ids)):
            eqn += f"flag[{ids[i]}] {ops[i]} "
        eqn = eqn[:-2] + " == " + str(eval(eqn[:-2]))
        eqns.append(eqn)

s = z3.Solver()
flag = [z3.BitVec(f"f{i}", 8) for i in range(len(flag))]
for eqn in eqns:
    print(f"{eqn}")
    s.add(eval(eqn))

print(s.check())
m = s.model()
for i in flag:
    print(chr(int(m[i].as_long())), end='')
