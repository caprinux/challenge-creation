# Challenge Details

I'm new to C so I wrote a very simple program that finds the length of a file in centimeters.
The program is so simple that nothing could go wrong :D

# Setup Instructions

Dockerfile

# Key Concepts

non null-terminating string via read function

heap data reuse if not cleared

# Author

Jin Kai

# Solution

The program checks the length of the file via 

```c
void measure(const char* name) {
        FILE *f = fopen(name, "r");
        if (f) {
                fseek(f, 0, SEEK_END);
                flag_len = ftell(f);
                fclose(f);
        }
}
```

The flag is immediately opened and closed, but it is not read into memory.

The trick is that when operations are done on our filp that deals with the contents of the file, the contents will be copied into a heap chunk and freed upon closing via `fclose`.

This means that the flag is in the heap within the wilderness.

Thus we can leak the flag when printing our name by not null terminating our input.

# Flag
 
`greyhats{th3_fl4g_w4s_fr33_bu7_y0u_br0ught_1t_b4ck_bY_h34p_r3us3!}`
