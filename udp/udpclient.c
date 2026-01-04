// udp_echo_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

int main() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

    char message[BUFFER_SIZE];
    printf("Enter message to send: ");
    fgets(message, BUFFER_SIZE, stdin);
    message[strcspn(message, "\n")] = '\0'; // Remove newline

    struct timeval start_time, end_time;
    socklen_t server_len = sizeof(server_addr);

    // Record start time
    gettimeofday(&start_time, NULL);

    // Send message to server
    sendto(client_socket, message, strlen(message), 0,
           (struct sockaddr*)&server_addr, server_len);

    // Receive echo from server
    char buffer[BUFFER_SIZE];
    int recv_len = recvfrom(client_socket, buffer, BUFFER_SIZE - 1, 0,
                            (struct sockaddr*)&server_addr, &server_len);
    if (recv_len < 0) {
        perror("Error receiving data");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Record end time
    gettimeofday(&end_time, NULL);

    buffer[recv_len] = '\0';
    printf("Echo from server: %s\n", buffer);

    // Calculate RTT in milliseconds
    double rtt = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                 (end_time.tv_usec - start_time.tv_usec) / 1000.0;
    printf("Round Trip Time (RTT): %.3f ms\n", rtt);

    close(client_socket);
    return 0;
}
