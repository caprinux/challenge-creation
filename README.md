# Challenge Archive

This is a collection of most of the challenges I've ever written for CTFs.

The difficulty tagged to the challenges are of my own opinion.

## NUS Greyhats Welcome CTF

The target audience for this CTF are students from National University of Singapore who are new to CTFs.

Challenges here are generally more beginner friendly.

### 2023

Writeups available [here](https://hackmd.io/@capri/SkivsJtah).

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

### 2024

Writeups available [here](https://blog.elmo.sg/posts/welcome-ctf-2024/).

| Difficulty (1-10) | Category | Challenge Name | Description |
| - | - | - | - |
|1| Misc| Notefactory | pwntools automation |
|1| Misc| Filefactory | fixing PNG magic bytes |
|1| Web | Aimfactory | client-side score tracking |
|2| Web | Submit your Homework | reflected cookie-stealing XSS |
|1| Rev | Simple Linux Flag Checker | strings |
|1| Rev | Simple Windows Flag Checker | stack strings / decompilation |
|1| Rev | Flag Roulette | intro to debugging |
|2| Rev | Random Secure or Secure Random | prng |
|2| Rev | Satisfiability | z3 |
|3| Rev | Is this really python? | pycompiled unflattening |
|1| Pwn | Stack BOF School | simple ret2win with illustrated stack|
|1| Pwn | cowsaymoo | buffer overflow variable overwrite |
|1| Pwn | Epic Boss Fight | integer overflow |
|2| Pwn | r/WholesomeCopypasta | rop |
|3| Pwn | The Trial Author | strcpy buffer overflow & one\_gadget |
|3| Pwn | Dreamfactory | UAF, function pointer overwrite |
|4| Pwn | re:life | bss to heap overflow (beap overflow) |


## [Cyberthon](https://www.cyberthon.hci.edu.sg/)

The target audience for this CTF are high school students, after going through 3 weeks of CTF training.

| Difficulty (1-10) | Category | Challenge Name | Description | Writeup |
| - | - | - | - | - |
|3| Pwn | Wordpocalypse | negative array indexing allows to overwrite GOT to ret2win | [here](https://medium.com/csit-tech-blog/cyberthon-2023-pwn-challenge-wordpocalypse-974b6708b023) |
|3| Pwn | The Forge | simple ret2libc challenge without pop rdi, but there is a part of code that is JITed allowing you to craft your own `pop rdi` gadget | [here](https://blog.elmo.sg/posts/cyberthon-2024-the-forge-pwn/) |

## [NUS Greyhats Grey Cat The Flag](https://ctf.nusgreyhats.org)

The target audience for this CTF is local and international university students.

This CTF contains some of the more interesting challenges that I've made.

### 2023

| Difficulty (1-10) | Category | Challenge Name | Description |
| - | - | - | - |
|4| Pwn | Read me a book | Uninitialized Stack Variable |
|5| Pwn | Write me a book | linux userland heap with orw seccomp |
|6| Pwn | Poopoo Pants | linux userland heap, FSOP to bypass ASLR followed by ROP to ret2libc |
|7| Pwn | Sanity Check | simple kernel ret2usr, flag is stored in memory and deleted. egghunting in kernel to find the flag |

### 2024

| Difficulty (1-10) | Category | Challenge Name | Description |
| - | - | - | - |
|1| Pwn | The Motorala 1 | simple buffer overflow to ret2win |
|4| Pwn | Super Secure Blob Runner | continuation from welcome ctf 2023. shellcode runner in non-writable memory, banned syscall bytes and orw seccomp |
|5| Pwn | The Motorala 2 | same program as motorala 1, but compiled in wasm. buffer overflow from stack to heap to overwrite a heap variable whilst fixing dlmalloc heap metadata |
|5| Pwn | overly simplified pwn challenge | ret2dlresolve without pop rdi |
|3| Rev | Hungry Ghost Festival | retf to transit from 64 bit to execute 32 bit shellcode (hell gate)|
|5| Rev | mazeware | return address hijacking with multi stage shellcode and api hooking |
|6| Rev | overly simplified rev challenge | obfuscated flag checker program with inlined functions, no stack memory and no `mov` instructions! |

## [TISC](https://www.csit.gov.sg/events/tisc)

This CTF is conducted in the same format as flare-on. This CTF lasts over 2 weeks, and each challenge unlocks the next in increasing difficulty.

The target audience for this CTF is experienced cybersecurity enthusiasts/researchers in Singapore (no age limit).

## Miscellaneous CTFs

| CTF | Difficulty (1-10) | Category | Challenge Name | Description |
|- | - | - | - | - |
| [r3kapig CTF](https://ctftime.org/event/2273) |6| Pwn | Feedback Portal | wasm heap UAF/overflow as a result of `realloc`. unsafe unlink attack to overwrite stdout file structure to call another function in `_stdio_exit` |
| ISC2CTF | 2 | Pwn | JIGROP Puzzle | simple orw rop using a series of provided rop gadgets |

