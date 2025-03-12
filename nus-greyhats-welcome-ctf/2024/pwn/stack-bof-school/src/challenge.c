#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>

#define BLACK "\e[0;30m"
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define CYAN "\e[0;36m"
#define WHITE "\e[0;37m"
#define RESET "\e[0m"
#define CLRSCR "\e[2J\e[H"

char save[0x1000] = {0};
int sz_input = 0, i = 0;
char chr_input;
char hex[2];
void * return_addr;

unsigned char h2d(char hex)
{
        switch(hex)
        {
                case '0':       return 0;
                case '1':       return 1;
                case '2':       return 2;
                case '3':       return 3;
                case '4':       return 4;
                case '5':       return 5;
                case '6':       return 6;
                case '7':       return 7;
                case '8':       return 8;
                case '9':       return 9;
                case 'a':       return 10;
                case 'A':       return 10;
                case 'b':       return 11;
                case 'B':       return 11;
                case 'c':       return 12;
                case 'C':       return 12;
                case 'd':       return 13;
                case 'D':       return 13;
                case 'e':       return 14;
                case 'E':       return 14;
                case 'f':       return 15;
                case 'F':       return 15;
        }
        return 0;
}


void print_stack(int input, int changed) {
	void* rbp = __builtin_frame_address(1) + 0x10;
	void* rsp = __builtin_frame_address(0) + 0x10;
	printf("\n\t             " YELLOW "stack memory" RESET "             \n");
	printf("\t┌─────────────────────────┬──────────┐\n");
	printf("\t│                         │          │");
	int num_printed = 0;
	for (char * cur = rsp; cur != rbp; cur += 8) {
		printf("\n\t│ "BLUE);
		for (int i = 0; i < 8; i++) {
			if ((num_printed >= input) && (num_printed+i < changed+input)) {
				printf(RED"%02hhx "BLUE, cur[i]);
			} else {
				printf("%02hhx ", cur[i]);
			}
		}

		printf(RESET"│ "GREEN);
		for (int i = 0; i < 8; i++) {
			if (cur[i] >= 0x20 && cur[i] < 0x7f) {
				if (num_printed >= input && num_printed+i < changed+input) {
					printf(RED"%c"GREEN, cur[i]);
				} else {
					printf("%c", cur[i]);
				}
			} else {
				if (num_printed >= input && num_printed+i < changed+input) {
					printf(RED"."GREEN);
				} else {
					printf(".");
				}
			}
		}
		printf(RESET" │");
		if (cur == rsp) {
			printf(" <---- RSP (stack pointer)");
		} else if (cur == rbp-16) {
			printf(" <---- RBP (base pointer)");
		} else if (cur == rbp-8) {
			printf(" <---- return address [%p]", *(char**)cur);
		} else if (cur == rsp+input) {
			printf(" <---- input");
		}
		printf("\n\t│                         │          │");
		num_printed += 8;
	}
	printf("\n\t└─────────────────────────┴──────────┘\n");
}

void win() {
	char flag[200] = {0};
	puts("\tyou got your first ret2win!");
	int fd = open("flag.txt", 0);
	read(fd, flag, 200);
	puts(flag);
	exit(0);
}

void vulnerable_function() {
	char buffer[0x20];

	do {
		printf(CLRSCR);
		i = 0;
		print_stack(0x0, sz_input);
		printf("\twin function @ "PURPLE"%p\n"RESET, win);
		printf("\n\tpress enter to terminate your input\n");
		printf(GREEN"\n\tto insert a byte based on the hex value, prefix it with a '\\' character\n");
		printf("\t(i.e.) typing \\41 will produce A\n\n"RESET);
		printf(PURPLE"\tinput: "RESET);
		for (i = 0; i < sz_input; i++) {
			printf("%c", buffer[i]);
		}
		chr_input = getchar();
		if (sz_input < 0x40 || chr_input == 0x7f || chr_input == 0xa) {
			if (chr_input == 0xa) {
				break;
			} else if (chr_input == 0x7f) {
				if (sz_input >= 0x1) {
					sz_input -= 1;
					buffer[sz_input] = save[sz_input];
				}
				sz_input -= 1;
			} else if (chr_input == '\\') {
				hex[0] = getchar();
				hex[1] = getchar();
				save[sz_input] = buffer[sz_input];
				buffer[sz_input] = h2d(hex[0])*0x10 + h2d(hex[1]);
			} else {
				save[sz_input] = buffer[sz_input];
				buffer[sz_input] = chr_input;
			}
			sz_input += 1;
		}
	} while (1);

	printf("\n\treturning to --> "RED"%p\n\n"RESET, *((long*)&buffer[0x38]));
	return_addr = (void*)*((long*)&buffer[0x38]);
}

void crashed() {
	printf("\tYou tried returning to "RED"%p"RESET" but the win function is at "GREEN "%p"RESET"! Try again :)\n\n", return_addr, win);
	exit(0);
}

int main() {
	// disable input buffering, not important!
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	signal(SIGSEGV, crashed);
	vulnerable_function();
	puts("\tDid you even try overwriting the return address?\n");
}
