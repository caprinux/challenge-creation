# Challenge Archive

This is a collection of most of the challenges I've ever written for CTFs.

The difficulty tagged to the challenges are of my own opinion.

## NUS Greyhats Welcome CTF

The target audience for this CTF are students from National University of Singapore who are new to CTFs.

| Difficulty (1-10) | Category | Challenge Name | Description |
| - | - | - | - |
|1| Pwn | complete me | simple shellcode runner |
|2| Pwn | late for school | simple rop challenge |
|3| Pwn | secure blob runner | shellcode runner with orw seccomp, and syscall bytes are blocked |
|3| Pwn | filelen | leak data that was previously allocated to the same memory in the heap then free-ed |
|1| Misc | pi-master | simple pwntools automation challenge |
|1| Misc | Baby Cake | three layers of steganography (exiftool,binwalk,lsb) |
|3| Misc | Pee Dee Eff | fix pdf stream |
|3| Misc | Fix Me Lahh | fix png magic bytes and extend image by modifying height |
|3| Misc | Backdoored | analyze pcap and replay the packets to get reverse shell in challenge server |
|1| Rev | Puzzles | simple flag xor challenge |

## [Cyberthon](https://www.cyberthon.hci.edu.sg/)

The target audience for this CTF are high school students, after going through 3 weeks of CTF training.

| Difficulty (1-10) | Category | Challenge Name | Description |
| - | - | - | - |
|3| Pwn | Wordpocalypse | negative array indexing allows to overwrite GOT to ret2win |
|3| Pwn | The Forge | simple ret2libc challenge without pop rdi, but there is a part of code that is JITed allowing you to craft your own `pop rdi` gadget |

## [NUS Greyhats Grey Cat The Flag](https://ctf.nusgreyhats.org)

The target audience for this CTF is local and international university students.

| Difficulty (1-10) | Category | Challenge Name | Description |
| - | - | - | - |
|1| Pwn | The Motorala 1 | simple buffer overflow to ret2win |
|4| Pwn | Read me a book | Uninitialized Stack Variable |
|5| Pwn | The Motorala 2 | same program as motorala 1, but compiled in wasm. buffer overflow from stack to heap to overwrite a heap variable whilst fixing dlmalloc heap metadata |
|5| Pwn | Write me a book | linux userland heap with orw seccomp |
|6| Pwn | Poopoo Pants | linux userland heap, FSOP to bypass ASLR followed by ROP to ret2libc |
|7| Pwn | Sanity Check | simple kernel ret2usr, flag is stored in memory and deleted. egghunting in kernel to find the flag |
|6| Rev | Mazeware | small snippet of assembly that cannot be seen via the decompiler calls into a shellcode which hooks into the GOT to hijack inputs into a hidden maze that will decrypt the flag |

## [R3KAPIG CTF](https://ctftime.org/event/2273)

As part of r3kapig, I contributed a single pwn challenge to the CTF.

| Difficulty (1-10) | Category | Challenge Name | Description |
| - | - | - | - |
|6| Pwn | Feedback Portal | wasm heap UAF/overflow as a result of `realloc`. unsafe unlink attack to overwrite stdout file structure to call another function in `_stdio_exit` |
