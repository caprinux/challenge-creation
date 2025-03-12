#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BOOK_NAME_SIZE 0x6
#define PAGE_SIZE 0x100

char ascii_art[] =
"\e[0;31m"
"\t⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀\n"
"\t⠀⠀⠀⠀⠀⣀⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀\n"
"\t⠀⠀⠀⠀⠀⠘⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠈⠑⠀⠀⠀⠀⠀⠀⠀⠀\n"
"\t⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"\t⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⠛⠁⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀\n"
"\t⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠙⢻⡇⠀⠀⠀⠀⠀⠀⠀⠐⠻⠏⠀⠀⠀⠀⠀⠀\n"
"\t⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"\t⠀⠀⠀⠀⣀⡀⢠⣴⣶⣿⣿⣿⣿⣿⡆⢰⣶⠶⠶⠶⠶⠦⣤⡄⢀⣀⠀⠀⠀⠀\n"
"\t⠀⠀⠀⠀⣿⠁⣼⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⣶⣶⣶⣶⣶⣿⣧⠈⣿⠀⠀⠀⠀\n"
"\t⠀⠀⠀⢠⡍⢀⣿⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⠛⠛⠛⠛⠛⠻⣿⡀⢻⡇⠀⠀⠀\n"
"\t⠀⠀⠀⠛⠃⣸⣿⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⠛⠛⠛⣿⡟⠛⢻⣇⠘⣷⠀⠀⠀\n"
"\t⠀⠀⢰⡟⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⠛⠛⠛⠛⠛⠛⠛⣿⡀⢻⡄⠀⠀\n"
"\t⠀⠀⣾⡇⠘⠟⠛⠛⠉⣉⣉⣉⡉⠛⠃⠘⠛⠛⠛⠛⠛⠛⠛⠲⠿⠃⢸⣧⠀⠀\n"
"\t⠀⢀⣉⣁⣀⣀⣉⣉⣉⣉⣉⣉⣉⣉⣁⣈⣉⣉⣉⣉⣉⣉⣁⣀⣀⣀⣈⣉⡀⠀\n"
"\t⠀⠘⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠃⠀\n"
"\t  welcome to the book factory \n"
"\e[0m";

void vuln() {
	char book_name[BOOK_NAME_SIZE] = {0};
	char page_to_print[PAGE_SIZE] = {0};
	unsigned int num_pages;

	puts("if you write me a good book, i might print it for you");
	printf("book name (%u characters): ", BOOK_NAME_SIZE-1);
	size_t sz = read(0, book_name, BOOK_NAME_SIZE-1);
	if (book_name[sz-1] == '\n')
		book_name[sz-1] = 0;

	printf("how many pages (max %u): ", 10);
	scanf("%u", &num_pages);
	getchar();

	if (num_pages > 10) {
		puts("That is too many pages for this book!");
		return;
	}

	if (num_pages == 0) {
		puts("You have to write at least one page :/");
		return;
	}

	printf("\nyour book '");
	printf(book_name);
	printf("' will have %u pages. write the book!\n", num_pages);

	char** pages = calloc(num_pages, sizeof(char*));
	char* book = calloc(num_pages * PAGE_SIZE, sizeof(char));

	for (int i = 0; i < num_pages; i++) {
		pages[i] = &book[i * PAGE_SIZE];
		printf("Page %u > ", i);
		read(0, pages[i], PAGE_SIZE);
		if (pages[i][sz-1] == '\n')
			pages[i][sz-1] = 0;
	}

	unsigned int chosen_page;
	printf("\nyour book is decent. pick a page and i will print it for you (0 - %u): ", num_pages-1);
	scanf("%u", &chosen_page);
	getchar();

	if (chosen_page >= num_pages) {
		puts("Invalid page!");
		return;
	}

	strcpy(page_to_print, pages[chosen_page]); // uwu rat2libz?
	printf("\nheres your page:\n%s\n", page_to_print);
}

int main() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	puts(ascii_art);
	vuln();
}
