#include <linux/module.h>
#include <linux/panic.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/random.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elma");
MODULE_DESCRIPTION("kRPG - An RPG game server, built right into your kernel, with the latest and most secure protections and anti-cheat!!");
#define DEVICE_NAME "kRPG"

static DEFINE_MUTEX(PLAYER_MUTEX);
static DEFINE_MUTEX(INVENTORY_MUTEX);
static DEFINE_MUTEX(MOB_MUTEX);
static DEFINE_MUTEX(ETC_MUTEX);

/*
 * art
 *
 */

char YOU_DIED[] =
        "`YMM'   `MM' .g8\"\"8q. `7MMF'   `7MF'   `7MM\"\"\"Yb. `7MMF'`7MM\"\"\"YMM  `7MM\"\"\"Yb.\n"
        "  VMA   ,V .dP'    `YM. MM       M       MM    `Yb. MM    MM    `7    MM    `Yb. \n"
        "   VMA ,V  dM'      `MM MM       M       MM     `Mb MM    MM   d      MM     `Mb \n"
        "    VMMP   MM        MM MM       M       MM      MM MM    MMmmMM      MM      MM \n"
        "     MM    MM.      ,MP MM       M       MM     ,MP MM    MM   Y  ,   MM     ,MP \n"
        "     MM    `Mb.    ,dP' YM.     ,M       MM    ,dP' MM    MM     ,M   MM    ,dP' \n"
        "   .JMML.    `\"bmmd\"'    `bmmmmd\"'     .JMMmmmdP' .JMML..JMMmmmmMMM .JMMmmmdP' \n";

/*
 *
 * helper functions
 *
 */

static void copy_to(void* __user dest, const void* src, unsigned long size) {
	if (copy_to_user(dest, src, size)) {
		panic("\n[!] copy to user failed!");
	}
	return;
}

static void copy_from(void* dest, void* __user src, unsigned long size) {
	if (copy_from_user(dest, src, size)) {
		panic("\n[!] copy from user failed!");
	}
	return;
}

/*
 *
 * Nouns
 *
 */


typedef struct {
	char			name[0x20];
	unsigned long	size;
} feedback_header_t;

typedef struct {
	char 			name[0x18];
	unsigned long 	attack;
} weapon_t;

typedef struct {
	unsigned long	health;
	weapon_t* 		equipped;
	unsigned long 	gold;
} player_t;

typedef struct {
	char 			name[0x20];
	unsigned long 	health;
	unsigned long 	max_health;
	unsigned long 	attack;
} mob_t;

/*
 *
 * Global Variables
 *
 */

player_t player = {
	.health 	= 10,
	.equipped 	= NULL,
	.gold 		= 0,
};

mob_t slime = {
	.name		= "SLIME\0",
	.health		= 5,
	.max_health	= 5,
	.attack		= 1,
};

mob_t wolf = {
	.name		= "WOLF\0",
	.health		= 30,
	.max_health	= 30,
	.attack		= 3,
};

mob_t dragon = {
	.name		= "DRAGON\0",
	.health		= 100,
	.max_health	= 100,
	.attack		= 50,
};

mob_t* mobs[3];
feedback_header_t* feedback;
uint8_t cur_mob;
atomic_t clients_connected;
atomic_t in_battle;
atomic_t dragon_killed;

/*
 *
 * Items, Inventory System
 *
 */

LIST_HEAD(inventory);

// ITEM TYPES
#define WEAPON 1
#define CONSUM 2

// UUIDS
#define RUSTY_SWORD 0x1337
#define POTION 0x1338

typedef struct {
	uint16_t		UUID;
	uint8_t			itemType;
	uint8_t			refCount;
} inventoryHeader_t ;

typedef struct {
	struct list_head	next;
	inventoryHeader_t*	header;
	void*				item;
} inventoryEntry_t ;


typedef struct {
	char 			name[0x20];
	unsigned long	heal;
} potion_t;

inventoryEntry_t* populate_item(uint16_t UUID) {

	inventoryHeader_t* header = kzalloc(sizeof(inventoryHeader_t), GFP_ATOMIC);
	inventoryEntry_t* entry = kzalloc(sizeof(inventoryEntry_t), GFP_ATOMIC);
	entry->header = header;
	header->UUID = UUID;
	header->refCount = 1;

	switch (UUID) {
		case RUSTY_SWORD:
			weapon_t* rustySword = kzalloc(sizeof(weapon_t), GFP_ATOMIC);
			entry->item = rustySword;
			rustySword->attack = 1;
			header->itemType = WEAPON;
			strcpy((char*)&(rustySword->name), "Rusty Sword");
			break;
		case POTION:
			potion_t* potion = kzalloc(sizeof(potion_t), GFP_ATOMIC);
			entry->item = potion;
			potion->heal = 1;
			header->itemType = CONSUM;
			strcpy((char*)&(potion->name), "Health Potion");
			break;
		default:
			panic("Error has occurred! Exiting!");
	}

	return entry;
}

void init_garbage_collection(void) {
	int done;
	inventoryEntry_t* item;
	struct list_head* ptr;

	while (1) {
		done = 1;
		ptr = NULL;
		list_for_each (ptr, &inventory) {
			item = list_entry(ptr, inventoryEntry_t, next);
			if (item->header->refCount <= 0 ) {
				kfree(item->item);
				list_del(ptr);
				done = 0;
				break;
			}
		}
		if (done) {
			break;
		}
	}
}

int add_item(uint16_t UUID) {
	inventoryEntry_t* item;
	struct list_head* ptr;
	mutex_lock(&INVENTORY_MUTEX);
	list_for_each (ptr, &inventory) {
		item = list_entry(ptr, inventoryEntry_t, next);
		if (item->header->UUID == UUID) {
			if (item->header->refCount < 255) {
				printk("[*] increment refcount for uuid %d!\n", UUID);
				item->header->refCount += 1;
				mutex_unlock(&INVENTORY_MUTEX);
				return 0;
			}
			mutex_unlock(&INVENTORY_MUTEX);
			return 2;
		}
	}

	item = populate_item(UUID);
	list_add(&(item->next), &inventory);
	mutex_unlock(&INVENTORY_MUTEX);
	return 0;
}

int visit_shop(unsigned int cmd) {
	switch (cmd) {
		case 1:
			mutex_lock(&PLAYER_MUTEX);
			if (player.gold < 1) {
				mutex_unlock(&PLAYER_MUTEX);
				return 1;
			}
			player.gold -= 1;
			mutex_unlock(&PLAYER_MUTEX);
			return add_item(RUSTY_SWORD);
		case 2:
			mutex_lock(&PLAYER_MUTEX);
			if (player.gold < 5) {
				mutex_unlock(&PLAYER_MUTEX);
				return 1;
			}
			player.gold -= 5;
			mutex_unlock(&PLAYER_MUTEX);
			return add_item(POTION);
		default:
			return 0;
	}
	return 0;
}

typedef struct {
	int16_t UUID;
	char name[0x20];
	uint8_t refCount;
} user_copy_of_items;

int list_inventory_items(void* __user inv) {
	int i = 0;
	user_copy_of_items* user_items;
	inventoryEntry_t* item;
	unsigned int item_count = 0;
	struct list_head* ptr;
	mutex_lock(&INVENTORY_MUTEX);
	list_for_each (ptr, &inventory) {
		item_count += 1;
	}
	if (!inv) {
		mutex_unlock(&INVENTORY_MUTEX);
		return item_count;
	}

	if (item_count == 0)
		mutex_unlock(&INVENTORY_MUTEX);
		return 1;

	user_items = kzalloc(sizeof(user_copy_of_items)*item_count, GFP_ATOMIC);
	list_for_each (ptr, &inventory) {
		item = list_entry(ptr, inventoryEntry_t, next);
		strcpy((char*)&(user_items[i].name), (char*)(item->item));
		user_items[i].UUID = item->header->UUID;
		user_items[i].refCount = item->header->refCount;
		i += 1;
	}
	copy_to((void*)inv, (void*)user_items, sizeof(user_copy_of_items)*item_count);
	kfree(user_items);
	mutex_unlock(&INVENTORY_MUTEX);
	return 0;
}

int mine_gold(void) {

	mutex_lock(&PLAYER_MUTEX);
	player.gold += 5;
	mutex_unlock(&PLAYER_MUTEX);
	return 5;
}

int get_battle_status(void) {
	int stat = atomic_read(&in_battle);
	return stat;
}

void set_battle_status(int status) {
	atomic_set(&in_battle, status);
}

/*
 *
 * i/o functions
 *
 */

#define QUERY_PLAYER_STATS 1
#define QUERY_WEAPON_STATS 13
#define QUERY_MOB_STATS 3
#define VISIT_SHOP 4
#define LIST_INVENTORY 5
#define MINE_GOLD 6
#define QUERY_BATTLE_STATUS 7
#define START_BATTLE 8
#define ATTACK 9
#define HEAL 10
#define RUN 11
#define USE_ITEM 12
#define REMOVE_ITEM 15
#define CREEPER_EXPLODED 14
#define FEEDBACK 16
#define RESET 17

static inline void died(void) {
	panic("%s\n\n%s%s%s\n\n", "\e[1;1H\e[2J", "\e[0;31m", YOU_DIED, "\e[0m");
}

static int attack_boss(void) {
	mutex_lock(&ETC_MUTEX);
	mutex_lock(&PLAYER_MUTEX);
	mutex_lock(&MOB_MUTEX);
	if (player.equipped != NULL) {
		mobs[cur_mob]->health -= player.equipped->attack;
		if (player.equipped->attack >= mobs[cur_mob]->health) {
			cur_mob += 1;
			mutex_unlock(&PLAYER_MUTEX);
			mutex_unlock(&MOB_MUTEX);
			mutex_unlock(&ETC_MUTEX);
			if (cur_mob == 3) {
				return 1337;
			}
			return 0;
		}
	}

	if (mobs[cur_mob]->attack >= player.health)
		died();

	player.health -= mobs[cur_mob]->attack;
	mutex_unlock(&PLAYER_MUTEX);
	mutex_unlock(&ETC_MUTEX);
	mutex_unlock(&MOB_MUTEX);
	return 0;
}

static int use_health_potion(void) {
	inventoryEntry_t* item;
	struct list_head* ptr;
	list_for_each (ptr, &inventory) {
		item = list_entry(ptr, inventoryEntry_t, next);
		if (item->header->UUID == POTION) {
			mutex_lock(&INVENTORY_MUTEX);
			mutex_lock(&PLAYER_MUTEX);
			item->header->refCount -= 1;
			init_garbage_collection();
			player.health = 10;
			mutex_unlock(&INVENTORY_MUTEX);
			mutex_unlock(&PLAYER_MUTEX);
			return player.health;
		}
	}
	mutex_unlock(&INVENTORY_MUTEX);
	return 0;
}

static int equip_weapon(inventoryEntry_t* item) {
	mutex_lock(&PLAYER_MUTEX);
	if (item->header->refCount > 0)
		player.equipped = item->item;
	mutex_unlock(&PLAYER_MUTEX);
	return 0;
}

static int use_item(int UUID) {
	inventoryEntry_t* item;
	struct list_head* ptr;
	list_for_each (ptr, &inventory) {
		item = list_entry(ptr, inventoryEntry_t, next);
		if (item->header->UUID == UUID) {
			if (item->header->itemType == WEAPON) {
				return equip_weapon(item);
			} else if (UUID == POTION) {	
				if (use_health_potion())
					return 0;
				return 1;
			}
		}
	}
	return 1;
}

static int remove_item(int UUID) {
	inventoryEntry_t* item;
	struct list_head* ptr;
	mutex_lock(&INVENTORY_MUTEX);
	list_for_each (ptr, &inventory) {
		item = list_entry(ptr, inventoryEntry_t, next);
		if (item->header->UUID == UUID) {
			if (item->header->refCount > 1) {
				item->header->refCount -= 1;
				init_garbage_collection();
				mutex_unlock(&INVENTORY_MUTEX);
				return 0;
			}
			break;
		}
	}
	mutex_unlock(&INVENTORY_MUTEX);
	return 1;
}

static int get_feedback(char* __user buf) {
	feedback_header_t tmp;

	mutex_lock(&ETC_MUTEX);
	// player can only feedback after killing the dragon!
	if (cur_mob <= 2 || buf == NULL) {
		return 1;
	}

	atomic_set(&dragon_killed, 1);

	copy_from((void*)&tmp, buf, sizeof(feedback_header_t));

	if (feedback == NULL) {
		feedback = kzalloc(sizeof(feedback_header_t) + tmp.size, GFP_ATOMIC);
		memcpy((void*)feedback, (void*)&tmp, sizeof(feedback_header_t));
	}

	if (tmp.size > feedback->size)
		return 1;

	copy_from((void*)feedback + sizeof(feedback_header_t), buf + sizeof(feedback_header_t), tmp.size);
	mutex_unlock(&ETC_MUTEX);
	return 0;
}

static long rpg_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	int i;

	switch (cmd) {
		case QUERY_PLAYER_STATS:
			copy_to((void*)arg, (void*)&player, sizeof(player_t));
			break;
		case QUERY_WEAPON_STATS:
			if (player.equipped != NULL) {
				copy_to((void*)arg, (void*)player.equipped, sizeof(weapon_t));
				return 0;
			} else {
				return 1;
			}
		case QUERY_MOB_STATS:
			mutex_lock(&MOB_MUTEX);
			mutex_lock(&ETC_MUTEX);
			if (!get_battle_status())
				return 1;
			if (cur_mob < 3) {
				copy_to((void*)arg, (void*)mobs[cur_mob], sizeof(mob_t));
			} else {
				mobs[2]->health = 100;
				copy_to((void*)arg, (void*)mobs[2], sizeof(mob_t));
			}
			mutex_unlock(&ETC_MUTEX);
			mutex_unlock(&MOB_MUTEX);
			break;
		case VISIT_SHOP:
			if (get_battle_status())
				return 1;
			return visit_shop(arg);
		case LIST_INVENTORY:
			return list_inventory_items((void* __user) arg);
		case MINE_GOLD:
			if (!get_battle_status())
				return mine_gold();
			break;
		case QUERY_BATTLE_STATUS:
			return get_battle_status();
		case START_BATTLE:
			if (get_battle_status()) {
				return -1;
			}
			set_battle_status(1);
			return 0;
		case ATTACK:
			if (get_battle_status())
				return attack_boss();
			break;
		case HEAL:
			return use_health_potion();
		case RUN:
			set_battle_status(0);
			break;
		case USE_ITEM:
			return use_item(arg);
			break;
		case REMOVE_ITEM:
			return remove_item(arg);
		case CREEPER_EXPLODED:
			died();
			break;
		case FEEDBACK:
			return get_feedback((char* __user)arg);
		case RESET:
			mutex_lock(&ETC_MUTEX);
			mutex_lock(&MOB_MUTEX);
			i = atomic_read(&dragon_killed) ;
			if (i) {
				// only the dragon killer is worthy of revisiting his previous opponents.
				cur_mob -= 1;
				slime.health = 5;
				wolf.health = 30;
				dragon.health = 100;
			}
			mutex_unlock(&ETC_MUTEX);
			mutex_unlock(&MOB_MUTEX);
			return i-1;
		default:
			return -1;
			break;
	}
    return 0;
}

static int rpg_release(struct inode *inode, struct file *filp) {
	atomic_dec(&clients_connected);
	return 0;
}

static int rpg_open(struct inode *inode, struct file *filp) {

	// Ensure that only one client is connected at a time!
	mutex_lock(&ETC_MUTEX);
	if (atomic_read(&clients_connected) > 0) {
		mutex_unlock(&ETC_MUTEX);
		return -ENODEV;
	}

	atomic_inc(&clients_connected);
	mutex_unlock(&ETC_MUTEX);

	return 0;
}

/*
*
* Initialization and Cleanup Code
*
*/

static struct file_operations module_fops = {
    .owner  			= THIS_MODULE,
    .open   			= rpg_open,
    .unlocked_ioctl 	= rpg_ioctl,
	.release 			= rpg_release,
};


static struct miscdevice misc_dev = {
	.minor 		= MISC_DYNAMIC_MINOR,
	.name 		= DEVICE_NAME,
	.fops 		= &module_fops,
};

static int __init krpg_init(void) {

	if (misc_register(&misc_dev)) {
		printk("[!] failed to register krpg device!");
		return -EBUSY;
	}
	printk("[-] krpg device registered!");
	
	cur_mob = 0;
	atomic_set(&in_battle, 0);
	atomic_set(&clients_connected, 0);
	atomic_set(&dragon_killed, 0);
	mobs[0] = &slime;
	mobs[1] = &wolf;
	mobs[2] = &dragon;

    return 0;
}

static void __exit krpg_destroy(void) {
	misc_deregister(&misc_dev);
	printk("[-] krpg device unregistered!");
}

module_init(krpg_init);
module_exit(krpg_destroy);
