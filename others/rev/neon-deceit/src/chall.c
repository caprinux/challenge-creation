#define _GNU_SOURCE // For asprintf and realpath
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>    // For seeding the random qsort and timing checks
#include <setjmp.h>  // For non-local jumps

// --- Anti-Debugging & CTF Includes ---
#include <sys/ptrace.h> // For ptrace
#include <errno.h>      // For errno
#include <ncurses.h>
#include <openssl/sha.h>

// This is a placeholder for any custom headers you might use.
// If you don't have this file, you can comment out the line.
// #include "funny_populate_got.h"


// --- Maze Configuration (Hidden from user) ---
#define MAZE_WIDTH 51
#define MAZE_HEIGHT 21
#define WALL '#'
#define PATH ' '

// --- Global jump buffer for non-local error handling ---
static jmp_buf validation_failed_jmp;
// --- Global variable for timing check ---
static uint64_t timing_check_start;


// +++ ANTI-DEBUGGING AND OBFUSCATION SECTION +++

// A simple high-resolution timer using x86's rdtsc instruction.
static inline uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

// --- String Encryption ---
#define XOR_KEY 0x5A
typedef struct { unsigned char* data; size_t len; } EncryptedString;
static void __attribute__((always_inline))decrypt_string(EncryptedString enc_str, char* out_buf) {
    for (size_t i = 0; i < enc_str.len; i++) { out_buf[i] = enc_str.data[i] ^ XOR_KEY; }
    out_buf[enc_str.len] = '\0';
}

static unsigned char enc_gdb[]     = { 0x3d, 0x3e, 0x38 }; // "gdb"
static unsigned char enc_lldb[]    = { 0x36, 0x36, 0x3e, 0x38 }; // "lldb"
static unsigned char enc_ida[]     = { 0x33, 0x3e, 0x3b }; // "ida"
static unsigned char enc_strace[]  = { 0x29, 0x2e, 0x28, 0x3b, 0x39, 0x3f }; // "strace"
static unsigned char enc_ltrace[]  = { 0x36, 0x2e, 0x28, 0x3b, 0x39, 0x3f }; // "ltrace"
static unsigned char enc_radare2[] = { 0x28, 0x3b, 0x3e, 0x3b, 0x28, 0x3f, 0x68 }; // "radare2"
static unsigned char enc_r2[]      = { 0x28, 0x68 }; // "r2"

void __attribute__((always_inline))check_ptrace() { if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) longjmp(validation_failed_jmp, 2); }

void __attribute__((always_inline))check_parent_debugger() {
    char path[PATH_MAX]; char comm[1024] = {0}; pid_t ppid = getppid();
    snprintf(path, sizeof(path), "/proc/%d/comm", ppid);
    FILE* f = fopen(path, "r");
    if (!f) return;
    fread(comm, 1, sizeof(comm) - 1, f); fclose(f);
    EncryptedString targets[] = {{enc_gdb, sizeof(enc_gdb)}, {enc_lldb, sizeof(enc_lldb)}, {enc_ida, sizeof(enc_ida)}, {enc_strace, sizeof(enc_strace)}, {enc_ltrace, sizeof(enc_ltrace)}, {enc_radare2, sizeof(enc_radare2)}, {enc_r2, sizeof(enc_r2)}};
    char decrypted_buf[32];
    for (size_t i = 0; i < sizeof(targets)/sizeof(targets[0]); i++) {
        decrypt_string(targets[i], decrypted_buf);
        if (strstr(comm, decrypted_buf)) longjmp(validation_failed_jmp, 3);
    }
}

void __attribute__((always_inline))timing_probe_start() { timing_check_start = rdtsc(); }
void __attribute__((always_inline))timing_probe_end() {
    uint64_t end = rdtsc(); uint64_t elapsed = end - timing_check_start;
    const uint64_t threshold = 10000000000ULL; // ~10 billion cycles
    if (elapsed > threshold) longjmp(validation_failed_jmp, 4);
}

void __attribute__((always_inline))initialize_security_probes() { check_ptrace(); check_parent_debugger(); }


// --- DATA STRUCTURES ---
typedef struct { int y; int x; } Point;
typedef struct { int y; int x; } Wall; // A wall separates two cells

// --- PATHFINDING & NORMALIZATION DATA STRUCTURES ---
typedef struct PathNode { Point pos; struct PathNode* parent; } PathNode;
typedef struct QueueNode { PathNode* p_node; struct QueueNode* next; } QueueNode;
typedef struct { QueueNode *front, *rear; } Queue;


// --- PATHFINDING & NORMALIZATION HELPERS ---
Queue* create_queue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}
void enqueue(Queue* q, PathNode* p_node) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->p_node = p_node; temp->next = NULL;
    if (q->rear == NULL) { q->front = q->rear = temp; return; }
    q->rear->next = temp; q->rear = temp;
}
PathNode* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    QueueNode* temp = q->front; PathNode* p_node = temp->p_node;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp); return p_node;
}


// --- DISJOINT SET UNION (FOR MAZE GENERATION) ---
int find_set(int i, int* parent) { if (parent[i] == i) return i; return parent[i] = find_set(parent[i], parent); }
void unite_sets(int a, int b, int* parent) { a = find_set(a, parent); b = find_set(b, parent); if (a != b) parent[b] = a; }


// --- MAZE GENERATION (KRUSKAL'S) ---
int random_comparator(const void* a, const void* b) { (void)a; (void)b; return (rand() % 3) - 1; }
void generate_maze_kruskal(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    memset(maze, WALL, MAZE_HEIGHT * MAZE_WIDTH);
    Wall* walls = malloc(sizeof(Wall) * MAZE_WIDTH * MAZE_HEIGHT); int wall_count = 0;
    for (int y = 1; y < MAZE_HEIGHT - 1; y++) {
        for (int x = 1; x < MAZE_WIDTH - 1; x++) {
            maze[y][x] = (x % 2 && y % 2) ? PATH : WALL;
            if (x % 2 == 0 && y % 2 == 1) walls[wall_count++] = (Wall){y, x};
            if (y % 2 == 0 && x % 2 == 1) walls[wall_count++] = (Wall){y, x};
        }
    }
    qsort(walls, wall_count, sizeof(Wall), random_comparator);
    int num_cells = (MAZE_WIDTH / 2) * (MAZE_HEIGHT / 2);
    int* parent_set = malloc(sizeof(int) * num_cells);
    for (int i = 0; i < num_cells; i++) parent_set[i] = i;
    for (int i = 0; i < wall_count; i++) {
        int r = walls[i].y; int c = walls[i].x; int c1, c2;
        if (c % 2 == 0) { c1 = (r/2)*(MAZE_WIDTH/2)+(c-1)/2; c2 = (r/2)*(MAZE_WIDTH/2)+(c+1)/2; }
        else { c1 = ((r-1)/2)*(MAZE_WIDTH/2)+c/2; c2 = ((r+1)/2)*(MAZE_WIDTH/2)+c/2; }
        if (find_set(c1, parent_set) != find_set(c2, parent_set)) {
            unite_sets(c1, c2, parent_set); maze[r][c] = PATH;
        }
    }
    free(walls); free(parent_set);
}


// --- PATH NORMALIZATION (GUARANTEES UNIQUE SOLUTION) ---
int normalize_solution_path(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    bool visited[MAZE_HEIGHT][MAZE_WIDTH] = {false};
    Queue* q = create_queue();
    Point start_pos = {1, 1}, end_pos = {MAZE_HEIGHT - 2, MAZE_WIDTH - 2};
    PathNode* start_node = (PathNode*)malloc(sizeof(PathNode));
    start_node->pos = start_pos; start_node->parent = NULL;
    enqueue(q, start_node);
    visited[start_pos.y][start_pos.x] = true;
    PathNode* end_node = NULL;
    while (q->front != NULL) {
        PathNode* current = dequeue(q);
        if (current->pos.y == end_pos.y && current->pos.x == end_pos.x) { end_node = current; break; }
        int dy[] = {-1, 1, 0, 0}; int dx[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; i++) {
            Point next_pos = {current->pos.y + dy[i], current->pos.x + dx[i]};
            if (next_pos.y >= 0 && next_pos.y < MAZE_HEIGHT && next_pos.x >= 0 && next_pos.x < MAZE_WIDTH &&
                maze[next_pos.y][next_pos.x] == PATH && !visited[next_pos.y][next_pos.x]) {
                visited[next_pos.y][next_pos.x] = true;
                PathNode* next_p_node = (PathNode*)malloc(sizeof(PathNode));
                next_p_node->pos = next_pos; next_p_node->parent = current;
                enqueue(q, next_p_node);
            }
        }
    }
    while(q->front != NULL) { dequeue(q); } free(q);
    if (end_node == NULL) return -1;
    int path_len = 0; PathNode* path_arr[MAZE_WIDTH * MAZE_HEIGHT];
    for (PathNode* current = end_node; current != NULL; current = current->parent) { path_arr[path_len++] = current; }
    // NOTE: Path surgery logic could be added here to force len%4==0, but is complex.
    // For this challenge, we will just use the natural path length. Strict validation is enough.
    for (int i = 0; i < path_len; i++) { free(path_arr[i]); }
    // Total moves = 1 (enter) + (path_len - 1) (internal) + 1 (exit)
    return path_len + 1;
}


// --- STRICT PATH VALIDATION ---
void __attribute__((always_inline))validate_path(char maze[MAZE_HEIGHT][MAZE_WIDTH], const char *hex_input, int required_moves) {
    size_t len = strlen(hex_input);
    if (len == 0 || len % 2 != 0) longjmp(validation_failed_jmp, 1);
    size_t decoded_len = len / 2;
    size_t total_moves = decoded_len * 4;
    if (required_moves > total_moves || required_moves <= total_moves - 4) { longjmp(validation_failed_jmp, 1); }
    unsigned char* path = malloc(decoded_len); if (!path) longjmp(validation_failed_jmp, 1);
    for (size_t i = 0; i < decoded_len; i++) {
        if (sscanf(hex_input + 2 * i, "%2hhx", &path[i]) != 1) { free(path); longjmp(validation_failed_jmp, 1); }
    }
    bool visited[MAZE_HEIGHT][MAZE_WIDTH] = {false};
    Point player = {1, 0};
    visited[player.y][player.x] = true;
    int moves_taken = 0;
    for (size_t i = 0; i < decoded_len; i++) {
        for (int j = 0; j < 4; j++) {
            if (moves_taken >= required_moves) break;
            int move = (path[i] >> (6 - j * 2)) & 0x03;
            Point next = player;
            switch (move) {
                case 0b00: next.y--; break; case 0b01: next.y++; break;
                case 0b10: next.x--; break; case 0b11: next.x++; break;
            }
            if (next.y < 0 || next.y >= MAZE_HEIGHT || next.x < 0 || next.x >= MAZE_WIDTH || maze[next.y][next.x] == WALL) { free(path); longjmp(validation_failed_jmp, 1); }
            if (visited[next.y][next.x]) { free(path); longjmp(validation_failed_jmp, 1); }
            player = next; visited[player.y][player.x] = true;
            moves_taken++;
        }
    }
    free(path);
    if (player.y != MAZE_HEIGHT - 2 || player.x != MAZE_WIDTH - 1) { longjmp(validation_failed_jmp, 1); }
}


// --- SEED GENERATION (FROM IN-MEMORY HASH) ---
unsigned int __attribute__((always_inline))get_seed_from_memory() {
    char* exe_path = realpath("/proc/self/exe", NULL);
    if (!exe_path) { exit(EXIT_FAILURE); }
    FILE* maps_file = fopen("/proc/self/maps", "r");
    if (!maps_file) { free(exe_path); exit(EXIT_FAILURE); }
    SHA256_CTX sha256_ctx; SHA256_Init(&sha256_ctx);
    char line_buf[PATH_MAX + 100];
    while (fgets(line_buf, sizeof(line_buf), maps_file)) {
        unsigned long start, end; char perms[5]; char mapped_path[PATH_MAX] = {0};
        sscanf(line_buf, "%lx-%lx %4s %*x %*x:%*x %*d %s", &start, &end, perms, mapped_path);
        if (strcmp(mapped_path, exe_path) == 0) {
            if (perms[0] == 'r' && perms[1] == '-') { // 'r--p' and 'r-xp'
                SHA256_Update(&sha256_ctx, (void*)start, end - start);
            }
        }
    }
    fclose(maps_file); free(exe_path);
    unsigned char hash[SHA256_DIGEST_LENGTH]; SHA256_Final(hash, &sha256_ctx);
    unsigned int seed;
    memcpy(&seed, &hash[SHA256_DIGEST_LENGTH - sizeof(seed)], sizeof(seed));
    return seed;
}


int real() {
    initialize_security_probes();

    unsigned int seed = get_seed_from_memory();
    srand(seed);

    char maze[MAZE_HEIGHT][MAZE_WIDTH];
    generate_maze_kruskal(maze);

    int required_moves = normalize_solution_path(maze);

    maze[1][0] = PATH;
    maze[MAZE_HEIGHT - 2][MAZE_WIDTH - 1] = PATH;

    initscr(); cbreak(); noecho(); curs_set(0);

    if (setjmp(validation_failed_jmp)) {
        endwin();
        puts("Access Denied.");
        return EXIT_FAILURE;
    }

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(max_y / 2 - 2, (max_x - 30) / 2, "Synchronizing with Quantum Relay...");
    timing_probe_start();
    for(int i = 0; i <= 30; i++) {
		check_parent_debugger();
        mvprintw(max_y / 2, (max_x - 32) / 2, "[%-30s]", "=============================="+ (30-i));
        refresh();
        usleep(50000);
    }
    timing_probe_end();
    mvprintw(max_y / 2 + 2, (max_x - 30) / 2, "Enter Encryption Key:");

    char user_input[512] = {0};
    curs_set(1); echo();
    move(max_y / 2 + 2, (max_x - 30) / 2 + 23);
    wgetnstr(stdscr, user_input, sizeof(user_input) - 1);
    noecho(); curs_set(0);

	check_parent_debugger();
    validate_path(maze, user_input, required_moves);

    endwin();
    printf("R3CTF{%s}\n", user_input);
	asm volatile (
    ".rept 0x40000\n\t"
    ".byte 0x90\n\t"
    ".endr"
	);
    return EXIT_SUCCESS;
}

// --- MAIN FUNCTION ---
int main(int argc) {

	strdup("hello world"); // change to puts
	sleep(0);
	if (argc == 7) {
		real();
	} else {
		puts("hello world");
		exit(0);
	}

}

