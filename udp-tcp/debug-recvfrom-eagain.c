#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

void check_socket_status(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return;
    }
    printf("Socket is %s\n", (flags & O_NONBLOCK) ? "non-blocking" : "blocking");

    int option_value;
    socklen_t option_len = sizeof(option_value);
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &option_value, &option_len) == -1) {
        perror("getsockopt SO_RCVTIMEO");
    } else {
        printf("SO_RCVTIMEO: %d\n", option_value);
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP server is listening on port %d\n", PORT);
    check_socket_status(sockfd);

    while (1) {
        printf("Waiting for data...\n");
        ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                    (struct sockaddr *)&client_addr, &addr_len);

        if (recv_len < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("EAGAIN or EWOULDBLOCK error occurred. This is unexpected in blocking mode.\n");
                check_socket_status(sockfd);
            } else {
                perror("recvfrom failed");
            }
            continue;
        }

        buffer[recv_len] = '\0';
        printf("Received %zd bytes from %s:%d\n", recv_len,
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Data: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
