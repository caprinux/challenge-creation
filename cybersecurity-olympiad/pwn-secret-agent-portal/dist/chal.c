#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/random.h>
#include <termios.h>

#define MAX_NOTES 0x5

// ANSI Color codes
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"

// Text colors
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Bright colors
#define BRIGHT_BLACK "\033[90m"
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_WHITE "\033[97m"

// Background colors
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

struct note {
	char *content;
	size_t size;
	int active;
};

// Global variables
struct note notes[MAX_NOTES];

// Clear screen function
void clear_screen() {
	printf("\033[2J\033[H");
}

// Print banner
void print_banner() {
	printf(BRIGHT_CYAN BOLD);
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║" BRIGHT_YELLOW "    ____  _____ ____ ____  _____ _____       _    ____ _____ _   _ _____      " BRIGHT_CYAN "║\n");
	printf("║" BRIGHT_YELLOW "   / ___|| ____/ ___|  _ \\| ____|_   _|     / \\  / ___| ____| \\ | |_   _|     " BRIGHT_CYAN "║\n");
	printf("║" BRIGHT_YELLOW "   \\___ \\|  _|| |   | |_) |  _|   | |      / _ \\| |  _|  _| |  \\| | | |       " BRIGHT_CYAN "║\n");
	printf("║" BRIGHT_YELLOW "    ___) | |__| |___|  _ <| |___  | |     / ___ \\ |_| | |___| |\\  | | |       " BRIGHT_CYAN "║\n");
	printf("║" BRIGHT_YELLOW "   |____/|_____\\____|_| \\_\\_____| |_|    /_/   \\_\\____|_____|_| \\_| |_|       " BRIGHT_CYAN "║\n");
	printf("║                                                                              ║\n");
	printf("║" BRIGHT_WHITE "                           🕵️  CLASSIFIED SYSTEM 🕵️                           " BRIGHT_CYAN "║\n");
	printf("╚══════════════════════════════════════════════════════════════════════════════╝" RESET "\n");
}

// Print access granted animation
void print_access_granted() {
	printf(BRIGHT_GREEN BOLD);
	printf("\n");
	printf("     █████╗  ██████╗ ██████╗███████╗███████╗███████╗    ██████╗ ██████╗  █████╗ ███╗   ██╗████████╗███████╗██████╗ \n");
	printf("    ██╔══██╗██╔════╝██╔════╝██╔════╝██╔════╝██╔════╝   ██╔════╝ ██╔══██╗██╔══██╗████╗  ██║╚══██╔══╝██╔════╝██╔══██╗\n");
	printf("    ███████║██║     ██║     █████╗  ███████╗███████╗   ██║  ███╗██████╔╝███████║██╔██╗ ██║   ██║   █████╗  ██║  ██║\n");
	printf("    ██╔══██║██║     ██║     ██╔══╝  ╚════██║╚════██║   ██║   ██║██╔══██╗██╔══██║██║╚██╗██║   ██║   ██╔══╝  ██║  ██║\n");
	printf("    ██║  ██║╚██████╗╚██████╗███████╗███████║███████║   ╚██████╔╝██║  ██║██║  ██║██║ ╚████║   ██║   ███████╗██████╔╝\n");
	printf("    ╚═╝  ╚═╝ ╚═════╝ ╚═════╝╚══════╝╚══════╝╚══════╝    ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝╚═════╝ \n");
	printf(RESET "\n");
	
	for (int i = 0; i < 3; i++) {
		printf(BRIGHT_GREEN "█" RESET);
		fflush(stdout);
	}
	printf(" " BRIGHT_WHITE "SYSTEM INITIALIZED" RESET "\n\n");
}

// Print access denied
void print_access_denied() {
	printf(BRIGHT_RED BOLD);
	printf("\n");
	printf("    ██████╗ ███████╗███╗   ██╗██╗███████╗██████╗ \n");
	printf("    ██╔══██╗██╔════╝████╗  ██║██║██╔════╝██╔══██╗\n");
	printf("    ██║  ██║█████╗  ██╔██╗ ██║██║█████╗  ██║  ██║\n");
	printf("    ██║  ██║██╔══╝  ██║╚██╗██║██║██╔══╝  ██║  ██║\n");
	printf("    ██████╔╝███████╗██║ ╚████║██║███████╗██████╔╝\n");
	printf("    ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚═╝╚══════╝╚═════╝ \n");
	printf("           ⚠️  UNAUTHORIZED ACCESS ATTEMPT ⚠️\n" RESET);
}

// Function to generate random password
void generate_password(char *password_buffer) {
	unsigned char random_bytes[16];
	
	if (getrandom(random_bytes, 16, 0) == -1) {
		perror("getrandom failed");
		exit(1);
	}
	
	// Convert to hex string
	for (int i = 0; i < 16; i++) {
		sprintf(&password_buffer[i * 2], "%02x", random_bytes[i]);
	}
	password_buffer[32] = '\0';
}

// Function to hide password input
void get_password(char *password, size_t max_len) {
	struct termios old_term, new_term;
	
	// Get current terminal settings
	tcgetattr(STDIN_FILENO, &old_term);
	new_term = old_term;
	
	// Disable echo
	new_term.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
	
	printf(BRIGHT_YELLOW "🔐 Password: " RESET);
	fgets(password, max_len, stdin);
	
	// Remove newline if present
	password[strcspn(password, "\n")] = '\0';
	
	// Restore terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
	printf("\n");
}

// Authentication function
int authenticate() {
	char username[33];
	char password[33];
	char correct_password[33];
	
	clear_screen();
	print_banner();
	
	// Generate password on stack
	generate_password(correct_password);
	
	printf(BRIGHT_WHITE "\n");
	printf("╔════════════════════════════════════════╗\n");
	printf("║         " BRIGHT_CYAN "AUTHENTICATION REQUIRED" BRIGHT_WHITE "        ║\n");
	printf("╚════════════════════════════════════════╝\n" RESET);
	
	printf(BRIGHT_YELLOW "👤 Username: " RESET);
	fgets(username, 32, stdin);
	username[strcspn(username, "\n")] = '\0';
	
	get_password(password, 0x32);
	
	printf(BRIGHT_BLUE "🔄 Verifying credentials" RESET);
	for (int i = 0; i < 3; i++) {
		printf(".");
		fflush(stdout);
	}
	printf("\n");
	
	if ((strcmp(password, correct_password) == 0) && (strcmp(username, "agent") == 0)) {
		print_access_granted();
		printf(BRIGHT_GREEN "Welcome, Agent %s! 🕵️\n" RESET, username);
		return 1;
	} else {
		print_access_denied();
		return 0;
	}
}

// Initialize notes array
void init_notes() {
	for (int i = 0; i < MAX_NOTES; i++) {
		notes[i].content = NULL;
		notes[i].size = 0;
		notes[i].active = 0;
	}
}

// Create note function
void create_note() {
	int slot = -1;
	size_t note_size;
	
	printf(BRIGHT_CYAN "\n📝 " BOLD "CREATING NEW CLASSIFIED NOTE" RESET "\n");
	printf(BRIGHT_WHITE "═══════════════════════════════════════\n" RESET);
	
	// Find empty slot
	for (int i = 0; i < MAX_NOTES; i++) {
		if (notes[i].content == NULL) {
			slot = i;
			break;
		}
	}
	
	if (slot == -1) {
		printf(BRIGHT_RED "❌ Error: Maximum number of notes reached!\n" RESET);
		return;
	}
	
	printf(BRIGHT_YELLOW "📏 Enter note size: " RESET);
	if (scanf("%zu", &note_size) != 1 || note_size == 0) {
		printf(BRIGHT_RED "❌ Invalid size!\n" RESET);
		while (getchar() != '\n'); // clear input buffer
		return;
	}
	
	// Clear input buffer
	while (getchar() != '\n');
	
	// Allocate memory for note
	notes[slot].content = malloc(note_size + 1); // +1 for null terminator
	if (!notes[slot].content) {
		printf(BRIGHT_RED "❌ Memory allocation failed!\n" RESET);
		return;
	}
	
	notes[slot].size = note_size;
	notes[slot].active = 1;
	
	printf(BRIGHT_YELLOW "✏️  Enter note content: " RESET);
	fgets(notes[slot].content, note_size + 1, stdin);
	
	// Remove newline if present and within size limit
	notes[slot].content[strcspn(notes[slot].content, "\n")] = '\0';
	
	printf(BRIGHT_GREEN "✅ Note created in slot " BOLD "%d" RESET BRIGHT_GREEN " successfully!\n" RESET, slot);
}

// Edit note function
void edit_note() {
	int slot;
	
	printf(BRIGHT_CYAN "\n✏️  " BOLD "EDITING CLASSIFIED NOTE" RESET "\n");
	printf(BRIGHT_WHITE "═══════════════════════════════════════\n" RESET);
	
	printf(BRIGHT_YELLOW "🎯 Enter note slot (0-%d): " RESET, MAX_NOTES - 1);
	if (scanf("%d", &slot) != 1 || slot < 0 || slot >= MAX_NOTES) {
		printf(BRIGHT_RED "❌ Invalid slot!\n" RESET);
		while (getchar() != '\n');
		return;
	}
	
	if (notes[slot].content == NULL) {
		printf(BRIGHT_RED "❌ No note in slot %d\n" RESET, slot);
		while (getchar() != '\n');
		return;
	}
	
	// Clear input buffer
	while (getchar() != '\n');
	
	printf(BRIGHT_BLUE "📄 Current content: " RESET "%s\n", notes[slot].content);
	printf(BRIGHT_YELLOW "✏️  Enter new content: " RESET);
	fgets(notes[slot].content, notes[slot].size + 1, stdin);
	
	// Remove newline if present
	notes[slot].content[strcspn(notes[slot].content, "\n")] = '\0';
	
	printf(BRIGHT_GREEN "✅ Note updated successfully!\n" RESET);
}

// Read note function
void read_note() {
	int slot;
	
	printf(BRIGHT_CYAN "\n📖 " BOLD "READING CLASSIFIED NOTE" RESET "\n");
	printf(BRIGHT_WHITE "═══════════════════════════════════════\n" RESET);
	
	printf(BRIGHT_YELLOW "🎯 Enter note slot (0-%d): " RESET, MAX_NOTES - 1);
	if (scanf("%d", &slot) != 1 || slot < 0 || slot >= MAX_NOTES) {
		printf(BRIGHT_RED "❌ Invalid slot!\n" RESET);
		while (getchar() != '\n');
		return;
	}
	
	if (notes[slot].content == NULL) {
		printf(BRIGHT_RED "❌ No note in slot %d\n" RESET, slot);
	} else {
		printf(BRIGHT_WHITE BOLD "NOTE %d" RESET BRIGHT_GREEN " │ " BRIGHT_YELLOW "SIZE: %zu bytes\n" BRIGHT_GREEN, slot, notes[slot].size);
		printf("═══════════════════════════════════════════════════════════════════════════════\n");
		printf(BRIGHT_WHITE "%s\n" BRIGHT_GREEN, notes[slot].content);
	}
	
	// Clear input buffer
	while (getchar() != '\n');
}

// Free note function
void free_note() {
	int slot;
	
	printf(BRIGHT_RED "\n🗑️  " BOLD "DELETING CLASSIFIED NOTE" RESET "\n");
	printf(BRIGHT_WHITE "═══════════════════════════════════════\n" RESET);
	
	printf(BRIGHT_YELLOW "🎯 Enter note slot to delete (0-%d): " RESET, MAX_NOTES - 1);
	if (scanf("%d", &slot) != 1 || slot < 0 || slot >= MAX_NOTES) {
		printf(BRIGHT_RED "❌ Invalid slot!\n" RESET);
		while (getchar() != '\n');
		return;
	}
	
	if (notes[slot].content == NULL) {
		printf(BRIGHT_RED "❌ No note in slot %d to delete\n" RESET, slot);
	} else {
		free(notes[slot].content);
		notes[slot].active = 0;
		printf(BRIGHT_GREEN "✅ Note %d deleted successfully! 🔥\n" RESET, slot);
	}
	
	// Clear input buffer
	while (getchar() != '\n');
}

// Display menu
void display_menu() {
	printf(BRIGHT_MAGENTA "\n");
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║                            " BRIGHT_CYAN BOLD "🕵️  SECRET AGENT MENU  🕵️" RESET BRIGHT_MAGENTA "                         ║\n");
	printf("╠══════════════════════════════════════════════════════════════════════════════╣\n");
	printf("║  " BRIGHT_YELLOW "1." BRIGHT_WHITE " 📝 Create classified note                                                " BRIGHT_MAGENTA "║\n");
	printf("║  " BRIGHT_YELLOW "2." BRIGHT_WHITE " ✏️  Edit classified note                                                 " BRIGHT_MAGENTA "║\n");
	printf("║  " BRIGHT_YELLOW "3." BRIGHT_WHITE " 📖 Read classified note                                                  " BRIGHT_MAGENTA "║\n");
	printf("║  " BRIGHT_YELLOW "4." BRIGHT_WHITE " 🗑️  Delete classified note                                               " BRIGHT_MAGENTA "║\n");
	printf("║  " BRIGHT_YELLOW "5." BRIGHT_WHITE " 🚪 Logout and secure system                                              " BRIGHT_MAGENTA "║\n");
	printf("╚══════════════════════════════════════════════════════════════════════════════╝\n" RESET);
	printf(BRIGHT_CYAN "🎯 Enter your choice: " RESET);
}

// Cleanup function
void cleanup() {
	for (int i = 0; i < MAX_NOTES; i++) {
		if (notes[i].active && notes[i].content) {
			free(notes[i].content);
		}
	}
}

// Logout animation
void logout_animation() {
	printf(BRIGHT_YELLOW "\n🔒 Securing classified data" RESET);
	for (int i = 0; i < 5; i++) {
		printf(".");
		fflush(stdout);
	}
	printf("\n");
	
	printf(BRIGHT_GREEN BOLD);
	printf("███╗   ███╗██╗███████╗███████╗██╗ ██████╗ ███╗   ██╗     ██████╗ ██████╗ ███╗   ███╗██████╗ ██╗     ███████╗████████╗███████╗\n");
	printf("████╗ ████║██║██╔════╝██╔════╝██║██╔═══██╗████╗  ██║    ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██║     ██╔════╝╚══██╔══╝██╔════╝\n");
	printf("██╔████╔██║██║███████╗███████╗██║██║   ██║██╔██╗ ██║    ██║     ██║   ██║██╔████╔██║██████╔╝██║     █████╗     ██║   █████╗  \n");
	printf("██║╚██╔╝██║██║╚════██║╚════██║██║██║   ██║██║╚██╗██║    ██║     ██║   ██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝     ██║   ██╔══╝  \n");
	printf("██║ ╚═╝ ██║██║███████║███████║██║╚██████╔╝██║ ╚████║    ╚██████╗╚██████╔╝██║ ╚═╝ ██║██║     ███████╗███████╗   ██║   ███████╗\n");
	printf("╚═╝     ╚═╝╚═╝╚══════╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝     ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚══════╝╚══════╝   ╚═╝   ╚══════╝\n");
	printf(RESET "\n");
	printf(BRIGHT_WHITE "🎖️  All classified data has been secured. Thank you for your service, Agent.\n" RESET);
}

int main() {
	int choice;
	
	// Initialize
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	init_notes();
	
	// Authentication
	if (!authenticate()) {
		return 1;
	}
	
	// Main menu loop
	while (1) {
		display_menu();
		
		if (scanf("%d", &choice) != 1) {
			printf(BRIGHT_RED "❌ Invalid input!\n" RESET);
			while (getchar() != '\n'); // clear input buffer
			continue;
		}
		
		switch (choice) {
			case 1:
				create_note();
				break;
			case 2:
				edit_note();
				break;
			case 3:
				read_note();
				break;
			case 4:
				free_note();
				break;
			case 5:
				logout_animation();
				cleanup();
				return 0;
			default:
				printf(BRIGHT_RED "❌ Invalid choice! Please select 1-5.\n" RESET);
		}
	}
	
	return 0;
}
