#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 4096
void handle_connection(int client_socket);

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);
    printf("Open your browser and go to http://127.0.0.1:%d\n", PORT);
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            perror("Accept failed");
            continue; 
        }
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Accepted connection from %s\n", client_ip);
        handle_connection(clie nt_socket);
        close(client_socket);
    }

    close(server_fd);
    return 0;
}
void handle_connection(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char http_header[BUFFER_SIZE] = {0};
    long valread = read(client_socket, buffer, BUFFER_SIZE);
    if (valread <= 0) {
        perror("Read failed");
        return;
    }

    printf("--- Request from Browser ---\n%s\n--------------------------\n", buffer);
    
    FILE *web_page = fopen("index.html", "r");

    if (web_page == NULL) {
        char *response_404 = "HTTP/1.1 404 NOT FOUND\r\n"
                             "Content-Type: text/plain\r\n"
                             "Content-Length: 13\r\n\r\n"
                             "404 Not Found";
        printf("Sending 404 Not Found\n");
        send(client_socket, response_404, strlen(response_404), 0);
    } else {
        fseek(web_page, 0, SEEK_END);
        long file_size = ftell(web_page);
        fseek(web_page, 0, SEEK_SET);
        char *file_content = (char *)malloc(file_size + 1);
        if (file_content == NULL) {
            perror("Failed to allocate memory for file");
            fclose(web_page);
            return;
        }
        fread(file_content, 1, file_size, web_page);
        fclose(web_page);
        file_content[file_size] = 0;
        sprintf(http_header, 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %ld\r\n\r\n", 
                file_size);
        
        printf("Sending 200 OK with %ld bytes\n", file_size);
        send(client_socket, http_header, strlen(http_header), 0);
        send(client_socket, file_content, file_size, 0);
        free(file_content);
    }
}