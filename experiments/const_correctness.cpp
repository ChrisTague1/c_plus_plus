// const_correctness.cpp
// Demonstrates const-correctness — a core C++ interview topic.
// Quant firms will judge you on this. The rule is simple:
// if a function doesn't modify something, mark it const.
//
// Compile: clang++ -std=c++17 -Wall -Wextra -o const_correctness const_correctness.cpp

#include <iostream>
#include <string>
#include <vector>

struct Position {
    double price;
    int quantity;

    // const member function: promises not to modify the object
    double notional() const {
        return price * quantity;
    }

    // non-const: modifies the object
    void fill(int qty, double px) {
        quantity += qty;
        price = px;
    }
};

// BAD: takes by non-const ref but doesn't modify
// An interviewer sees this and thinks "does this mutate my data?"
double bad_total_notional(std::vector<Position>& positions) {
    double total = 0.0;
    for (auto& pos : positions) {  // non-const ref — why?
        total += pos.notional();
    }
    return total;
}

// GOOD: const ref signals "I only read"
double good_total_notional(const std::vector<Position>& positions) {
    double total = 0.0;
    for (const auto& pos : positions) {
        total += pos.notional();  // only works because notional() is const
    }
    return total;
}

// BAD: takes string by value — copies the entire string
void bad_print(std::string msg) {
    std::cout << msg << "\n";
}

// GOOD: const ref — no copy
void good_print(const std::string& msg) {
    std::cout << msg << "\n";
}

// BEST for read-only string params: string_view — no copy, no ownership
// Works with std::string, const char*, substrings, etc.
void best_print(std::string_view msg) {
    std::cout << msg << "\n";
}

int main() {
    std::vector<Position> book = {
        {100.5, 200},
        {50.25, -100},
        {75.0, 500},
    };

    std::cout << "Total notional: " << good_total_notional(book) << "\n";

    // const object can only call const member functions
    const Position& first = book[0];
    std::cout << "First notional: " << first.notional() << "\n";
    // first.fill(10, 101.0);  // COMPILE ERROR — first is const

    // string_view works with any string-like thing
    std::string s = "hello from std::string";
    best_print(s);              // from std::string
    best_print("hello from literal");  // from const char*

    return 0;
}
