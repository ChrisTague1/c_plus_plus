#include <iostream>
#include <string>
#include <unordered_map>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "protocol.h"

struct SockAddrMapper {
    bool operator()(const sockaddr_in& lhs, const sockaddr_in& rhs) const noexcept {
        return lhs.sin_addr.s_addr == rhs.sin_addr.s_addr && lhs.sin_port == rhs.sin_port;
    }

    std::size_t operator()(const sockaddr_in& sa) const noexcept {
        std::size_t h1 = std::hash<uint32_t>{}(sa.sin_addr.s_addr);
        std::size_t h2 = std::hash<uint32_t>{}(sa.sin_port);

        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

using SockAddrMap = std::unordered_map<
    sockaddr_in,
    std::string,
    SockAddrMapper,
    SockAddrMapper
>;

int main(void) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr {
        .sin_family = AF_INET,
        .sin_port = htons(5000),
        .sin_addr.s_addr = INADDR_ANY,
    };

    bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    SockAddrMap player_map;

    while (true) {
        Message message;

        sockaddr_in sender;
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
            case MessageType::JoinServer: {
                std::string_view name = message.join_server.name;

                auto [it, inserted] = player_map.try_emplace(sender, name);

                std::string reply;

                if (inserted) {
                    std::cout << name << " joined the server\n";
                    reply = std::format("Welcome, {}", name);

                } else {
                    std::cout << name << " already joined the server, ignoring\n";
                    reply = std::format("{}, you are pushing your luck!", name);
                }

                sendto(
                    fd,
                    reply.data(),
                    reply.length(),
                    0,
                    reinterpret_cast<sockaddr*>(&sender),
                    sizeof(sender)
                );

                break;
            }
            default:
                break;
        }
    }

    return 0;
}