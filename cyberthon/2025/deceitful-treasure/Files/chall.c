#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

// ANSI Escape Codes for colors
#define RESET	"\x1b[0m"
#define RED		"\x1b[31m"
#define GREEN	"\x1b[32m"
#define YELLOW	"\x1b[33m"
#define BLUE	"\x1b[34m"
#define CYAN	"\x1b[36m"

char flag[] = "Cyberthon{this is a test flag. exploit the remote instance for the actual flag!}";

// Function to print ASCII art for the start
void print_intro() {
	printf(YELLOW);
	printf("	 _______\n");
	printf("	/		\\ \n");
	printf("   /  _   _  \\ \n");
	printf("  /  | | | |  \\ \n");
	printf(" /	 | | | |   \\ \n");
	printf("/____|_|_|_|____\\\n");
	printf("   Treasure Hunt\n");
	printf(RESET);
	printf("\nWelcome to the Treasure Hunt CTF Challenge!\n");
	printf("Your mission is to solve a series of clues and find the hidden treasure.\n");
}

// Function to handle the first clue
void clue1() {
	char answer[256];
	
	printf(CYAN);
	printf("\nYou are standing in a dense, foggy forest. A sign appears in front of you.\n");
	printf("It reads: 'Find the key to open the gate'.\n");
	printf("What is the key? (Hint: It's something you can read)\n");
	printf(RESET);

	scanf("%256[^\n]", answer);
	getchar();

	if (strcmp(answer, "book") == 0) {
		printf(GREEN "Correct! You've found the key in a hidden book.\n" RESET);
	} else {
		printf(RED "Incorrect answer. Try again.\n" RESET);
		clue1();  // Retry the clue
	}
}

// Function to handle the second clue
void clue2() {
	char answer[256];
	
	printf(CYAN);
	printf("\nYou walk through the gate and into a dark cave. Strange symbols line the walls.\n");
	printf("One of them is glowing brightly.\n");
	printf("What does the glowing symbol represent? (Hint: Think about a common shape)\n");
	printf(RESET);

	scanf("%256[^\n]", answer);
	getchar(); // consume newline

	if (strcmp(answer, "circle") == 0) {
		printf(GREEN "Correct! The glowing symbol is a circle, revealing a hidden path.\n" RESET);
	} else {
		printf(RED "Incorrect answer. Try again.\n" RESET);
		clue2();  // Retry the clue
	}
}

// Function to handle the final treasure clue
void clue3() {
	char answer[256];
	
	printf(CYAN);
	printf("\nYou enter the deepest part of the cave and find a treasure chest.\n");
	printf("A riddle is etched into the chest:\n");
	printf("I am the key to your success, I hold the number of days in a week. What number am I?\n");
	printf(RESET);

	scanf("%256[^\n]", answer);
	getchar();

	if (strcmp(answer, "7") == 0) {
		printf(GREEN "Correct! The chest opens, and you find the treasure inside.\n" RESET);
		printf(GREEN "Congratulations, you've completed the treasure hunt!\n" RESET);
	} else {
		printf(RED "Incorrect answer. Try again.\n" RESET);
		clue3();  // Retry the final clue
	}
}

// hehe, we don't want to give them the flag do we?
int hide_treasure() {
	void* rand_addr = 0x0;

	int rand_fd = open("/dev/urandom", O_RDONLY);
	read(rand_fd, &rand_addr, 2);
	rand_addr = (void*)((long)rand_addr << 12);

	rand_addr = mmap(rand_addr, 0x1000, 6, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0);
	if (rand_addr == MAP_FAILED) {
		puts("mmap failed!");
		exit(0);
	}

	strcpy(rand_addr, flag);
	memset(flag, 0, sizeof(flag));

	rand_addr = 0;
	close(rand_fd);
	rand_fd = 0;
	return 0;
}

int main() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	hide_treasure();
	print_intro();
	clue1();
	clue2();
	clue3();
	return 0;
}
