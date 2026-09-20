- https://beej.us/guide/bgnet/html/split-wide/index.html
- https://www.youtube.com/watch?v=LtXEMwSG5-8
    - https://www.youtube.com/watch?v=mStnzIEprH8
- https://www.rfc-editor.org/info/rfc768/
    - https://www.rfc-editor.org/info/rfc1112/
- https://www.rfc-editor.org/info/rfc9868/#section-21
- https://chatgpt.com/c/6aaf3538-d888-83ea-a97d-a85c92d72be5

# Running linux-like

```bash
colima start -f
```

```bash
docker run --rm -d --name udp-lab --cap-add=NET_RAW -v "$PWD":/work -w /work ubuntu:24.04 sleep infinity
```

- `--cap-add=NET_RAW` - give it permission for raw network access

```bash
docker exec -it udp-lab bash
```

```bash
apt-get update
apt-get install gcc
```

```bash
docker exec udp-lab ./sender
```

```bash
docker system prune --all --volumes --force
```

# Commands

- ifconfig
    - will show network interfaces (NICs would be here)
    - eth0, eth1, en1, etc.
    - a VLAN would be eth0.1001, ex.
    - effectively gives you 'fake' extra interfaces
- netstat -rn -f inet
- route -n get 239.10.10.10
