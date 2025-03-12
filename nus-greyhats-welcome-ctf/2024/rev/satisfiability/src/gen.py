import random
import z3

operands = ["+", "-", "*"]
flag = list(b"grey{i_learnt_all_about_SMT_solvers_today_z3_or_cvc5_is_god}")
eqns = []

for j in range(6):
    shuf_flag = [i for i in range(len(flag))]
    random.shuffle(shuf_flag)
    n = 5
    while shuf_flag:
        ids = [shuf_flag.pop() for _ in range(n) if shuf_flag]
        ops = random.choices(operands, k=len(ids))
        eqn = "("
        for i in range(len(ids)):
            eqn += f"flag[{ids[i]}] {ops[i]} "
        eqn = eqn[:-2] + ")== " + str(eval(eqn[:-2]+")"))
        eqns.append(eqn)

s = z3.Solver()
flag = [z3.BitVec(f"f{i}", 8) for i in range(len(flag))]
for eqn in eqns:
    print(f"(long)({eqn}")
    s.add(eval(eqn))

print(s.check())
m = s.model()
for i in flag:
    print(chr(int(m[i].as_long())), end='')
