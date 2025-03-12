#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	setbuf(stdout, 0);
	srand(time(NULL));

	FILE *f = fopen("flag.txt", "r");
	fseek(f, 0, SEEK_END);
	size_t sz = ftell(f);
	char* flag = calloc(sz+4, sizeof(char));
	rewind(f);
	fread(flag, sz, sizeof(char), f);
	fclose(f);

	printf("generating super secure random sequence");
	sleep(1);
	printf(".");
	sleep(1);
	printf(".");
	sleep(1);
	printf(".\n");

	size_t blocks = sz / 4;
	if (sz % 4) {
		blocks += 1;
	}
	printf("here's your encrypted flag: ");
	for (int i = 0; i < blocks; i++) {
		((int*)flag)[i] ^= rand();
		for (int j = 0; j < 4; j++) {
			printf("%02hhx", ((char*)&(((int*)flag)[i]))[j]);
		}
	}
	putchar(0xa);
	free(flag);
}
