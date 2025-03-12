#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char ascii_art[] =

"\e[0;33m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠸⠿⠿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣿⡟⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⣿⣷⠿⠃⢠⣤⣴⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠀⠀⠀⣿⣏⣤⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡈⠉⠁⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢺⣿⣯⠆⠀⣠⣶⠿⣛⠛⠳⣦⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣙⣤⣤⣼⡏⠹⠛⠋⢁⣠⣿⠋⠉⠛⠳⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣴⡶⠾⣿⠟⢉⣭⡿⠛⠁⠀⠀⢰⠟⠉⠀⠀⠀⠀⠀⠀⠙⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⡾⠟⠛⠉⠀⠀⠀⣿⣾⠟⠁⠀⠀⠀⠀⠀⠘⣷⡀⠀⠀⠀⠀⠀⠀⠀⠈⢿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⠟⠋⣿⠀⠀⠀⠀⠀⠀⢰⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠘⣷⡀⠀⠀⠀⠀⠀⠀⠀⠘⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠋⠁⠀⠀⣿⠀⠀⠀⠀⠀⠀⣿⣧⣿⠀⠀⢰⡷⠀⠀⠀⠀⢠⡟⠃⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡾⠋⠀⠀⠀⠀⠀⣿⡟⠀⠀⠀⠀⠀⢿⡏⠛⢀⠀⠙⠃⠀⠀⠀⢠⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⡟⠀⠀⠀⠀⢰⣄⠀⠀⣰⡄⠀⠀⠀⠀⠘⢿⣄⢻⣖⡀⠀⠀⠀⠀⢸⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⡟⠨⠻⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠈⠙⠛⠷⢾⣧⣀⠀⠀⠀⠀⢠⣿⣧⣉⢷⠀⠀⠀⠀⢸⡆⠀⠀⠀⠀⠀⠀⠀⠀⣴⡟⠛⠛⠀⠈⠻⣦⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣙⣻⣦⣴⠶⣿⡛⢻⡙⠻⢧⣄⣀⠀⠈⢿⣄⠀⠀⠀⠀⢀⣴⡾⠋⠀⠀⠀⠀⠀⠀⢹⣇⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⢀⣤⡾⠋⠉⠻⣦⣀⠀⠀⠀⠀⢀⣤⣶⣿⠛⠉⠁⠀⠀⠈⣿⡈⣿⠀⠀⠉⠙⠛⠶⠶⣽⣷⣤⣶⠞⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⣾⠻⣦⡄⠀⠀⠀⠀\n"
"⠀⠀⠀⢀⣴⡾⠋⠁⠀⠀⠀⠀⠈⠙⠳⢶⣤⣄⣿⡵⣟⣿⢷⡄⠀⠀⠀⠀⣿⠇⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⡾⠏⠀⠈⠻⣦⡀⠀⠀\n"
"⢀⣤⣾⣛⣁⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣵⣯⣾⢋⣁⣤⣴⠶⠾⠛⠶⠾⠷⢶⣦⣤⣤⣤⣤⣤⣤⡤⠶⠶⠶⠶⠶⠿⠿⠛⠋⠉⠉⠀⠀⠀⠀⠀⠈⠻⣦⡀\n"
"⠀⠉⠙⠛⣿⡛⠛⠛⠛⠳⠶⠶⠶⠶⠦⠤⢤⣤⣤⣤⣤⣽⣯⣥⣠⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣄⣤⣤⣤⡤⠤⠴⢶⡾⠟⠛⠃\n"
"⠀⠀⠀⠀⢹⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⣾⠃⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⢻⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡏⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠈⠿⠿⠿⠿⣿⠿⠿⠿⢿⡶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⢶⣶⠶⢶⠶⣶⣶⣶⣶⣶⡿⠁⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡇⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣧⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⢸⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡀⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⣼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠓⠚⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠚⠓⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"         class was so boring, you fell fast asleep...        \n"
"               tell me about your dreams?...                 \n\e[0m";

char* notes[10];
int note_ctr = 0;

void take_note() {
	size_t size;

	if (note_ctr < 10) {
		printf("note size: ");
		scanf("%zu", &size);
		getchar();

		if (size > 0x100) {
			puts("your notebook is not that big!");
			return;
		}

		notes[note_ctr] = malloc(size);
		printf("note content: ");
		size_t sz = read(STDIN_FILENO, notes[note_ctr], size);
		if (notes[note_ctr][sz-1] == '\n') {
			notes[note_ctr][sz-1] = 0;
		}
		printf("you took down note #%d into your notebook!\n", note_ctr);
		note_ctr++;
	} else {
		puts("your notebook ran out of space");
		return;
	}
}

void erase_note() {
	unsigned int idx;
	printf("note index to remove: ");
	scanf("%u", &idx);
	getchar();

	if (idx < 10 && notes[idx]) {
		free(notes[idx]);
		notes[idx] = 0;
		printf("note #%u is removed\n", idx);
	} else {
		puts("that note does not exist!");
	}
}

void read_note() {
	unsigned int idx;
	printf("note index to read: ");
	scanf("%u", &idx);
	getchar();

	if (idx < 10 && notes[idx]) {
		printf("note #%u: %s\n", idx, notes[idx]);
	} else {
		puts("that note does not exist!");
	}

}

void class() {

	int opt;
	int done = 1;

	while(done) {
		puts("\n1) take down a note");
		puts("2) erase a note");
		puts("3) read a note");
		puts("4) go back");
		printf("> ");
		scanf("%d", &opt);
		getchar();

		switch(opt) {
			case 1:
				take_note();
				break;
			case 2:
				erase_note();
				break;
			case 3:
				read_note();
				break;
			case 4:
				done = 0;
				break;
			default:
				break;
		}
	}
}

typedef void func();
func** dreams;
size_t num_dreams;
size_t dream_i;

void dream_about_kdrama_guys() {
	puts("oooooooooooooooooooooooooo, cute kdrama guy.......");
}

void dream_about_flag_real() {
	char flag[100] = {0};
	FILE *f = fopen("flag.txt", "r");
	fread(flag, 100, sizeof(char), f);
	fclose(f);
	printf("and the flag is.............. %s............\n", flag);
}

void dream_about_flag_fake() {
	printf("and the flag is.............. i forgot............\n");
}

void dream_about_valorant() {
	puts("THWACK! THWACK! THWACK! THWACK! THWACK! ACE!......");
}

void dream_about_school() {
	puts("and the best STUDENT award goes to...........");
}

func* pick_a_dream() {
	int opt;

	while (1) {
		puts("\n1) dream about valorant");
		puts("2) dream about kdrama guys");
		puts("3) dream about school");
		puts("4) dream about flag");
		printf("> ");
		scanf("%d", &opt);
		getchar();

		switch(opt) {
			case 1:
				return (func*)&dream_about_valorant;
			case 2:
				return (func*)&dream_about_kdrama_guys;
			case 3:
				return (func*)&dream_about_school;
			case 4:
				return (func*)&dream_about_flag_fake;
			default:
				break;
		}
	}
}

void add_dream() {
	if (dreams == NULL) {
		puts("you currently have no dreams planned.");
		printf("how many dreams are you planning to have? ");
		scanf("%zu", &num_dreams);
		if (num_dreams > 100) {
			puts("that's too many dreams to have in a single nap!");
			num_dreams = 0;
			return;
		}
		dreams = malloc(sizeof(func*) * num_dreams);
	}

	if (dream_i < num_dreams) {
		dreams[dream_i] = pick_a_dream();
		dream_i++;
		printf("you currently have %zu/%zu dreams prepared!\n", dream_i, num_dreams);
	} else {
		puts("you have already finished planning all your dreams!");
	}
}

void start_dreaming() {

	for (size_t i = 0; i < num_dreams; i++) {
		if (dreams[i]) {
			((func*)(dreams[i]))();
		} else {
			break;
		}
		printf(".");
		printf(".");
		printf(".\n");
	}

	puts("you woke up from your dream -- 'wow what a good dream!'");
	num_dreams = 0;
	dream_i = 0;
	free(dreams);
	dreams = NULL;

}

void dream() {
	int opt;
	int done = 1;

	while(done) {
		puts("\n1) add a dream");
		puts("2) start dreaming!");
		puts("3) go back");
		printf("> ");
		scanf("%d", &opt);
		getchar();

		switch(opt) {
			case 1:
				add_dream();
				break;
			case 2:
				start_dreaming();
				break;
			case 3:
				done = 0;
				break;
			default:
				break;
		}
	}

}

void menu() {

	int opt;

	puts("\n1) listen to class");
	puts("2) start dreaming");
	puts("3) exit");
	printf("> ");
	scanf("%d", &opt);
	getchar();

	switch(opt) {
		case 1:
			class();
			break;
		case 2:
			dream();
			break;
		case 3:
		default:
			exit(0);
	}
}

int main() {

	setbuf(stdin, 0);
	setbuf(stdout, 0);
	printf("%s", ascii_art);
	while (1) {
		menu();
	}

}

