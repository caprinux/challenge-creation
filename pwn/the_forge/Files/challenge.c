#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "color.h"

// CHALLENGE NAME: 			The Forge
// CHALLENGE AUTHOR: 		Jin Kai
// CHALLENGE DESCRIPTION:	Craft your saber. Prove your worth.

#define CLRSCR "\033[2J\033[H"
#define CIVIS "\033[?25l"
#define CNORM "\033[34h\033[?25h"

char* COLORS[3] = {"\e[0;31m", "\e[0;34m", "\e[0;32m"};

// look pretty stuff

void cool_print(char* msg, useconds_t delay) {
	// this function is for cool printing
	int i = 0;
	do {
		putchar(msg[i++]);
		usleep(delay);
	} while (msg[i]);
}

void banner() {
	// feel free to ignore this function
	// cool like star wars :)
	cool_print(CIVIS CLRSCR"\n\n", 0);
	cool_print(BHRED"\tPROVE YOUR WORTH.", 5000);
	usleep(500000);
	cool_print(BHBLU"\tBUILD YOUR SABER.", 5000);
	usleep(500000);
	cool_print(BHGRN"\tSHOW YOUR STRENGTH.\n\n", 5000);
	usleep(100000);
	cool_print(BHMAG"\t▄▄▄█████▓ ██░ ██ ▓█████      █████▒▒█████   ██▀███    ▄████ ▓█████ \n", 500);
	cool_print("\t▓  ██▒ ▓▒▓██░ ██▒▓█   ▀    ▓██   ▒▒██▒  ██▒▓██ ▒ ██▒ ██▒ ▀█▒▓█   ▀ \n", 500);
	cool_print("\t▒ ▓██░ ▒░▒██▀▀██░▒███      ▒████ ░▒██░  ██▒▓██ ░▄█ ▒▒██░▄▄▄░▒███   \n", 500);
	cool_print("\t░ ▓██▓ ░ ░▓█ ░██ ▒▓█  ▄    ░▓█▒  ░▒██   ██░▒██▀▀█▄  ░▓█  ██▓▒▓█  ▄ \n", 500);
	cool_print("\t  ▒██▒ ░ ░▓█▒░██▓░▒████▒   ░▒█░   ░ ████▓▒░░██▓ ▒██▒░▒▓███▀▒░▒████▒\n", 500);
	cool_print("\t  ▒ ░░    ▒ ░░▒░▒░░ ▒░ ░    ▒ ░   ░ ▒░▒░▒░ ░ ▒▓ ░▒▓░ ░▒   ▒ ░░ ▒░ ░\n", 500);
	cool_print("\t    ░     ▒ ░▒░ ░ ░ ░  ░    ░       ░ ▒ ▒░   ░▒ ░ ▒░  ░   ░  ░ ░  ░\n", 500);
	cool_print("\t  ░       ░  ░░ ░   ░       ░ ░   ░ ░ ░ ▒    ░░   ░ ░ ░   ░    ░   \n", 500);
	cool_print("\t          ░  ░  ░   ░  ░              ░ ░     ░           ░    ░  ░\n\n"RESET CNORM, 500);
}

// the 3 forging outcomes

void crash() {
	printf(BHYEL"\n\tThe results of your forgery, ");
	printf(BHRED"nothing more than plain rubbish.\n");
	printf(BHYEL"\tCome back when you can prove yourself worthy.\n"RESET);
	exit(-1);
}

void verdict() {
	printf(BHYEL"\n\tThe results of your forgery, just average.");
	printf("\n\tYou can do better.\n"RESET);
	exit(-1);
}

void a_worthy_saber(char* answer) {
	// a worthy saber deserves a worthy flag :)
	FILE* f = fopen("i_am_worthy", "r");
	if (f == 0) {
		puts("Flag file not found!");
		exit(-1);
	}
	char* flag = malloc(0x100);

	if (strcmp(answer, "worthy")) {
		fclose(f);
		crash();
	}

	fread(flag, 0x100, 1, f);
	fclose(f);
	printf(BHYEL"\n\tThe results of your forgery, you have proven yourself worthy.\n\tTake this with you: %s\n"RESET, flag);
	free(flag);
	exit(0);
}

void setup() {
	// ignore this!!
	// setbuf is not of any interest to you.
	setbuf(stdout, 0);
	setbuf(stdin, 0);

	signal(SIGSEGV, crash);
}


void craft_saber(char* color) {
	uint8_t dimensions[2] = {6, 20}; // width, height

	printf("%s", color);
	printf("\t\t\t ");
	for (int i = 0; i < dimensions[0]; i++) {
		printf("▂");
	}
	printf("\n\t\t\t");
	for (int j = 0; j < dimensions[1]; j++) {
		printf("▐");
		for (int i = 0; i < dimensions[0]; i++) {
			printf("█");
		}
		printf("▌\n\t\t\t");
	}

	for (int i = 0; i<(dimensions[0]/2)-1; i++) {putchar(0x20);}
	printf(" ━━\n\t\t\t");
	for (int i = 0; i<(dimensions[0]/2)-1; i++) {putchar(0x20);}
	printf("▕ ▕\n\t\t\t");
	for (int i = 0; i<(dimensions[0]/2)-1; i++) {putchar(0x20);}
	printf("▕░▕\n\t\t\t");
	for (int i = 0; i<(dimensions[0]/2)-1; i++) {putchar(0x20);}
	printf("▕ ▕\n\t\t\t");
	for (int i = 0; i<(dimensions[0]/2)-1; i++) {putchar(0x20);}
	printf(" ▔▔\n\t\t\t"RESET);
}
void prepare_the_fire() {
	// make code section writable, so we can modify our saber to our liking
	mprotect((void*)((long)craft_saber - ((long)craft_saber % 0x1000)), 0x1000, 7);
}

void quelling_the_fire() {
	// make code section non-writable, as it should be
	mprotect((void*)((long)craft_saber - ((long)craft_saber % 0x1000)), 0x1000, 5);
}


void forge() {
	int length = 0, width = 0, color_index = 0;
	char color[0x20];

	printf(BHYEL"\t COLOR (RED/BLUE/GREEN)\t: "RESET );
	scanf("%s", color);  // we love some color!
	printf(BHYEL"\t\tLENGTH (0-255)\t: "RESET);
	scanf("%d", &length);
	printf(BHYEL"\t\t WIDTH (0-255)\t: "RESET);
	scanf("%d", &width);

	// only 3 colors: RED BLUE GREEN
	if (!strcmp(color, "RED")) {
		color_index = 0;
	} else if (!strcmp(color, "BLUE")) {
		color_index = 1;
	} else if (!strcmp(color, "GREEN")) {
		color_index = 2;
	} else {
		puts("\tDon't mess with me.");
		exit(1);
	}

	printf(BHMAG"\n\tPREPARING THE FIRE");
	prepare_the_fire();
	cool_print("...", 10000);

	*((uint8_t*)craft_saber+20) = width;
	*((uint8_t*)craft_saber+21) = length;

	printf(BHMAG"\n\tQUELLING THE FIRE");
	quelling_the_fire();
	cool_print("...", 10000);

	printf(BHMAG"\n\tIT IS READY");
	cool_print("...", 10000);
	printf("\n\n");

	craft_saber(COLORS[color_index]);
}

int main() {
	setup();
	banner();
	forge();
	verdict();
}
