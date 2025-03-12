#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

//  gcc  -zrelro -znow -no-pie -fstack-protector-all chall.c
struct profile {
	int age;
	int kidAge;
	char* yourName;
	char* lastName;
	char* kidName;
	char* kidLastName;
	char* bucketList[7];
	char reflection[100];
};

char* lastNames[] = {"Tan", "Lim", "Lee", "Ng", "Ong", "Wong", "Goh", "Chua", "Chan", "Koh"};
char* firstNames[] = {"Sarah", "Ryan", "Nicole", "Nicholas", "Valerie", "Javier", "Samantha", "Roy", "Chloe", "Jason"};
bool kid_adopted = false;
bool reflected = false;
struct profile life;

/*

	 PRINT STUFF

*/

void banner() {
	printf("%s", 
		"\e[0;94;5m"
		" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⠶⠛⠛⠛⠶⣤⡀⠀⠀\n"
		"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⠟⠋⢁⣠⣴⣶⣶⣶⣬⣿⣆⠀\n"
		"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⡾⠟⠉⢀⣠⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄\n"
		"⠀⠀⠀⠀⠀⠀⠀⢀⣠⡴⠟⠋⠁⠀⠀⠺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇\n"
		"⠀⠀⠀⠀⢀⣴⠾⠛⠉⠀⠀⠀⠀⠀⠀⠀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀          Congratulations!\n"
		"⠀⠀⢀⡾⠋⠁⠀re:life⠀⠀⠀⢈⣿⣿⣿⣿⣿⣿⣿⠿⠋⠀⠀          You've been given a second chance at life.\n"
		"⠀⢀⣿⠁⠀by elmo⠀⠀⣀⣤⣾⠿⢿⣿⣿⡿⠟⠋⠀⠀⠀⠀⠀          Use it well!\n"
		"⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⡾⠛⢉⣠⣴⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀\n"
		"⠀⠀⢿⡄⠐⢦⣤⣤⣴⣾⠿⠛⣁⣤⡾⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
		"⠀⠀⠀⠻⢦⣄⣀⠉⣉⣀⣴⠾⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
		"⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\e[0m");
}

void print_menu() {
	printf("%s",
		"\n\e[0;33m"
		"1. Review Life\n"
		"2. Fast Forward Time\n"
		"3. Add todo to bucket list\n"
		"4. Tick off bucket list entry\n"
		"5. Adopt a Kid\n"
		"6. Disown a Kid\n"
		"7. Reflect upon your life\n"
		"8. Next life\n\n"
		"\e[0mAction: "
	);
}

void adopt_a_kid() {
	if (kid_adopted) {
		puts("There's no more kid available for adoption, please come back again in your next life.");
		return;
	}
	puts("You found a lovely kid in the orphanage!");
	puts("He takes after you.");
	life.kidName = calloc(0x20, sizeof(char));
	life.kidLastName = life.lastName;
	life.kidAge = rand() % 100;
	strcpy(life.kidName, firstNames[rand() % 10]);
	printf("Your kid is called %s %s and he is %d years old\nTake care of him well :)\n", life.kidName, life.kidLastName, life.kidAge);
	kid_adopted = true;
}

void disown_a_kid() {
	if (kid_adopted && life.kidName) {
		printf("%s %s performed poorly in the exams, and you kicked them out of the house. You don't deserve to adopt anymore kids...\n", life.kidName, life.kidLastName);
		free(life.kidLastName);
		free(life.kidName);
		life.kidName = NULL;
		life.kidLastName = NULL;
	}
	puts("You don't have a kid to disown...");
}

void add_to_bucket_list() {
	for (int i = 0; i < 5; i++) {
		if (life.bucketList[i] == NULL) {
			char* res = malloc(0x100);
			memset(res, 0, 0x100);
			printf("What would you like to add to your bucket list?\n> ");
			read(0, res, 0xff);
			life.bucketList[i] = res;
			// life.bucketList[i][strcspn(life.bucketList[i], "\n")] = 0;
			return;
		}
	}
}

void tick_off_bucket_list() {
	unsigned int idx;

	printf("Which bucket list item have you completed?\n> ");
	scanf("%d", &idx);
	getchar();

	if ((idx < 5) && (life.bucketList[idx])) {
		free(life.bucketList[idx]);
		life.bucketList[idx] = NULL;
		printf("Congratulations on your accomplishment :)");
	} else {
		puts("Are you dreaming?");
	}
	return;
}

void time_skip() {
	unsigned int add = 0;
	printf("How many years would you like to fast forward?\n> ");
	scanf("%u", &add);
	life.age += add;

	if (life.age >= 100) {
		puts("You died of old age");
		exit(-1);
	}
}

void review_life() {
	printf(
		"Name: %s %s\n"
		"Age: %d\n"
	, life.yourName, life.lastName, life.age);

	if (life.kidName) {
		printf("Kid: %s %s\n", life.kidName, life.kidLastName);
		printf("Kid Age: %d\n", life.kidAge);
	}

	puts("\e[0;4;1mBucket List\e[0m");
	for (int i = 0; i < 7; i++) {
		if (life.bucketList[i]) {
			printf("%d. %s\n", i, life.bucketList[i]);
		}
	}
}

void tips_for_next_life() {
	if (reflected) {
		puts("You have reflected once this life. GG go next");
		return;
	}
	reflected = true;
	puts("In RE:LIFE, we encourage you to constantly reflect upon your life actions, so that you can improve your life each time!");
	puts("With age comes wisdom, what is your reflection?");

	if (life.age == 0) {
		puts("But you are merely a new-born, come back after you lived for longer.");
		return;
	}

	int sz = read(0, life.reflection, life.age-1);
	puts("Sounds good. This will be reflected upon in your next life.");
}

void setup() {
	srand(time(0));
	memset(&life, 0, sizeof(life));
	life.yourName = calloc(0x20, sizeof(char));
	life.lastName = calloc(0x20, sizeof(char));
	strcpy(life.yourName, firstNames[rand() % 10]);
	strcpy(life.lastName, lastNames[rand() % 10]);
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
}

void go_next() {
	char* args[2] = {NULL, NULL};
	args[0] = malloc(0x1000);
	memset(args[0] , 0, 0x1000);
	readlink("/proc/self/exe", args[0], 0x1000);
	execv(args[0], args);
}

int main() {

	int opt;

	setup();
	banner();

	while (1) {
		print_menu();
		scanf("%d", &opt);
		getchar();
		putchar(0xa);

		switch (opt) {
			case 1:
				review_life();
				break;
			case 2:
				time_skip();
				break;
			case 3:
				add_to_bucket_list();
				break;
			case 4:
				tick_off_bucket_list();
				break;
			case 5:
				adopt_a_kid();
				break;
			case 6:
				disown_a_kid();
				break;
			case 7:
				tips_for_next_life();
				break;
			case 8:
				go_next();
			default:
				break;
		}
	}
}
