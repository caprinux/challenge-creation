all:
	gcc -fno-builtin -w -fPIE -pie -Wl,-z,relro,-z,lazy chall.c -o no_obfs -lm -ldl -lpthread -lcrypt -lrt -lutil -lresolv -lnsl -lselinux -lcrypto -lncurses
	objcopy --remove-section ".fun" chall
debug:
	gcc -g -O1 -fno-builtin -w -fPIE -pie -Wl,-z,relro,-z,lazy chall.c -o chall -lm -ldl -lpthread -lcrypt -lrt -lutil -lresolv -lnsl -lselinux -lcrypto -lncurses
	objcopy --remove-section ".fun" chall
obf:
	strip chall
	python3 import_obfuscator.py
