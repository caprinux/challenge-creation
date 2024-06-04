#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

long flag_len = 0;

// buffer stdin stdout 
void init() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
}

// measure length of a file
void measure(const char* name) {
	FILE *f = fopen(name, "r");
	if (f) {
		fseek(f, 0, SEEK_END);
		flag_len = ftell(f);
		fclose(f);
	}
}

// prompt for name
char* get_name() {
	unsigned int size = 0;
	printf("Btw what is your name?\n");
	printf("Length: ");
	scanf("%u", &size);
	if (size <= 1 || size > 0x100) {
		printf("Invalid name length!");
		exit(0);
	}
	getchar();
	char* name = malloc(size);
	printf("Name: ");
	read(0, name, size);
	char* nl = strchr(name, 0xa);
	if (nl)
		*nl = 0x0;
	return name;

}

int main() {
	char file_name[0x50];
	init();

	// measure file
	printf("Which file do you want to measure?\n> ");
	read(0, file_name, 0x50);
	char* nl = strchr(file_name, 0xa);
	if (nl)
		*nl = 0x0;
	measure(file_name);

	// get name
	printf("The file is %ldcm long!\n\n", flag_len);
	char* name = get_name();
	printf("Goodbye %s!\n", name);
}
