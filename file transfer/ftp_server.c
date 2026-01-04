#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd, newsock;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    socklen_t len;
    FILE *fp;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);
    printf("Server waiting for connection...\n");

    len = sizeof(cliaddr);
    newsock = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
    printf("Client connected.\n");

    fp = fopen("file.txt", "r");
    if (fp == NULL) {
        strcpy(buffer, "File not found.");
        send(newsock, buffer, strlen(buffer), 0);
    } else {
        while (fgets(buffer, sizeof(buffer), fp)) {
            send(newsock, buffer, strlen(buffer), 0);
        }
        fclose(fp);
    }

    printf("File sent successfully.\n");
    close(newsock);
    close(sockfd);
    return 0;
}
