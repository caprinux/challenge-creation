#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdint.h>

int main() {

	setbuf(stdin, 0);
	setbuf(stdout, 0);

	void (*print_flag)(void);
	char* code = mmap(0, 0x1000, 7, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	print_flag = (void(*)(void))code;

	printf("The flag is: ");
	fgets(code, 0x1000, stdin);
	print_flag();

}
