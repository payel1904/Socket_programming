// udp_echo_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    struct sockaddr_in client_addr;
    char message[BUFFER_SIZE];
    int message_len;
} client_request_t;

void* handle_client(void* arg) {
    client_request_t* req = (client_request_t*)arg;

    // Echo the message back to the client
    sendto(req->socket, req->message, req->message_len, 0,
           (struct sockaddr*)&req->client_addr, sizeof(req->client_addr));

    printf("Echoed message to %s:%d -> %s\n",
           inet_ntoa(req->client_addr.sin_addr),
           ntohs(req->client_addr.sin_port),
           req->message);

    free(req);
    pthread_exit(NULL);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("UDP Echo Server is running on port %d\n", SERVER_PORT);

    while (1) {
        char buffer[BUFFER_SIZE];
        socklen_t client_len = sizeof(client_addr);
        int recv_len = recvfrom(server_socket, buffer, BUFFER_SIZE - 1, 0,
                                (struct sockaddr*)&client_addr, &client_len);
        if (recv_len < 0) {
            perror("Error receiving data");
            continue;
        }

        buffer[recv_len] = '\0';
        printf("Received from %s:%d -> %s\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               buffer);

        // Prepare request structure for the thread
        client_request_t* req = malloc(sizeof(client_request_t));
        req->socket = server_socket;
        req->client_addr = client_addr;
        req->message_len = recv_len;
        memcpy(req->message, buffer, recv_len);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*)req);
        pthread_detach(thread_id);
    }

    close(server_socket);
    return 0;
}
