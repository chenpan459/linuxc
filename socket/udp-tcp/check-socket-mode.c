#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

int main() {
    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (flags & O_NONBLOCK) {
        printf("Socket is non-blocking by default\n");
    } else {
        printf("Socket is blocking by default\n");
    }

    close(sockfd);
    return 0;
}
