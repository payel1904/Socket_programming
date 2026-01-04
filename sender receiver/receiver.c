// receiver.c : Go-Back-N Receiver using TCP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sockfd, newsock;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[MAX];
    int expected_frame = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind"); exit(1);
    }

    listen(sockfd, 5);

    len = sizeof(cliaddr);
    newsock = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
    if (newsock < 0) { perror("accept"); exit(1); }

    while (1) {
        memset(buffer, 0, MAX);
        int n = read(newsock, buffer, MAX);
        if (n <= 0) break;

        int frame_no = atoi(buffer);
        printf("Received Frame: %d\n", frame_no);

        if (frame_no == expected_frame) {
            printf("Sending ACK for Frame %d\n", frame_no);
            char ack[20];
            snprintf(ack, sizeof(ack), "%d", frame_no);
            write(newsock, ack, strlen(ack));
            expected_frame++;
        } else {
            printf("Discarded Frame %d, expecting %d\n", frame_no, expected_frame);
            // Resend last ACK (Go-Back-N behavior)
            char ack[20];
            snprintf(ack, sizeof(ack), "%d", expected_frame - 1);
            write(newsock, ack, strlen(ack));
        }
    }

    close(newsock);
    close(sockfd);
    return 0;
}
