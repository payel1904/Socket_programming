#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_len;
    char msg[1024];
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_len = sizeof(serverAddr);

    while (1) {
        printf("Enter a number to check (or -1 to quit): ");
        int num;
        scanf("%d", &num);

        if (num == -1)
            break;

        sprintf(msg, "%d", num);
        sendto(clientSocket, msg, strlen(msg), 0,
               (struct sockaddr*)&serverAddr, addr_len);
        memset(msg, 0, sizeof(msg));
        recvfrom(clientSocket, msg, sizeof(msg), 0,
                 (struct sockaddr*)&serverAddr, &addr_len);

        printf("Server says: %s\n\n", msg);
    }

    close(clientSocket);
    return 0;
}
