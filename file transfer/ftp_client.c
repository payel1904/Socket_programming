#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("Connected to server. Receiving file...\n\n");

    while ((n = recv(sockfd, buffer, sizeof(buffer)-1, 0)) > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    printf("\n\nFile received successfully.\n");
    close(sockfd);
    return 0;
}
