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

## Generate Search Corpus

Recreate the deterministic large test corpus exactly:

```bash
python3 generate_test_data.py \
  --output test_data/generated \
  --seed 20260413 \
  --top-level-dirs 12 \
  --depth 3 \
  --branch-factor 4 \
  --files-per-dir 12 \
  --lines-per-file 120
```

Search one of the known seeded targets:

```bash
./build/grabber --path test_data/generated --search grabber-target-alpha
```

Known target locations are written to `test_data/generated/SEARCH_TARGETS.txt`.

## Project Structure

```text
src/
└── main.cpp   # Hello world entry point
```

## Random Commands

```bash
find test_data -type f -exec grep -H "item" {} \;
```

- https://gemini.google.com/app/d454f3f8af26a6f8