#include <stdio.h>
#include <stdlib.h>
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

    int reuse = 1;

    setsockopt(
        fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &reuse,
        sizeof(reuse)
    );

    // needed on mac
    setsockopt(
        fd,
        SOL_SOCKET,
        SO_REUSEPORT,
        &reuse,
        sizeof(reuse)
    );

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    if (bind(
        fd,
        (struct sockaddr *) &address,
        sizeof(address)
    ) < 0) {
        perror("bind");
        return 1;
    }

    struct ip_mreq membership;

    membership.imr_multiaddr.s_addr = inet_addr(GROUP);

    membership.imr_interface.s_addr = htonl(INADDR_ANY); // just like the python override this to specify NIC/VLAN

    if (setsockopt(
        fd,
        IPPROTO_IP,
        IP_ADD_MEMBERSHIP,
        &membership,
        sizeof(membership)
    ) < 0) {
        perror("IP_ADD_MEMBERSHIP");
        return 1;
    }

    printf("Listening to %s:%d\n", GROUP, PORT);

    while (1) {
        char buffer[2048];

        ssize_t n = recvfrom(
            fd,
            buffer,
            sizeof(buffer),
            0,
            NULL,
            NULL
        );

        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        printf("received %zd bytes: %.*s\n", n, (int) n, buffer);
    }
}