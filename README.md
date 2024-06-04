| Event | Difficulty (1-10) | Category | Challenge Name | Description |
| - | - | - | - | - |
| Welcome CTF 2023 |1| Pwn | complete me | simple shellcode runner |
| Welcome CTF 2023 |2| Pwn | late for school | simple rop challenge |
| Welcome CTF 2023 |3| Pwn | secure blob runner | shellcode runner with orw seccomp, and syscall bytes are blocked |
| Welcome CTF 2023 |3| Pwn | filelen | really cool challenge. flag file is `fopen` and `fseek`-ed to tell the file size. this results in the flag content being implicitly allocated into the heap and free-ed when `fclose(f)` is called. user is then allowed to pass in an input into the heap which allows them to leak the flag by passing in an input without the null terminator |
| Welcome CTF 2023 |1| Misc | pi-master | simple pwntools automation challenge |
| Welcome CTF 2023 |1| Misc | Baby Cake | three layers of steganography (exiftool,binwalk,lsb) |
| Welcome CTF 2023 |3| Misc | Pee Dee Eff | fix pdf stream |
| Welcome CTF 2023 |3| Misc | Fix Me Lahh | fix png magic bytes and extend image by modifying height |
| Welcome CTF 2023 |3| Misc | Backdoored | analyze pcap and replay the packets to get reverse shell in challenge server |
| Welcome CTF 2023 |1| Rev | Puzzles | simple flag xor challenge |
| Cyberthon 2023 |3| Pwn | Wordpocalypse | negative array indexing allows to overwrite GOT to ret2win |
| Cyberthon 2024 |3| Pwn | The Forge | simple ret2libc challenge without pop rdi, but there is a part of code that is JITed allowing you to craft your own `pop rdi` gadget |
| GreyCTF 2024 |1| Pwn | The Motorala 1 | simple buffer overflow to ret2win |
| GreyCTF 2023 |4| Pwn | Read me a book | Uninitialized Stack Variable |
| GreyCTF 2024 |5| Pwn | The Motorala 2 | same program as motorala 1, but compiled in wasm. buffer overflow from stack to heap to overwrite a heap variable whilst fixing dlmalloc heap metadata |
| GreyCTF 2023 |5| Pwn | Write me a book | linux userland heap with orw seccomp |
| GreyCTF 2023 |6| Pwn | Poopoo Pants | linux userland heap, FSOP to bypass ASLR followed by ROP to ret2libc |
| GreyCTF 2023 |7| Pwn | Sanity Check | simple kernel ret2usr, flag is stored in memory and deleted. egghunting in kernel to find the flag |
| GreyCTF 2024 |6| Rev | Mazeware | small snippet of assembly that cannot be seen via the decompiler calls into a shellcode which hooks into the GOT to hijack inputs into a hidden maze that will decrypt the flag |
