#include <iostream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "protocol.h"

int main(void) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(0), // kernel chooses my port
    };

    bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    sockaddr_in dest {
        .sin_family = AF_INET,
        .sin_port = htons(5000),
    };

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &dest.sin_addr
    );

    const Message message {
        .message_type = MessageType::JoinServer,
        .join_server.name = "Chris"
    };

    sendto(
        fd,
        &message,
        sizeof(message),
        0,
        reinterpret_cast<sockaddr*>(&dest),
        sizeof(dest)
    );

    char buffer[256];
    ssize_t n = recv(fd, &buffer, sizeof(buffer), 0);

    std::cout << n << " bytes from server: " << buffer << "\n";

    close(fd);

    return 0;
}