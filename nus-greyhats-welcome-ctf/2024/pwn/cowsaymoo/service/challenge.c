#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void setup() // safely ignore this
{
    setbuf(stdout, 0);
    setbuf(stdin, 0);
}

int main() {
    setup();
    char command[80];
    char name[80];
    strcpy(command, "cowsay ");

    printf("your name: ");
    gets(name);
	putchar(0xa);

	if (strchr(name, '\''))
		*strchr(name, '\'') = 0;

	snprintf(command + 7, 80-7, "'hello %s!'", name); 
    system(command);
}
