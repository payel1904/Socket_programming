#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int checkPalindrome(int num) {
    int temp = num, rev = 0, digit;
    while (num > 0) {
        digit = num % 10;
        rev = rev * 10 + digit;
        num /= 10;
    }
    return (temp == rev);
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_len;
    char msg[1024];
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        exit(1);
    }

    printf("Server started... waiting for messages from clients.\n");

    while (1) {
        addr_len = sizeof(clientAddr);
        memset(msg, 0, sizeof(msg));
        recvfrom(serverSocket, msg, sizeof(msg), 0,
                 (struct sockaddr*)&clientAddr, &addr_len);

        int num = atoi(msg);
        printf("Received number: %d\n", num);
        char reply[100];
        if (checkPalindrome(num))
            strcpy(reply, "Yes, it is a palindrome number!");
        else
            strcpy(reply, "No, it is not a palindrome number!");
        sendto(serverSocket, reply, strlen(reply), 0,
               (struct sockaddr*)&clientAddr, addr_len);

        printf("Response sent: %s\n\n", reply);
    }

    close(serverSocket);
    return 0;
}
