#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(5000),
        .sin_addr.s_addr = INADDR_ANY,
    };

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    while (1) {
        char buffer[2048];

        struct sockaddr_in sender;
        socklen_t sender_len = sizeof(sender);

        ssize_t n = recvfrom(
            fd,
            buffer,
            sizeof(buffer),
            0,
            (struct sockaddr *) &sender,
            &sender_len
        );

        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        char sender_ip[INET_ADDRSTRLEN];

        inet_ntop(
            AF_INET,
            &sender.sin_addr,
            sender_ip,
            sizeof(sender_ip)
        );

        printf("%s:%d -> %.*s\n",
            sender_ip,
            ntohs(sender.sin_port),
            (int) n,
            buffer
        );
    }

    close(fd);
}