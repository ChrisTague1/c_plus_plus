# Grabber

Basic C++20 project scaffolded with CMake.

## Dependencies

- CMake 3.20+
- A C++20 compiler

## Build

```bash
cmake -B build -S .
cmake --build build
```

## Run

```bash
./build/grabber
```

## Project Structure

```text
src/
└── main.cpp   # Hello world entry point
```

## Random Commands

```bash
find test_data -type f -exec grep -H "item" {} \;
```