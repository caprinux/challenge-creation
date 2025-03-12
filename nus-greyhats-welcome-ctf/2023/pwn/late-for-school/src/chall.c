// gcc chall.c -no-pie -fno-stack-protector -o chall

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define BLK "\e[1;30m"
#define RED "\e[1;31m"
#define GRN "\e[1;32m"
#define YEL "\e[1;33m"
#define BLU "\e[1;34m"
#define MAG "\e[1;35m"
#define CYN "\e[1;36m"
#define WHT "\e[1;37m"
#define RESET "\e[0m"

char late[] = 
"     __________________________________\n"
"    |.===============================,|\n"
"    ||  I WILL NOT BE LATE TO CLASS  ||\n"
"    ||  I WILL NOT BE LATE TO CLASS  ||\n"
"    ||  I WILL NOT BE LATE TO CLASS  ||\n"
"    ||  I .----.OT B,                ||\n"
"    ||   / ><   \\  /                 ||\n"
"    ||  |        |/\\                 ||\n"
"    ||   \\______//\\/                 ||\n"
"    ||   _(____)/ /                  ||\n"
"    ||__/ ,_ _  _/___________________||\n"
"    '===\\___\\_) |====================='\n"
"         |______|\n"
"         |  ||  |\n"
"         |__||__|\n"
"         (__)(__)\n";

char book[] = 
"            ,     ,\n"
"           ///////|\n"
"          /////// |\n"
"         ///////  |\n"
"        |~~~~~|   |\n"
"        |=====|   |\n"
"        |  H  |   |\n"
"        |  a  |   |\n"
"        |  c  |   |\n"
"        |  k  |   |\n"
"        |  i  |   |\n"
"        |  n  |   |\n"
"        |  g  |   |\n"
"        |  1  |   |\n"
"        |  0  |   |\n"
"        |  1  |  /\n"
"        |=====| /\n"
"        '-----'`\n";

char bed[] = 
" .::\"\"-,                      .::\"\"-.\n"
"/::     \\                    /::     \\\n"
"|::     |   _..--\"\"\"\"--.._   |::     |\n"
"'\\:.__ /  .'              '.  \\:.__ /\n"
" ||____|.'                  '.||____|\n"
" ||:.  |                       |:.  |\n"
" ||:.  |                       |:.  |\n"
" ||:.  |                       |:.  |\n"
" ||:.  |  _..---\"````````'---. |:.  |\n"
" ||:.  | `                     \\:.  |\n"
" ||:.  |: :                .--._.-\"\"-;\n"
" ||:.  |: : _.---``````---/    '.   _.`.\n"
" ||:.  | .-'  _,'```'-...'   _ .-'.'    '-.\n"
" ||:. .-'   .'        '. . '      '.      `'.\n"
" ||: ;.' .`'        _. '`'-.         '.   . ''-._\n"
" ||:. :   '.     .'          '.  . ' ' '.`       '._\n"
" ||:. :    '. .'     .::\"\"-: .''.        ' .   . ' ' :::\"\"-.\n"
" ||:. '     ..' .    /::     \\    '.        . '.    /::     \\\n"
" ||:  :  . .'      '.|::     |    _.:---\"\"---.._'   |::     |\n"
" ||.  ;  .:          '\\:.__ /   .'              '.   \\:.__ /\n"
" ||:  ;  : '.       . ||____|_.'                  '._||____|\n"
" ||:  ;__:   '.   .'  ||:.  |                        ||:.  |\n"
" ||:___| \\     '. :   ||:.  |                        ||:.  |\n"
" [[____]  '.     '.-._||:.  |                        ||:.  |\n"
"            '.    :   ||:.  |                        ||:.  |\n"
"              '.  :   ||:.  |                        ||:.  |\n"
"                '-:   ||:.  |                        ||:.  |\n"
"                   '._||:.  |________________________||:.  |\n"
"                      ||:___|'-.-'-.-'-.-'-.-'-.-'-.-||:___|\n"
"                      [[____]                        [[____]\n";


void failed_time() {

	puts(CYN "\n\nYou tried to run as fast as you can...");
	puts("But you still ended up being late for class :(");
	printf(RED "%s" RESET, late);
	exit(0);

}

void failed_book() {
	printf(CYN"\n\nYou reached class in time! Your teacher asked you to take out your math workbook.\n");
	printf("You dig into your bag");
	for (int i = 0; i < 3 ; i++) {
		sleep(1);
		putchar('.');
	}
	printf(RED"\n%s"CYN, book);
	printf("You brought the wrong book \"Hacking 101\" to class and your teacher kicked you out!\n");
	exit(0);
}

void __attribute((noreturn)) class(int book) {

	char* flag = calloc(0x50, sizeof(char));
	FILE *f = fopen("flag.txt", "r");

	if (book != 0x13371337) {
		failed_book();
	}

	fread(flag, 0x50, sizeof(char), f);
	fclose(f);
	printf(CYN"You took all the right shortcuts to school, and even brought the correct book to class!\n");
	printf("For that, your teacher gave you a flag, "RED"%s"RESET".\n", flag);
	
	exit(0);

}

void run() {

	char run[0x200];

	printf(MAG "%s\n"RESET, bed);
	printf(YEL "The current time is " RED "08:50AM" YEL ", class starts at " RED "09:00AM" RESET ".\n");
	printf(YEL "You have " RED "5 MINUTES" YEL " to get to school and you cannot afford to be " RED "late" RESET ".\n");
	printf(YEL "Quick! Spam some " RED "A" RED " to make it to school in time!\n");
	printf(GRN "\n\nRun! > " RESET);
	scanf("%s", run);
}

void setup() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	signal(SIGSEGV, failed_time);
	__asm__("push %rdi;pop %rdi");
	return;
}

int main() {

	setup();
	run();
	failed_time();

}
