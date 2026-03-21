# Poker

Multiplayer poker game server and client written in C++17 using Asio for networking.

## Dependencies

- CMake 3.20+
- A C++17 compiler
- [Asio](https://github.com/chriskohlhoff/asio) (fetched automatically by CMake)

## Build

```bash
cmake -B build -S .
cmake --build build
```

## Run

Start the server in one terminal:

```bash
./build/server
```

Connect one or more clients in separate terminals:

```bash
./build/client
```

## Project Structure

```
src/
├── server.cpp   # TCP server, thread-per-client
└── client.cpp   # Interactive TCP client
```

## Roadmap

- [ ] Message protocol (JSON or custom framing)
- [ ] Lobby / room system
- [ ] Game state machine (deal, bet rounds, showdown)
- [ ] Hand evaluation
- [ ] Multiple concurrent tables
- [ ] Simple TUI for the client
