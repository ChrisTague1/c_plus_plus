#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "protocol.h"

int main(void) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr {
        .sin_family = AF_INET,
        .sin_port = htons(5000),
        .sin_addr.s_addr = INADDR_ANY,
    };

    bind(fd, (struct sockaddr *) &addr, sizeof(addr));

    while (true) {
        std::cout << "this is awesome!" << std::endl;
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

        char sender_ip[INET_ADDRSTRLEN];

        inet_ntop(
            AF_INET,
            &sender.sin_addr,
            sender_ip,
            sizeof(sender_ip)
        );

        std::cout << sender_ip << ":" << ntohs(sender.sin_port) << n << buffer << "\n";
    }

    return 0;
}