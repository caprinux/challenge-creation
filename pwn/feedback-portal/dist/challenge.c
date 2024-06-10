#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

char* feedbacks[100];
size_t sizes[100];
int cur = 0;

int print_banner(int a, int b, int c) {
	char banner[0x100] = {0};
	FILE *f = fopen("banner.txt", "r");
	fread(banner, 1, 0x100, f);
	puts(banner);
	return fclose(f);
}

void setup() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	setbuf(stderr, 0);
	print_banner(0,0,0);
}

size_t read_input(char* buf, size_t buf_size, size_t input_size) {
	if (input_size > buf_size) {
		buf = realloc(buf, input_size);
		buf_size = input_size;
	}

	for (size_t i = 0 ; i < input_size; i++) {
		if (read(0, buf+i, 1) == -1) {
			break;
		}
	}

	return buf_size;
}

void menu() {
	puts(
		"Submit feedback for R3CTF 2024!\n"
		"1. Create new feedback submission\n"
		"2. Modify your feedback submission\n"
		"3. Revoke your feedback submission\n"
		"4. Exit the feedback portal"
	);
}

void create() {
	if (cur >= 100) {
		puts("Too much feedback is not good...");
		return;
	}
	printf("how long is your feedback?\n> ");
	scanf("%zu", &sizes[cur]);
	getchar();
	feedbacks[cur] = malloc(sizes[cur]);
	printf("feedback: ");
	read(STDIN_FILENO, feedbacks[cur], sizes[cur]);
	printf("submitted feedback #%d\n\n", cur);
	cur++;
}

void edit() {
	unsigned int idx = 0;
	printf("which feedback submission would you like to modify?\n> ");
	scanf("%d", &idx);
	getchar();
	if (idx >= cur) {
		puts("you haven't even submitted that many feedbacks...");
		return;
	}
	if (!feedbacks[idx]) {
		puts("you've already deleted that feedback!");
		return;
	}
	printf("input size> ");
	size_t input_size;
	scanf("%zu", &input_size);
	getchar();
	printf("feedback: ");
	sizes[idx] = read_input(feedbacks[idx], sizes[idx], input_size);

	printf("re-submitted feedback #%d\n\n", idx);
}

void delete() {
	unsigned int idx = 0;
	printf("which feedback submission would you like to revoke?\n> ");
	scanf("%d", &idx);
	getchar();
	if (!feedbacks[idx] && (idx < cur)) {
		puts("you've already deleted that one!!\n");
		return;
	} else if (!feedbacks[idx]) {
		puts("you haven't even submitted that many feedbacks...\n");
		return;
	}
	free(feedbacks[idx]);
	feedbacks[idx] = 0;
	printf("revoked feedback #%d\n\n", idx);

}

void app(size_t* sizes, void** buffers) {
	int opt;
	while (1) {
		menu();
		printf("> ");
		scanf("%d", &opt);
		getchar();
		putchar(0xa);
		switch (opt) {
			case 1:
				create();
				break;
			case 2:
				edit();
				break;
			case 3:
				delete();
				break;
			case 4:
				return;
		}
	}
}

int main() {
	size_t sizes[20];
	void* buffers[20];
	setup();
	app(sizes, buffers);

	return 0;
}
