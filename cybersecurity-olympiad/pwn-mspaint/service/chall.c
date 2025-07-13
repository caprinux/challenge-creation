#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CLRSCR "\e[2J\e[H"
#define BOLDRED "\e[1;91m"
#define BOLDBLUE "\e[1;94m"
#define BOLDYELLOW "\e[1;93m"
#define GREENUNDERLINE "\e[4;32m"
#define YELLOW "\e[0;33m"
#define CYAN "\e[1;36m"
#define REDUNDERLINE "\e[4;31m"
#define RST "\e[0m"
#define NROWS 20
#define NCOLS 40

int calculate_pixel_score() {
	// totally compliacted score calculation algorithm XDXD
	return ((rand() % 3) - 1);
}

void print_flag() {

	char* flag;
	size_t sz;
	FILE *f = fopen("flag.txt", "r");

	if (f == NULL) {
		puts("Failed to read flag.txt.");
		puts("Remember to run your exploit remotely!");
	} else {
		fseek(f, 0, SEEK_END);
		sz = ftell(f);
		fseek(f, 0, SEEK_SET);
		flag = calloc(sz+1, sizeof(char));
		fgets(flag, sz, f);
		fclose(f);
		puts(flag);
	}
}

int msdraw() {
	int score = 0;
	int cury = NROWS/2;
	int curx = NCOLS/2;
	char canvas[NROWS * NCOLS];
	char cmd[100];
	memset(canvas, '.', NROWS*NCOLS);

	while (1) {
		printf(CLRSCR); // clear the screen
		printf(CYAN"┌");
		for (int i = 0; i < NCOLS; i++) {
			printf("─");
		}
		printf("┐\n"RST);
		for (int i = 0; i < NROWS; i++) {
			for (int j = 0; j < NCOLS; j++) {
				if (j == 0) {
					printf(CYAN"│"RST);
				}
				if (cury*NCOLS+curx == i*NCOLS+j) {
					printf(BOLDYELLOW"^"RST);
				} else {
					putchar(canvas[i*NCOLS + j]);
				}
			}
			printf(CYAN"│"RST);
			if (i == 2) {
				puts(REDUNDERLINE"\tCONTROLS"RST);
			}
			else if (i == 3) {
				puts(BOLDBLUE"\tw"RST " - move cursor up");
			}
			else if (i == 4) {
				puts(BOLDBLUE"\ta"RST " - move cursor left");
			}
			else if (i == 5) {
				puts(BOLDBLUE"\ts"RST " - move cursor right");
			}
			else if (i == 6) {
				puts(BOLDBLUE"\td"RST " - move cursor down");
			}
			else if (i == 7) {
				puts(BOLDBLUE"\to"RST " - paint a pixel at cursor");
			}
			else if (i == 8) {
				puts(BOLDBLUE"\tx"RST " - erase at cursor");
			}
			else if (i == 9) {
				puts(BOLDBLUE"\tq"RST " - end the msdraw session");
			}
			else if (i == 11) {
				puts(REDUNDERLINE"\tLEGEND"RST);
			}
			else if (i == 12) {
				puts(BOLDYELLOW"\t^"RST" - cursor");
			}
			else if (i == 13) {
				puts("\tO - painted pixel");
			}
			else if (i == 15) {
				printf(GREENUNDERLINE"\tSCORE"RST": %d\n", score);
			}
			else if (i == 17) {
				puts("\tnote: you can enter multiple commands at once!");
			} else {
				putchar(0xa);
			}
		}
		printf(CYAN"└");
		for (int i = 0; i < NCOLS; i++) {
			printf("─");
		}
		printf("┘\n"RST);
		printf("msdraw$ ");
		scanf("%100s", cmd);

		for (int i = 0; i < strlen(cmd); i++) {
			char c = cmd[i];
			if (c == 'w') {
				cury -= 1;
			}
			if (c == 'a') {
				curx -= 1;
			}
			if (c == 's') {
				cury += 1;
			}
			if (c == 'd') {
				curx += 1;
			}
			if (c == 'o') {
				canvas[(cury-1)*NCOLS + curx] = 'O';
				score += calculate_pixel_score();
			}
			if (c == 'x') {
				canvas[(cury-1)*NCOLS + curx] = ' ';
				score -= calculate_pixel_score();
			}
			if (c == 'q') {
				return score;
			}
		}

	}

}

int main() {

	setbuf(stdin, 0); // ignore
	setbuf(stdout, 0); // ignore

	// prompt
	puts(CLRSCR"\nWelcome to cmdpaint!"); // mspaint but command line painting!
	puts("Beat the high-score of " BOLDRED "13371337" RST " to win!!");
	printf("Press Enter to start!");
	getchar();

	int score = msdraw(); // actual game

	// check socre if won
	if (score > 13371337) {
		printf("WOW! You hit a HIGH SCORE of %d.\nAre you the next picasso or are you just a talented hacker? Anyways you deserve this: ", score);
		print_flag();
	} else {
		printf("\nYou got a pretty high... score of %d.\nThat's still far off the current high-score of 13371337 though, but it's a good effort!\nTry again when you're more prepared (to hack it) ~\n", score);
	}

	return 0;
}
