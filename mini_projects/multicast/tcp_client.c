#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>

int main(void) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(9002),
        .sin_addr.s_addr = INADDR_ANY,
    };

    int connection_status = connect(
        fd,
        (struct sockaddr *) &address,
        sizeof(address)
    );

    if (connection_status == -1) {
        perror("connection failed");
        return 1;
    }

    char buffer[256];
    ssize_t n = recv(fd, &buffer, sizeof(buffer), 0);

    printf("Server gave data: %s\n", buffer);

    close(fd);

    return 0;
}