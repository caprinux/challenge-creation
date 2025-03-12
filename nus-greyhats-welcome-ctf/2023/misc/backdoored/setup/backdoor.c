#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

uint64_t hands[10] = {
	0x124eac80fc0062b2,
	0xc5be0fd1d3acaa67,
	0x34dd2228a6a43282,
	0x1159339b541e56f1,
	0x70aca735b2bf7e6c,
	0x526c90e12ddf390b,
	0x98d85286c110b659,
	0x7e1431f33ce72aaa,
	0x5ff07e1e75f088e3,
	0x760fe253bef08ed1
};

int three_way_high_security_backdoor_handshake() {

	char buf[0x10];
	int x;

	// SMH (shake my hand) 10 times
	for (int i = 0; i < 10; i ++) {
		x = read(0, buf, 9);
		if (x != 8) {
			putchar(0xaa);
			return 1;
		}
		if (memcmp(buf, &hands[i], 8)) {
			putchar(0xaa);
			return 1;
		}
		putchar(0xef);
	}

	// ping pong with me for sanity check :D
	srand(time(0));
	uint64_t ball = rand();
	ball = ball << 32;
	ball |= rand();
	for (int i = 0; i < 8; i++) {
		putchar(*(((char*)&ball) + i));
	}

	x = read(0, buf, 9);
	if (x != 8) {
		putchar(0xaa);
		return 1;
	}

	if (memcmp(buf, &ball, 8)) {
		putchar(0xaa);
		return 1;
	}

	putchar(0xef);
	return 0;

}


int main () {

	setbuf(stdin, 0);
	setbuf(stdout, 0);
	if (three_way_high_security_backdoor_handshake()) {
		return 0;
	}

	int opt = 6;
	char fn[0x40];
	while (1) {
		if (!scanf("%d", &opt))
			return 0;

		switch (opt) {
			// backdoor -> readfile
			case 0xabca:
				fgets(fn, 0x40, stdin);
				FILE *f = fopen(fn, "r");
				if (f) {
					fseek(f, 0, SEEK_END);
					size_t len = ftell(f);
					char* buf = calloc(len, sizeof(char));
					fread(buf, len, 1, f);
					fclose(f);
					puts(buf);
				} else {
					return 1;
				}
				break;

			// backdoor -> spawn shell
			case 0xabcb:
				system("/bin/bash");
				break;

			// backdoor -> get system info
			case 0xabcc:
				system("uname -a");
				break;

			// backdoor -> exit
			case 0xabcd:
				putchar(0xaa);
				return 0;

			default:
				putchar(0xaa);
				return 0;
		}
	}

}
