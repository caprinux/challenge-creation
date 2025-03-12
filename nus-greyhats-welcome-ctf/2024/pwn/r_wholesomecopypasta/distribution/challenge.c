#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

char* copypastas[] = {"mc_chicken.txt", "chameleon.txt", "bob.txt", "flag.txt"};

void print_file_contents(char* file_name) {
	FILE *f = fopen(file_name, "r");
	if (f == NULL) {
		perror("file does not exist");
		exit(0);
	}
	fseek(f, 0, SEEK_END);
	size_t sz = ftell(f);
	rewind(f);
	char* buf = calloc(sz, sizeof(char));
	fread(buf, sz, sizeof(char), f);
	fclose(f);
	printf("copypasta contents:\n%s", buf);
}

int main() {
	
	char filename[100];

	setbuf(stdin, 0);
	setbuf(stdout, 0);

	printf("list of available copypastas\n");
	for (int i = 0; i < 3; i++) {
		printf("%d. %s\n", i, copypastas[i]);
	}
	putchar(0xa);
	printf("input copypasta to read: ");
	size_t end = read(0, filename, 0x100);
	if (filename[end-1] = '\n')
		filename[end-1] = 0;


	if (strstr(filename, "flag") || strchr(filename, '/') || strchr(filename, '\\')) {
		puts("this copypasta is premium!");
		exit(0);
	}

	print_file_contents(filename);
}
