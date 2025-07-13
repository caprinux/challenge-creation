#!/usr/bin/env python3
import struct
import re
import random
import sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection
from elftools.elf.relocation import RelocationSection
from elftools.elf.dynamic import DynamicSection

# useful_functions = []
# useless_functions = []
# jmprel = [] # (addr of jmprel entry, actual GOT entry, )
# used_functions = ["exit", "puts"]
used_functions = [re.findall(r"\] (.*)@",j)[0] for j in  """[0x55555559b018] printf@GLIBC_2.2.5 -> 0x555555556030 ◂— endbr64
[0x55555559b020] strstr@GLIBC_2.2.5 -> 0x555555556040 ◂— endbr64
[0x55555559b028] rand@GLIBC_2.2.5 -> 0x555555556050 ◂— endbr64
[0x55555559b030] cbreak@NCURSES6_TINFO_5.0.19991023 -> 0x555555556060 ◂— endbr64
[0x55555559b038] echo@NCURSES6_5.0.19991023 -> 0x555555556070 ◂— endbr64
[0x55555559b040] realpath@GLIBC_2.3 -> 0x555555556080 ◂— endbr64
[0x55555559b048] longjmp@GLIBC_2.2.5 -> 0x555555556090 ◂— endbr64
[0x55555559b050] strlen@GLIBC_2.2.5 -> 0x5555555560a0 ◂— endbr64
[0x55555559b058] memset@GLIBC_2.2.5 -> 0x5555555560b0 ◂— endbr64
[0x55555559b060] SHA256_Init@OPENSSL_3.0.0 -> 0x5555555560c0 ◂— endbr64
[0x55555559b068] sleep@GLIBC_2.2.5 -> 0x5555555560d0 ◂— endbr64
[0x55555559b070] curs_set@NCURSES6_TINFO_5.0.19991023 -> 0x5555555560e0 ◂— endbr64
[0x55555559b078] memcpy@GLIBC_2.14 -> 0x5555555560f0 ◂— endbr64
[0x55555559b080] ptrace@GLIBC_2.2.5 -> 0x555555556100 ◂— endbr64
[0x55555559b088] srand@GLIBC_2.2.5 -> 0x555555556110 ◂— endbr64
[0x55555559b090] __isoc99_sscanf@GLIBC_2.7 -> 0x555555556120 ◂— endbr64
[0x55555559b098] fclose@GLIBC_2.2.5 -> 0x555555556130 ◂— endbr64
[0x55555559b0a0] wmove@NCURSES6_5.0.19991023 -> 0x555555556140 ◂— endbr64
[0x55555559b0a8] __stack_chk_fail@GLIBC_2.4 -> 0x555555556150 ◂— endbr64
[0x55555559b0b0] wrefresh@NCURSES6_5.0.19991023 -> 0x555555556160 ◂— endbr64
[0x55555559b0b8] noecho@NCURSES6_5.0.19991023 -> 0x555555556170 ◂— endbr64
[0x55555559b0c0] endwin@NCURSES6_5.0.19991023 -> 0x555555556180 ◂— endbr64
[0x55555559b0c8] fopen@GLIBC_2.2.5 -> 0x555555556190 ◂— endbr64
[0x55555559b0d0] free@GLIBC_2.2.5 -> 0x5555555561a0 ◂— endbr64
[0x55555559b0d8] exit@GLIBC_2.2.5 -> 0x5555555561b0 ◂— endbr64
[0x55555559b0e0] mvprintw@NCURSES6_5.0.19991023 -> 0x5555555561c0 ◂— endbr64
[0x55555559b0e8] malloc@GLIBC_2.2.5 -> 0x5555555561d0 ◂— endbr64
[0x55555559b0f0] strcmp@GLIBC_2.2.5 -> 0x5555555561e0 ◂— endbr64
[0x55555559b0f8] getppid@GLIBC_2.2.5 -> 0x5555555561f0 ◂— endbr64
[0x55555559b100] usleep@GLIBC_2.2.5 -> 0x555555556200 ◂— endbr64
[0x55555559b108] _setjmp@GLIBC_2.2.5 -> 0x555555556210 ◂— endbr64
[0x55555559b110] initscr@NCURSES6_5.0.19991023 -> 0x555555556220 ◂— endbr64
[0x55555559b118] wgetnstr@NCURSES6_5.0.19991023 -> 0x555555556230 ◂— endbr64
[0x55555559b120] fread@GLIBC_2.2.5 -> 0x555555556240 ◂— endbr64
[0x55555559b128] puts@GLIBC_2.2.5 -> 0x555555556250 ◂— endbr64
[0x55555559b130] fgets@GLIBC_2.2.5 -> 0x555555556260 ◂— endbr64
[0x55555559b138] snprintf@GLIBC_2.2.5 -> 0x555555556270 ◂— endbr64
[0x55555559b140] SHA256_Update@OPENSSL_3.0.0 -> 0x555555556280 ◂— endbr64
[0x55555559b148] strdup@GLIBC_2.2.5 -> 0x555555556290 ◂— endbr64
[0x55555559b150] qsort@GLIBC_2.2.5 -> 0x5555555562a0 ◂— endbr64
[0x55555559b158] SHA256_Final@OPENSSL_3.0.0 -> 0x5555555562b0 ◂— endbr64""".split("\n")]
print(used_functions)
gots = []

def get_symbol_name(elffile, symbol_index):
    """Get symbol name from symbol index."""
    # Look for symbol tables
    for section in elffile.iter_sections():
        if isinstance(section, SymbolTableSection):
            if symbol_index < section.num_symbols():
                symbol = section.get_symbol(symbol_index)
                return symbol.name if symbol.name else f"<unnamed_{symbol_index}>"
    return f"<unknown_{symbol_index}>"

def mess_jmprel(filepath):
    with open(filepath, 'rb') as f:
        elffile = ELFFile(f)

        print(f"ELF File: {filepath}")
        print(f"Architecture: {'64-bit' if elffile.elfclass == 64 else '32-bit'}")
        print(f"Endianness: {'Little' if elffile.little_endian else 'Big'}")
        print("-" * 60)

        # Find dynamic section first
        dynamic_section = None
        for section in elffile.iter_sections():
            if isinstance(section, DynamicSection):
                dynamic_section = section
                break

        if not dynamic_section:
            print("No dynamic section found - this might be a statically linked binary")
            return

        # Parse dynamic entries to find JMPREL info
        jmprel_addr = None
        jmprel_size = None
        pltrel_type = None

        for tag in dynamic_section.iter_tags():
            if tag.entry.d_tag == 'DT_JMPREL':
                jmprel_addr = tag.entry.d_val
            elif tag.entry.d_tag == 'DT_PLTRELSZ':
                jmprel_size = tag.entry.d_val
            elif tag.entry.d_tag == 'DT_PLTREL':
                pltrel_type = tag.entry.d_val

        for section in elffile.iter_sections():
            if isinstance(section, RelocationSection):
                # Check if this section's address matches JMPREL
                if hasattr(section, 'header') and section.header.sh_addr == jmprel_addr:
                    jmprel_section = section
                    break
        for i, relocation in enumerate(jmprel_section.iter_relocations()):
            symbol_name = get_symbol_name(elffile, relocation['r_info_sym'])

            # print(f"Entry {i:2d}:")
            # print(f"  Offset:     0x{relocation['r_offset']:016x}")
            # print(f"  Type:       {relocation['r_info_type']:2d}")
            # print(f"  Symbol:     {relocation['r_info_sym']:4d} ({symbol_name})")

            if symbol_name == "exit":
                exit_got = relocation['r_offset']

            gots.append((symbol_name, relocation['r_offset']))
            # Check if this is RELA (has addend) or REL
            if hasattr(relocation, 'r_addend'):
                print(f"  Addend:     {relocation['r_addend']:+d}")

            # print()

    no_relocs = section.num_relocations()
    with open(filepath, "rb") as f:
        contents = bytearray(f.read())

    original_gots = gots[:]
    while True:
        works = True
        random.shuffle(gots)
        for entry in range(len(gots)):
            if original_gots[entry][0] in used_functions and gots[entry][0] in used_functions:
                print(original_gots[entry][0], 'overwrites', gots[entry][0])
                # input(".")
                works = False
                break
            elif original_gots[entry][0] in used_functions:
                print(original_gots[entry][0], '->', gots[entry][0])
        if works:
            break


    for i in range(no_relocs):
        contents[jmprel_addr+0x18*i:jmprel_addr+0x18*i+8] = struct.pack("<Q", gots[i][1])

    with open(filepath, "wb") as f:
        f.write(bytes(contents))

mess_jmprel("./chall")
