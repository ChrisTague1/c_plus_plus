import socket
import struct

GROUP = "239.10.10.10"
PORT = 5000

sock = socket.socket(
    socket.AF_INET,
    socket.SOCK_DGRAM,
    socket.IPPROTO_UDP,
)

sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# only needed on mac?
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)

sock.bind(("", PORT))

membership = struct.pack(
    "4s4s",
    socket.inet_aton(GROUP),
    socket.inet_aton("0.0.0.0") # you could sent this to explicitly decide which NIC to go to
    # ifconfig shows you these, ex. eth0, eth1, etc.
)

sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, membership)

print(f"Joined multicast group {GROUP}:{PORT}")

while True:
    data, addr = sock.recvfrom(65535)

    print(f"{addr[0]}:{addr[1]} -> {data.decode(errors='replace')}")