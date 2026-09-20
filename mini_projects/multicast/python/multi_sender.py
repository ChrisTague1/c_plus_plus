import socket
import time

GROUP = "239.10.10.10"
PORT = 5000

sock = socket.socket(
    socket.AF_INET,
    socket.SOCK_DGRAM,
    socket.IPPROTO_UDP,
)

sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 1)

counter = 0

while True:
    message = f"packet {counter}".encode()

    sock.sendto(message, (GROUP, PORT))

    print(f"sent: {message!r}")

    counter += 1
    time.sleep(1)