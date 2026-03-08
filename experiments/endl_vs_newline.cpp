// endl_vs_newline.cpp
// Shows why std::endl is slower than "\n".
// This seems trivial but quant interviewers notice it — it signals
// whether you understand I/O buffering.
//
// Compile: clang++ -std=c++17 -O2 -o endl_vs_newline endl_vs_newline.cpp && ./endl_vs_newline

#include <chrono>
#include <iostream>
#include <sstream>

constexpr int N = 100'000;

int main() {
    // std::endl = "\n" + flush
    // flush forces the buffer to be written to the OS immediately.
    // In a loop, this means N system calls instead of a handful.

    // Redirect to stringstream so we're not measuring terminal speed
    std::stringstream ss1, ss2;

    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        ss1 << i << std::endl;  // writes "\n" AND flushes
    }
    auto end1 = std::chrono::high_resolution_clock::now();

    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        ss2 << i << "\n";  // just writes "\n", buffer flushes when full
    }
    auto end2 = std::chrono::high_resolution_clock::now();

    double t_endl = std::chrono::duration<double, std::milli>(end1 - start1).count();
    double t_newline = std::chrono::duration<double, std::milli>(end2 - start2).count();

    // Print to actual stdout
    std::cout << "std::endl:  " << t_endl << " ms\n";
    std::cout << "\"\\n\":       " << t_newline << " ms\n";
    std::cout << "endl is ~" << t_endl / t_newline << "x slower\n";

    // WHEN TO USE std::endl:
    // - Logging errors that must appear before a crash
    // - Interactive prompts where you need output visible immediately
    // - Debugging (though std::cerr is unbuffered by default)
    //
    // EVERYWHERE ELSE: use "\n"

    return 0;
}
