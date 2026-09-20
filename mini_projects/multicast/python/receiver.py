import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.bind(("0.0.0.0", 5000))

print("Listening on UDP port 5000")

while True:
    data, addr = sock.recvfrom(65535)

    print(f"from={addr} bytes={len(data)} data={data!r}")