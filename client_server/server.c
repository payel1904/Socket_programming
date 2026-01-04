#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define MAX 1024
int main() {
    int server_fd, client1, client2;
    struct sockaddr_in addr;
    char buffer[MAX];
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("Socket failed"); exit(1); }
     addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);//htons (“host-to-network short”) converts 8080 to the network’s big-endian byte order
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed"); exit(1);
    }
    listen(server_fd, 2);
    printf("Server running... waiting for clients\n");
    client1 = accept(server_fd, NULL, NULL);
    printf("Client 1 connected\n");
    client2 = accept(server_fd, NULL, NULL);
    printf("Client 2 connected\n");
    // Chat relay between two clients
    if (!fork()) { // Child handles client1 → client2
        while (1) {
            int n = read(client1, buffer, MAX);
            if (n <= 0) break;
            buffer[n] = '\0';
            write(client2, buffer, strlen(buffer));
        }
    } else {       // Parent handles client2 → client1
        while (1) {
            int n = read(client2, buffer, MAX);
            if (n <= 0) break;
            buffer[n] = '\0';
            write(client1, buffer, strlen(buffer));
        }
    }

    close(client1); close(client2); close(server_fd);
    return 0;
}
