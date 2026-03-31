#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

// ============================================================
// LAMBDAS IN C++
// ============================================================
// Syntax: [capture](params) -> return_type { body }
//   - capture:      what from the outer scope the lambda can see
//   - params:       just like a function's parameter list
//   - return_type:  usually inferred, rarely written explicitly
//   - body:         the code to run
// ============================================================


// ---- 1. THE BASICS -----------------------------------------

void basics() {
    std::cout << "\n--- 1. Basics ---\n";

    // A lambda is just an anonymous function you can store in a variable
    auto greet = []() {
        std::cout << "Hello from a lambda\n";
    };
    greet();

    // With parameters
    auto add = [](int a, int b) {
        return a + b;  // return type inferred as int
    };
    std::cout << "3 + 4 = " << add(3, 4) << "\n";

    // Explicit return type (rarely needed, but valid)
    auto divide = [](double a, double b) -> double {
        return a / b;
    };
    std::cout << "7 / 2 = " << divide(7, 2) << "\n";

    // Call immediately without storing (IIFE)
    int result = [](int x) { return x * x; }(9);
    std::cout << "9^2 = " << result << "\n";
}


// ---- 2. CAPTURES -------------------------------------------

void captures() {
    std::cout << "\n--- 2. Captures ---\n";

    int x = 10;
    int y = 20;

    // [=]  capture all by value (copies made at lambda creation time)
    auto byValue = [=]() {
        std::cout << "by value: x=" << x << " y=" << y << "\n";
        // x = 99;  // ERROR: captured by value, not modifiable by default
    };

    // [&]  capture all by reference (sees live values, can mutate)
    auto byRef = [&]() {
        x += 5;
        std::cout << "by ref: x=" << x << "\n";
    };

    // capture specific variables
    auto specific = [x, &y]() {
        // x is a copy, y is a reference
        y += 100;
        std::cout << "specific: x=" << x << " y=" << y << "\n";
    };

    byValue();
    byRef();    // x is now 15
    specific(); // y is now 120
    std::cout << "after all: x=" << x << " y=" << y << "\n";
}


// ---- 3. MUTABLE LAMBDAS ------------------------------------

void mutable_lambdas() {
    std::cout << "\n--- 3. Mutable ---\n";

    // By-value captures are const by default — use `mutable` to modify the copy
    int count = 0;

    auto counter = [count]() mutable {
        ++count;  // modifies the lambda's own copy, not the outer `count`
        std::cout << "internal count: " << count << "\n";
    };

    counter(); // 1
    counter(); // 2
    counter(); // 3
    std::cout << "outer count unchanged: " << count << "\n";

    // Compare: capture by reference actually modifies the outer variable
    auto realCounter = [&count]() {
        ++count;
    };
    realCounter();
    realCounter();
    std::cout << "outer count after ref captures: " << count << "\n";
}


// ---- 4. LAMBDAS WITH STL ALGORITHMS ------------------------

void with_algorithms() {
    std::cout << "\n--- 4. With algorithms ---\n";

    std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // sort ascending
    std::sort(nums.begin(), nums.end(), [](int a, int b) { return a < b; });
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    // filter: copy_if into a new vector
    std::vector<int> evens;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(evens),
        [](int n) { return n % 2 == 0; });
    for (int n : evens) std::cout << n << " ";
    std::cout << "\n";

    // transform: square every element
    std::vector<int> squared(nums.size());
    std::transform(nums.begin(), nums.end(), squared.begin(),
        [](int n) { return n * n; });
    for (int n : squared) std::cout << n << " ";
    std::cout << "\n";

    // accumulate with a lambda: sum of squares
    int sumOfSquares = std::accumulate(nums.begin(), nums.end(), 0,
        [](int acc, int n) { return acc + n * n; });
    std::cout << "sum of squares: " << sumOfSquares << "\n";

    // capture a threshold from outer scope
    int threshold = 5;
    auto count = std::count_if(nums.begin(), nums.end(),
        [threshold](int n) { return n > threshold; });
    std::cout << "elements > " << threshold << ": " << count << "\n";
}


// ---- 5. std::function --------------------------------------

void stored_lambdas() {
    std::cout << "\n--- 5. std::function ---\n";

    // std::function<return(params)> can hold any callable with that signature
    std::function<int(int, int)> op;

    op = [](int a, int b) { return a + b; };
    std::cout << "add: " << op(3, 4) << "\n";

    op = [](int a, int b) { return a * b; };
    std::cout << "mul: " << op(3, 4) << "\n";

    // Useful for storing callbacks or passing lambdas around
    auto applyTwice = [](std::function<int(int)> f, int x) {
        return f(f(x));
    };
    std::cout << "double applied twice to 3: "
              << applyTwice([](int x) { return x * 2; }, 3) << "\n";  // 12

    // Note: std::function has overhead (type erasure + heap allocation).
    // Prefer auto or template params for performance-critical paths.
}


// ---- 6. GENERIC LAMBDAS (C++14) ----------------------------

void generic_lambdas() {
    std::cout << "\n--- 6. Generic lambdas (auto params) ---\n";

    // `auto` params make a lambda a template — works for any type
    auto print = [](auto val) {
        std::cout << val << "\n";
    };
    print(42);
    print(3.14);
    print("hello");

    auto add = [](auto a, auto b) { return a + b; };
    std::cout << add(1, 2) << "\n";
    std::cout << add(1.5, 2.5) << "\n";
    std::cout << add(std::string("foo"), std::string("bar")) << "\n";
}


// ---- 7. RETURNING LAMBDAS ----------------------------------

auto makeMultiplier(int factor) {
    // Captures `factor` by value — safe to return because the copy lives
    // inside the lambda itself, not on this function's stack
    return [factor](int x) { return x * factor; };
}

void returning_lambdas() {
    std::cout << "\n--- 7. Returning lambdas ---\n";

    auto triple = makeMultiplier(3);
    auto times10 = makeMultiplier(10);

    std::cout << "triple(5) = " << triple(5) << "\n";
    std::cout << "times10(7) = " << times10(7) << "\n";
}


// ---- 8. RECURSIVE LAMBDAS ----------------------------------

void recursive_lambdas() {
    std::cout << "\n--- 8. Recursive lambdas ---\n";

    // A lambda can't refer to itself by name in a capture directly.
    // Option 1: use std::function (simple but has overhead)
    std::function<int(int)> fib = [&fib](int n) -> int {
        return n <= 1 ? n : fib(n - 1) + fib(n - 2);
    };
    std::cout << "fib(10) = " << fib(10) << "\n";

    // Option 2 (C++23): use `this` parameter — the lambda receives itself
    // auto fib2 = [](this auto self, int n) -> int {
    //     return n <= 1 ? n : self(n - 1) + self(n - 2);
    // };
}


// ---- 9. IMMEDIATELY INVOKED + INIT CAPTURES (C++14) --------

void init_captures() {
    std::cout << "\n--- 9. Init captures ---\n";

    // [name = expr] lets you create a new variable inside the capture list
    int x = 42;
    auto lambda = [y = x * 2]() {
        std::cout << "y = " << y << "\n";  // 84
    };
    lambda();

    // Move-capture: useful for moving unique_ptr or other non-copyable types
    auto ptr = std::make_unique<int>(99);
    auto owner = [p = std::move(ptr)]() {
        std::cout << "owned value: " << *p << "\n";
    };
    owner();
    // ptr is now null — ownership moved into the lambda
    std::cout << "ptr after move: " << (ptr ? "valid" : "null") << "\n";
}


int main() {
    basics();
    captures();
    mutable_lambdas();
    with_algorithms();
    stored_lambdas();
    generic_lambdas();
    returning_lambdas();
    recursive_lambdas();
    init_captures();

    return 0;
}
