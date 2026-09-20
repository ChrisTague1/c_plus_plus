#include <stdio.h>
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

    struct sockaddr_in dest = {
        .sin_family = AF_INET,
        .sin_port = htons(5000),
    };

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &dest.sin_addr
    );

    const char *message = "hello from C";

    if (sendto(
        fd,
        message,
        strlen(message),
        0,
        (struct sockaddr *)&dest,
        sizeof(dest)
    ) < 0) {
        perror("sendto");
        return 1;
    }

    close(fd);
}