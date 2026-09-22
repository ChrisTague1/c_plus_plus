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

    bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    while (true) {
        Message message;

        struct sockaddr_in sender;
        socklen_t sender_len = sizeof(sender);

        ssize_t n = recvfrom(
            fd,
            &message,
            sizeof(message),
            0,
            reinterpret_cast<sockaddr*>(&sender),
            &sender_len
        );

        char sender_ip[INET_ADDRSTRLEN];

        inet_ntop(
            AF_INET,
            &sender.sin_addr,
            sender_ip,
            sizeof(sender_ip)
        );

        std::cout << sender_ip << ":" << ntohs(sender.sin_port) << " sent: " << n << " bytes\n";

        switch (message.message_type) {
            case MessageType::JoinServer:
                std::cout << message.join_server.name << " joined the server\n";
                break;
            default:
                break;
        }
    }

    return 0;
}