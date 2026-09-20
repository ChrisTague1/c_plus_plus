#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>

int main(void) {
    char message[256] = "Hello from the server!";

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(9002),
        .sin_addr.s_addr = INADDR_ANY,
    };

    int bind_status = bind(fd, (struct sockaddr *) &address, sizeof(address));

    if (bind_status == -1) {
        perror("bind failed");
        return 1;
    }

    listen(fd, 5);

    int client_socket = accept(fd, NULL, NULL);

    send(client_socket, message, sizeof(message), 0);

    close(fd);

    return 0;
}