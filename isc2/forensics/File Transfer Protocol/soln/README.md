## File Transfer Protocol 1

> I caught them transferring a file, but it got cut off halfway...
> 
> Can you get me their credentials?
> 
> Flag format: flag{username:password]

This PCAP is very short and simple. We can follow the TCP stream to obtain the FTP credentials.

```
220 (vsFTPd 3.0.5)

USER hehehaha

331 Please specify the password.

PASS lololala

230 Login successful.

SYST

215 UNIX Type: L8

FEAT

211-Features:
 EPRT
 EPSV
 MDTM
 PASV
 REST STREAM
 SIZE
 TVFS
211 End

EPSV

229 Entering Extended Passive Mode (|||13889|)

LIST

150 Here comes the directory listing.
226 Directory send OK.
```

`flag{hehehaha:lololala}`

## File Transfer Protocol 2

> I caught them transferring a file, but it got cut off halfway...
>
> Seems like the server is still alive, can you get me that flag file in their server?

Interestingly, only half the teams that solved the first part got this challenge. This probably means that they managed to come across the credentials without really understanding what is FTP and how it works.

Anyways, this challenge can be solved by connecting to the server via FTP and using the credentials from the previous part to fetch the flag file.

```
❯ ftp 209.97.171.172
Connected to 209.97.171.172.
220 (vsFTPd 3.0.5)
Name (209.97.171.172:elmo): hehehaha
331 Please specify the password.
Password: 
230 Login successful.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp> ls
229 Entering Extended Passive Mode (|||62931|)
150 Here comes the directory listing.
-rw-r--r--    1 0        0              32 Sep 06 05:57 flag.txt
226 Directory send OK.
ftp> get flag.txt
local: flag.txt remote: flag.txt
229 Entering Extended Passive Mode (|||35770|)
150 Opening BINARY mode data connection for flag.txt (32 bytes).
100% |************************************************************************************************************************|    32      520.83 KiB/s    00:00 ETA
226 Transfer complete.
32 bytes received in 00:00 (6.94 KiB/s)
ftp> exit
221 Goodbye.

❯ cat flag.txt 
flag{hehehahalololalaftpserver}
```

`flag{hehehahalololalaftpserver}`