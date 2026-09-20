#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

static void dump_bytes(const unsigned char *data, size_t length)
{
    for (size_t i = 0; i < length; ++i) {
        if (i % 16 == 0)
            printf("%04zx: ", i);

        printf("%02x ", data[i]);

        if (i % 16 == 15 || i + 1 == length)
            putchar('\n');
    }
}

int main(void)
{
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    for (;;) {
        unsigned char packet[65535];

        ssize_t received = recv(fd, packet, sizeof(packet), 0);
        if (received < 0) {
            perror("recv");
            continue;
        }

        if ((size_t)received < sizeof(struct ip)) {
            fprintf(stderr, "Truncated IPv4 packet\n");
            continue;
        }

        const struct ip *ip = (const struct ip *)packet;
        size_t ip_header_length = (size_t)ip->ip_hl * 4;

        if (ip_header_length < sizeof(struct ip) ||
            (size_t)received < ip_header_length + sizeof(struct
            udphdr)) {
            fprintf(stderr, "Malformed or truncated packet\n");
            continue;
        }

        const struct udphdr *udp =
            (const struct udphdr *)(packet + ip_header_length);

        size_t udp_length = ntohs(udp->uh_ulen);

        printf("%s:%u -> ",
                inet_ntoa(ip->ip_src),
                ntohs(udp->uh_sport));

        printf("%s:%u, UDP length=%zu\n",
                inet_ntoa(ip->ip_dst),
                ntohs(udp->uh_dport),
                udp_length);

        dump_bytes(packet, (size_t)received);
    }

    close(fd);
    return EXIT_SUCCESS;
}