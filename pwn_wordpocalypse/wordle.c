#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define RED 1
#define GREEN 2
#define BLACK 3
#define YELLOW 4
#define ARED "\e[0;91m"
#define ABLK "\e[0;96m"
#define ARST "\e[0m"
#define MAX_ATTEMPTS 5

char correct_word[] = "havoc";
size_t centre_x = 0;
size_t centre_y = 0;
int num_boxes = 0;
char wordle_board[MAX_ATTEMPTS*5] = { 0 };

void win() {

	char flag[100];

	printf("%s%s%s", ARED, "A qualified agent... welcome to APOCALYPSE. Hope you didn't destroy too much ony our way here >:)", ARST);
	endwin();
	system("/bin/sh");
	exit(0);

}

int next_guess(char* buf) {
	num_boxes += 1;
	WINDOW* win = newwin(3, 31, (num_boxes)*2 + 1, centre_x);
	wattron(win, COLOR_PAIR(RED) | A_BOLD);
	box(win, 0, 0);
	wattroff(win, COLOR_PAIR(RED) | A_BOLD);
	for (int i = 1; i <= 5; i++) {
		mvwaddch(win, 1, 5*i, '_');
	}
	wmove(win, 1, 3);
	wrefresh(win);
	int i = 0;
	int ch = 0;
	while (i < 5) {
		i += 1;
		ch = wgetch(win);
		if (ch == 127 || ch == KEY_BACKSPACE || ch == KEY_DC) {
			if (i > 1) {
				i -= 2;
				mvwaddch(win, 1, 5*(i+1), '_');
			} else if (i == 1) {
				i -= 1;
			}
		} else {
			mvwaddch(win, 1, 5*i, ch);
			buf[i-1] = ch;
		}
		wrefresh(win);
	}

	int correct = 0;

	// first we recolor the current box
	box(win, 0, 0);

	// now we check if the word is correct
	for (int i = 0; i < 5; i += 1) {
		if (correct_word[i] == buf[i]) {
			wattron(win, COLOR_PAIR(GREEN));
			mvwaddch(win, 1, 5*(i+1), buf[i]);
			wattroff(win, COLOR_PAIR(GREEN));
			correct += 1;
		} else {
			wattron(win, COLOR_PAIR(RED));
			mvwaddch(win, 1, 5*(i+1), buf[i]);
			wattroff(win, COLOR_PAIR(RED));
		}
		wrefresh(win);
	}

	if (correct == 5) {
		endwin();
		printf("%sDid you think it was so simple?%s\n", ARED, ARST);
		return 1;
	}
	return 0;
}

void init_wordle(int attempts) {
	
	// do some initialization stuff
	initscr();
	raw();
	keypad(stdscr, true);
	curs_set(0);
	noecho();
	start_color();
	if (!has_colors()) {
		printw("Terminal does not support color.");
		getch();
		endwin();
		exit(1);
	}
	init_pair(BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair(RED, COLOR_RED, COLOR_BLACK);
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
	bkgd(COLOR_PAIR(BLACK));

	// check if player's terminal is big enough for APOCALYPSE WORDLE
	size_t max_x = getmaxx(stdscr);
	size_t max_y = getmaxy(stdscr);
	size_t max_cols = max_y / 3;
	centre_x = (max_x / 2) - 15;
	centre_y = (max_y / 2);
	if ((max_x / 5) <= 0 || (max_x/35) <= 0) {
		endwin();
		puts("Your terminal has to be at least 5 cells in height and 35 cells in width!");
		exit(0);
	}

	// main game
	attron(A_BLINK | A_STANDOUT | COLOR_PAIR(RED));
	mvprintw(1, centre_x + 11,  " WORDLE ");
	attroff(A_BLINK | A_STANDOUT | COLOR_PAIR(RED));
	refresh();
	int curr_col = 0;
	for (int i = attempts; i != 0; i -= 1) {
		// we want to make sure that the number of boxes do not render beyond the terminal
		if (curr_col >= max_cols) {
			num_boxes = 0;
			curr_col = 0;
		}
		if (next_guess(wordle_board+(i*5))) {
			attron(COLOR_PAIR(RED));
			mvprintw(centre_y, centre_x+11, " YOU WIN! ");
			mvprintw(centre_y+1, centre_x +2, " PRESS ANY KEY TO CONTINUE... ");
			attroff(COLOR_PAIR(RED));
			refresh();
			getch();
			endwin();
			return;
		}
		curr_col += 1;
	}

	// player failed
	attron(COLOR_PAIR(RED));
	mvprintw(centre_y, centre_x + 11, " GAME OVER ");
	mvprintw(centre_y+1, centre_x +2, " PRESS ANY KEY TO CONTINUE... ");
	attroff(COLOR_PAIR(RED));
	refresh();
	getchar();
	endwin();
	printf("Mere %sweakling.%s\n", ARED, ARST);
}

void setup() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	setbuf(stderr, 0);
}

void menu() {
	printf("%s%s%s", ARED, "------------------ THE WORD-POCALYPSE -----------------\n", ARST);
	printf("%s%s%s", ABLK, "Prove your worth and I will give you what you desire...\n", ARST);
	printf("1. Start Game\n");
	printf("2. Quit\n");
	printf("%s%s%s", ARED, "---------------------------------------------------------\n", ARST);
}

void __attribute__((noreturn)) main() {

	int opt = 0;
	setup();
	menu();
	printf("Choice: ");
	scanf("%d", &opt);
	getchar();
	switch (opt) {
		case 1:
			printf("Show us you are worthy.\n");
			printf("How many attempts do you need: ");
			scanf("%d", &opt);
			getchar();
			printf("\n");
			if (opt > 5) {
				puts("Apocalypse agents can do better than that.");
				exit(0);
			}
			init_wordle(opt);
			exit(0);
		case 2:
			printf("\nKnowing %swhen to quit%s is commendable.\n", ARED, ARST);
			exit(0);
		default:
			printf("Don't try to cheat us.\n");
			exit(0);
	}
}
