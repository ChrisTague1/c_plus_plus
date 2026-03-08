# Optimization Notes for main.cpp

## 1. Flat array instead of vector<vector<optional<int>>>

**Where:** Line 10 (type alias), and everywhere `Sudoku` is used

`vector<vector<...>>` means each row is a separate heap allocation scattered in memory. When you iterate the grid, the CPU cache has to chase pointers to different memory locations.

Replace with a flat `std::array`:

```cpp
constexpr int SUDOKU_SIZE = 9;
using Sudoku = std::array<int, SUDOKU_SIZE * SUDOKU_SIZE>;
// Access: sudoku[row * SUDOKU_SIZE + col]
// Use 0 to represent empty cells instead of std::optional
```

This gives you:
- One contiguous block of memory (cache-friendly)
- Stack-allocated (no heap allocation at all)
- `int` instead of `optional<int>` cuts cell size from 8 bytes to 4

**Experiment:** `experiments/flat_vs_nested.cpp`

## 2. constexpr instead of const

**Where:** Lines 12-13

```cpp
// Current
const int SUDOKU_SIZE = 4;
const int SUDOKU_ROOT = static_cast<int>(std::sqrt(SUDOKU_SIZE));

// Better
constexpr int SUDOKU_SIZE = 9;
constexpr int SUDOKU_ROOT = 3;  // or write a constexpr isqrt()
```

`const` means "I won't modify this." `constexpr` means "compute this at compile time." The value gets baked into the binary with zero runtime cost.

`std::sqrt` is not `constexpr` in C++17, so you either hardcode the root or write a `constexpr` integer square root (see `experiments/constexpr_demo.cpp` for an example).

**Experiment:** `experiments/constexpr_demo.cpp`

## 3. Pass std::string by const& (not by value)

**Where:** Line 15 — `extract_sudoku(std::string filename, ...)`

```cpp
// Current — copies the entire string
int extract_sudoku(std::string filename, Sudoku& sudoku);

// Fixed — no copy
int extract_sudoku(const std::string& filename, Sudoku& sudoku);
```

Passing by value allocates a new string and copies all the characters. `const&` just passes a pointer to the existing string.

**Experiment:** `experiments/const_correctness.cpp`

## 4. const& on print_sudoku

**Where:** Line 98

```cpp
// Current — implies mutation
void print_sudoku(Sudoku& sudoku);

// Fixed — signals read-only
void print_sudoku(const Sudoku& sudoku);
```

If a function doesn't modify its argument, mark the parameter `const&`. This is both documentation ("I only read this") and a safety guarantee (compiler will error if you accidentally mutate it).

**Experiment:** `experiments/const_correctness.cpp`

## 5. "\n" instead of std::endl

**Where:** Lines 19 and 117

```cpp
// Current — writes newline AND flushes the buffer (triggers a system call)
std::cerr << "Error: ..." << std::endl;

// Better — just writes a newline, buffer flushes when full
std::cerr << "Error: ...\n";
```

`std::endl` = `"\n"` + `flush()`. In a loop, that's one system call per line instead of letting the buffer batch writes. Note: `std::cerr` is unbuffered by default so the difference is smaller there, but it's a good habit.

**Experiment:** `experiments/endl_vs_newline.cpp`

## 6. Remove explicit inputFile.close()

**Where:** Line 42

```cpp
inputFile.close();  // unnecessary — RAII handles this
```

`std::ifstream`'s destructor automatically closes the file when it goes out of scope. Explicit `.close()` is a C/Java pattern. Removing it shows you understand RAII (Resource Acquisition Is Initialization), which is a core C++ concept.

## 7. Add missing #include <optional>

**Where:** Top of file (includes section)

You use `std::optional<int>` but don't include `<optional>`. It compiles because another header pulls it in transitively, but this is fragile — a different compiler or standard library version might not include it transitively.

Always include what you use.

## 8. Compiler flags (already done in Makefile)

Debug: `-g -O0 -fsanitize=address,undefined`
Release: `-O3 -march=native -DNDEBUG`

Use debug for development (catches memory errors), release for benchmarking.
