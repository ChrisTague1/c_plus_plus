// constexpr_demo.cpp
// constexpr means "evaluate at compile time". The result is baked into
// the binary — zero runtime cost. Quant firms love this.
//
// Compile: clang++ -std=c++17 -O2 -o constexpr_demo constexpr_demo.cpp && ./constexpr_demo

#include <array>
#include <iostream>

// const vs constexpr:
// - const: "I promise not to modify this" (may still be computed at runtime)
// - constexpr: "This MUST be computable at compile time"

// BAD: const — the compiler MAY evaluate at compile time, but doesn't have to
const int SIZE_A = 9;

// GOOD: constexpr — guaranteed compile-time evaluation
constexpr int SIZE_B = 9;

// constexpr function: if given constexpr inputs, runs at compile time
constexpr int isqrt(int n) {
    int i = 0;
    while ((i + 1) * (i + 1) <= n) ++i;
    return i;
}

// This is computed at COMPILE TIME — the binary just contains "3"
constexpr int SUDOKU_ROOT = isqrt(9);

// You can even build lookup tables at compile time
constexpr auto build_squares() {
    std::array<int, 10> table{};
    for (int i = 0; i < 10; ++i) {
        table[i] = i * i;
    }
    return table;
}

// This entire table is computed at compile time and embedded in the binary
constexpr auto SQUARES = build_squares();

// static_assert: compile-time check (fails to compile if false)
static_assert(SUDOKU_ROOT == 3, "sqrt(9) should be 3");
static_assert(SQUARES[5] == 25, "5^2 should be 25");

// constexpr + templates: compile-time fibonacci (interview classic)
template <int N>
constexpr int fib() {
    if constexpr (N <= 1) return N;
    else return fib<N-1>() + fib<N-2>();
}

// if constexpr: compile-time branching (C++17)
// The "wrong" branch is not even compiled — useful for template metaprogramming
template <typename T>
void print_type_info(const T& val) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << val << " is an integer type\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << val << " is a floating point type\n";
    } else {
        std::cout << "unknown type\n";
    }
}

int main() {
    std::cout << "SUDOKU_ROOT = " << SUDOKU_ROOT << "\n";
    std::cout << "SQUARES[7] = " << SQUARES[7] << "\n";

    // fib<20> is computed entirely at compile time
    std::cout << "fib(20) = " << fib<20>() << "\n";

    print_type_info(42);
    print_type_info(3.14);

    // WHY THIS MATTERS FOR QUANT:
    // In a trading system, anything you can compute at compile time
    // is one less thing to compute in the hot path. Lookup tables,
    // configuration constants, mathematical identities — push as much
    // as possible to constexpr.

    return 0;
}
