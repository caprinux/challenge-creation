#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

char flag[] = {0x26, 0x33, 0x24, 0x38, 0x3a, 0x70, 0x1e, 0x26, 0x71, 0x35, 0x1e, 0x32, 0x34, 0x31, 0x72, 0x33, 0x1e, 0x25, 0x34, 0x31, 0x72, 0x33, 0x1e, 0x2d, 0x34, 0x22, 0x2a, 0x38, 0x1e, 0x2e, 0x33, 0x1e, 0x70, 0x2c, 0x1e, 0x2b, 0x34, 0x32, 0x35, 0x1e, 0x31, 0x33, 0x71, 0x1e, 0x33, 0x72, 0x37, 0x24, 0x33, 0x32, 0x24, 0x33, 0x3c, 0x41};

void deobfuscate_flag() {
	for (int i = 0; i < sizeof(flag); i++) {
		flag[i] ^= 0x41;
	}
}

int main() {
	deobfuscate_flag();

	srand(time(NULL));
	uint64_t res = rand();
	res <<= 32;
	res += rand();

	printf("Let's play a game. If you hit the lucky number 7777777777777777, you get the flag!\n");
	printf("If not, I will still reward you with part of the flag :)\n\n");

	if (res == 0x7777777777777777) {
		printf("JACKPOT! Here's the flag: %s!\n", flag);
	} else {
		printf("You hit the number %lx, heres %d characters of the flag: %.*s!\n", res, (int)(res % 6), (int)(res % 6), flag);
	}

}
