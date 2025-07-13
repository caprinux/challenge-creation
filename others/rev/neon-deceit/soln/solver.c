#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

// --- Maze Configuration (Copied from challenge) ---
#define MAZE_WIDTH 51
#define MAZE_HEIGHT 21
#define WALL '#'
#define PATH ' '

// --- Data Structures (Copied from challenge) ---
typedef struct { int y; int x; } Point;
typedef struct { int y; int x; } Wall;

// Pathfinding data structures
typedef struct PathNode {
    Point pos;
    struct PathNode* parent; // Used to backtrack and find the path
} PathNode;

typedef struct QueueNode {
    PathNode* p_node;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode *front, *rear;
} Queue;

// --- Helper Functions (Copied/Adapted from challenge) ---

// Queue implementation for BFS
Queue* create_queue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, PathNode* p_node) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->p_node = p_node;
    temp->next = NULL;
    if (q->rear == NULL) { q->front = q->rear = temp; return; }
    q->rear->next = temp;
    q->rear = temp;
}

PathNode* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    QueueNode* temp = q->front;
    PathNode* p_node = temp->p_node;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return p_node;
}

void free_queue(Queue* q) {
    while(q->front != NULL) {
        PathNode* node = dequeue(q);
        free(node);
    }
    free(q);
}

// Disjoint Set Union for maze generation
int find_set(int i, int* parent) {
    if (parent[i] == i) return i;
    return parent[i] = find_set(parent[i], parent);
}

void unite_sets(int a, int b, int* parent) {
    a = find_set(a, parent);
    b = find_set(b, parent);
    if (a != b) parent[b] = a;
}

// The exact random comparator from the challenge
int random_comparator(const void* a, const void* b) {
    (void)a; (void)b;
    return (rand() % 3) - 1;
}

// The exact maze generation logic from the challenge
void generate_maze_kruskal(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    memset(maze, WALL, MAZE_HEIGHT * MAZE_WIDTH);
    Wall* walls = malloc(sizeof(Wall) * MAZE_WIDTH * MAZE_HEIGHT);
    int wall_count = 0;
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
            unite_sets(c1, c2, parent_set);
            maze[r][c] = PATH;
        }
    }
    free(walls);
    free(parent_set);
}

// --- SOLVER LOGIC ---

// Finds the solution path using BFS and returns the final node
PathNode* find_solution_path(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    bool visited[MAZE_HEIGHT][MAZE_WIDTH] = {false};
    Queue* q = create_queue();

    // The actual start and end points for the player
    Point start_pos = {1, 0};
    Point end_pos = {MAZE_HEIGHT - 2, MAZE_WIDTH - 1};

    // Create start node and enqueue it
    PathNode* start_node = (PathNode*)malloc(sizeof(PathNode));
    start_node->pos = start_pos;
    start_node->parent = NULL;
    enqueue(q, start_node);
    visited[start_pos.y][start_pos.x] = true;

    PathNode* end_node = NULL;
    while (q->front != NULL) {
        PathNode* current = dequeue(q);

        // Check if we reached the end
        if (current->pos.y == end_pos.y && current->pos.x == end_pos.x) {
            end_node = current;
            break;
        }

        // Explore neighbors (Up, Down, Left, Right)
        int dy[] = {-1, 1, 0, 0};
        int dx[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; i++) {
            Point next_pos = {current->pos.y + dy[i], current->pos.x + dx[i]};
            if (next_pos.y >= 0 && next_pos.y < MAZE_HEIGHT &&
                next_pos.x >= 0 && next_pos.x < MAZE_WIDTH &&
                maze[next_pos.y][next_pos.x] == PATH &&
                !visited[next_pos.y][next_pos.x]) {
                
                visited[next_pos.y][next_pos.x] = true;
                PathNode* next_p_node = (PathNode*)malloc(sizeof(PathNode));
                next_p_node->pos = next_pos;
                next_p_node->parent = current;
                enqueue(q, next_p_node);
            }
        }
    }

    // Free remaining nodes in the queue if the path was found early
    // Note: Don't free the solution path nodes yet!
    while(q->front != NULL) {
        dequeue(q); // Just remove from queue, don't free the PathNode itself
    }
    free(q);

    return end_node;
}

void print_maze(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            putchar(maze[y][x]);
        }
        putchar('\n');
    }
}

// --- MAIN SOLVER ---
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <seed>\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned int seed = strtoul(argv[1], NULL, 10);
    printf("Using seed: %u\n\n", seed);

    // 1. SEED the random number generator
    srand(seed);

    // 2. GENERATE the maze exactly like the challenge does
    char maze[MAZE_HEIGHT][MAZE_WIDTH];
    generate_maze_kruskal(maze);

    // Open the entrance and exit
    maze[1][0] = PATH;
    maze[MAZE_HEIGHT - 2][MAZE_WIDTH - 1] = PATH;

    printf("--- Generated Maze ---\n");
    print_maze(maze);
    printf("----------------------\n\n");

    // 3. SOLVE the maze using BFS
    PathNode* solution_end_node = find_solution_path(maze);
    if (!solution_end_node) {
        fprintf(stderr, "Error: Could not find a solution path for this seed.\n");
        return EXIT_FAILURE;
    }

    // 4. RECONSTRUCT the path by walking backwards from the end node
    int path_len = 0;
    for (PathNode* curr = solution_end_node; curr != NULL; curr = curr->parent) {
        path_len++;
    }

    Point* path = malloc(sizeof(Point) * path_len);
    int index = path_len - 1;
    for (PathNode* curr = solution_end_node; curr != NULL; curr = curr->parent) {
        path[index--] = curr->pos;
    }

    // 5. CONVERT the path of (y,x) points into a sequence of moves
    // Path has path_len points, which means path_len-1 moves.
    int num_moves = path_len - 1;
    unsigned char* moves = malloc(num_moves);
    for (int i = 0; i < num_moves; i++) {
        Point p1 = path[i];
        Point p2 = path[i+1];
        if (p2.y < p1.y)      moves[i] = 0b00; // Up
        else if (p2.y > p1.y) moves[i] = 0b01; // Down
        else if (p2.x < p1.x) moves[i] = 0b10; // Left
        else                  moves[i] = 0b11; // Right
    }

    // 6. ENCODE the moves into the final hex string
    printf("--- Solution ---\n");
    printf("Password (hex path): ");
    
    unsigned char current_byte = 0;
    int bits_in_byte = 0;
    for (int i = 0; i < num_moves; i++) {
        // Pack the 2-bit move into the current byte
        current_byte = (current_byte << 2) | moves[i];
        bits_in_byte += 2;

        if (bits_in_byte == 8) {
            printf("%02x", current_byte);
            current_byte = 0;
            bits_in_byte = 0;
        }
    }
    // Handle the last, possibly incomplete, byte
    if (bits_in_byte > 0) {
        // Left-shift to pad the remaining bits with 0, as per standard practice
        current_byte <<= (8 - bits_in_byte);
        printf("%02x", current_byte);
    }
    printf("\n\n");


    // 7. CLEAN UP memory
    free(path);
    free(moves);
    PathNode* curr = solution_end_node;
    while (curr) {
        PathNode* temp = curr;
        curr = curr->parent;
        free(temp);
    }
    
    return EXIT_SUCCESS;
}
