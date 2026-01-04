#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char buffer[1024];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    addr_size = sizeof(serverAddr);

    while (1) {
        printf("Enter a number to find factorial (or -1 to exit): ");
        int num;
        scanf("%d", &num);

        if (num == -1)
            break;

        sprintf(buffer, "%d", num);
        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr*)&serverAddr, addr_size);
        memset(buffer, 0, sizeof(buffer));
        recvfrom(sockfd, buffer, sizeof(buffer), 0,
                 (struct sockaddr*)&serverAddr, &addr_size);

        printf("Factorial received from server: %s\n\n", buffer);
    }

    close(sockfd);
    return 0;
}
