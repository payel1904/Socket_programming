// sender.c : Go-Back-N Sender using TCP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024
#define WINDOW_SIZE 4
#define TOTAL_FRAMES 10

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAX];
    int base = 0, nextseqnum = 0;
    int ack_recvd[TOTAL_FRAMES] = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect"); exit(1);
    }

    while (base < TOTAL_FRAMES) {
        // Send frames in window
        while (nextseqnum < base + WINDOW_SIZE && nextseqnum < TOTAL_FRAMES) {
            sprintf(buffer, "%d", nextseqnum);
            write(sockfd, buffer, strlen(buffer));
            printf("Sent Frame: %d\n", nextseqnum);
            nextseqnum++;
            sleep(1); // simulate delay
        }

        // Read ACK
        memset(buffer, 0, MAX);
        int n = read(sockfd, buffer, MAX);
        if (n > 0) {
            int ack = atoi(buffer);
            printf("Received ACK: %d\n", ack);
            if (ack >= base) {
                base = ack + 1;  // Slide window
            }
        } else {
            // Timeout: retransmit
            printf("Timeout! Resending from Frame %d\n", base);
            nextseqnum = base;
        }
    }
    close(sockfd);
    return 0;
}
