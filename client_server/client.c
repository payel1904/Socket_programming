#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[MAX], msg[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
        perror("Connection failed"); exit(1);
    }
    if (!fork()) { // Child: read from server
        while (1) {
            int n = read(sock, buffer, MAX);
            if (n > 0) { buffer[n] = '\0'; printf("Friend: %s\n", buffer); }
        }
    } else {// Parent: send to server
        while (1) {
            fgets(msg, MAX, stdin);
            write(sock, msg, strlen(msg));
        }
    }

    close(sock);
    return 0;
}
