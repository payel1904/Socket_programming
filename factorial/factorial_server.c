#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
long long factorial(int n) {
    if (n < 0) return -1;  
    long long fact = 1;
    for (int i = 1; i <= n; i++)
        fact *= i;
    return fact;
}

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[1024];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    printf("UDP Server is running... Waiting for client messages...\n");

    while (1) {
        addr_size = sizeof(clientAddr);
        memset(buffer, 0, sizeof(buffer));
        int nBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                              (struct sockaddr*)&clientAddr, &addr_size);
        if (nBytes < 0) {
            perror("Receive failed");
            continue;
        }

        int num = atoi(buffer);
        printf("Received number from client: %d\n", num);
        long long result = factorial(num);
        sprintf(buffer, "%lld", result);
        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr*)&clientAddr, addr_size);

        printf("Sent factorial result: %s\n\n", buffer);
    }

    close(sockfd);
    return 0;
}
