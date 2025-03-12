#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void win() {
	
	char flag[100] = {0};
	FILE *f = fopen("flag.txt", "r");
	fread(flag, 1, 100, f);
	fclose(f);

	printf("\nThe boss left you a message as it dies: %s\n", flag);
	exit(0);
}

void die() {
	puts("\nWhile you tried to make your final escape, the boss stabbed you in the back.");
	puts("YOU DIED.");
	exit(0);
}

void boss_fight() {
	int opt;
	short int boss_hp = 10000;
	short int your_hp = 100;

	while (1) {
		printf("\n\n\n");
		printf("----------- Boss Fight -------------\n");
		printf(" Boss HP: %hd/10000\n", boss_hp);
		printf(" Player HP: %hd/100\n", your_hp);
		printf("------------------------------------\n");
		printf("1. Attack (1 damage)\n");
		printf("2. Defend\n");
		printf("3. Escape\n> ");

		scanf("%d", &opt);

		switch (opt) {
			case 1:
				boss_hp -= 1;
				your_hp -= 10;
				printf("You deal 1 damage to the boss.\nThe boss deals 10 damage to you.\nPress Enter to continue.\n");
				getchar();
				break;
			case 2:
				printf("You defended against the boss's attack.\nIn the meantime, the boss healed 1000 hp.\n");
				boss_hp += 1000;
				printf("The boss now has %hd hp!\n", boss_hp);
				getchar();
				break;
			default:
				die();
				break;
		}
		
		if (boss_hp <= 0) {
			win();
		} else if (your_hp<= 0) {
			die();
		}
	}
}

void setup() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
}

int main() {
	setup();
	boss_fight();
}
