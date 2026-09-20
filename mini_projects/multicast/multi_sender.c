#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define GROUP "239.10.10.10"
#define PORT 5000

int main(void) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd < 0) {
        perror("socket");
        return 1;
    }

    unsigned char ttl = 1;

    setsockopt(
        fd,
        IPPROTO_IP,
        IP_MULTICAST_TTL,
        &ttl,
        sizeof(ttl)
    );

    struct sockaddr_in destination = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
    };

    inet_pton(
        AF_INET,
        GROUP,
        &destination.sin_addr
    );

    for (int i = 0; ; i++) {
        char message[128];

        snprintf(
            message,
            sizeof(message),
            "multicast packet %d",
            i
        );

        sendto(
            fd,
            message,
            strlen(message),
            0,
            (struct sockaddr *)&destination,
            sizeof(destination)
        );

        printf("sent: %s\n", message);

        sleep(1);
    }
}