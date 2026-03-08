// move_semantics.cpp
// Move semantics are THE key C++ performance feature that distinguishes
// it from other languages. Quant firms will 100% ask about this.
//
// Core idea: instead of copying data, "move" it by transferring ownership
// of the underlying memory. The source is left in a valid but empty state.
//
// Compile: clang++ -std=c++17 -O2 -o move_semantics move_semantics.cpp && ./move_semantics

#include <chrono>
#include <iostream>
#include <string>
#include <utility>  // std::move
#include <vector>

// A type that tracks copies vs moves so you can see what happens
struct Tracker {
    std::vector<double> data;

    Tracker(int n) : data(n, 1.0) {}

    // Copy constructor
    Tracker(const Tracker& other) : data(other.data) {
        std::cout << "  COPY ctor (" << data.size() << " elements)\n";
    }

    // Move constructor — steals the guts, no allocation
    Tracker(Tracker&& other) noexcept : data(std::move(other.data)) {
        std::cout << "  MOVE ctor (nearly free)\n";
    }

    // Copy assignment
    Tracker& operator=(const Tracker& other) {
        data = other.data;
        std::cout << "  COPY assign (" << data.size() << " elements)\n";
        return *this;
    }

    // Move assignment
    Tracker& operator=(Tracker&& other) noexcept {
        data = std::move(other.data);
        std::cout << "  MOVE assign (nearly free)\n";
        return *this;
    }
};

// BAD: returns by value but the caller might copy
// (actually, modern compilers use RVO/NRVO here — see below)
std::vector<double> make_data(int n) {
    std::vector<double> v(n, 42.0);
    return v;  // NRVO (Named Return Value Optimization) — no copy OR move
}

// Key interview topic: when does std::move help?
void demonstrate_move() {
    std::cout << "--- Creating original ---\n";
    Tracker a(1'000'000);

    std::cout << "\n--- Copy a to b ---\n";
    Tracker b = a;  // copy: allocates 1M doubles, memcpy

    std::cout << "\n--- Move a to c ---\n";
    Tracker c = std::move(a);  // move: just swaps pointers

    std::cout << "\n--- After move, a.data.size() = " << a.data.size() << " ---\n";
    // a is now empty — its data was stolen. This is fine, a is in a
    // "valid but unspecified state". Don't use it except to reassign.

    std::cout << "\n--- push_back without move ---\n";
    std::vector<Tracker> vec;
    vec.reserve(2);  // avoid reallocation
    Tracker d(100);
    vec.push_back(d);  // copies d into the vector

    std::cout << "\n--- push_back with move ---\n";
    vec.push_back(std::move(d));  // moves d into the vector — no copy

    std::cout << "\n--- emplace_back (constructs in-place, even better) ---\n";
    vec.emplace_back(100);  // constructs directly in vector storage
}

// RVO/NRVO: the compiler can elide copies/moves entirely
void demonstrate_rvo() {
    std::cout << "\n=== RVO Demo ===\n";
    // No copy, no move — the compiler constructs directly in the caller's memory
    auto data = make_data(1'000'000);
    std::cout << "Got " << data.size() << " elements (no copy or move happened)\n";
}

int main() {
    demonstrate_move();
    demonstrate_rvo();

    // INTERVIEW TIP: common follow-up questions:
    // 1. "What is the rule of five?" — if you define any of
    //    destructor/copy ctor/copy assign/move ctor/move assign,
    //    you should define ALL of them.
    // 2. "When should you NOT use std::move?" — don't move from
    //    const objects (it silently copies), don't move return values
    //    (prevents RVO), don't move into functions that take by value
    //    unless you're done with the source.
    // 3. "What does noexcept do on move operations?" — lets containers
    //    use move instead of copy during reallocation (e.g. vector growth).

    return 0;
}
