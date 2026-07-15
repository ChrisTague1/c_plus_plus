#include <iostream>
#include <array>
#include <fstream>

int main() {
    std::ifstream file("justfile");

    std::array<char, 4096> buffer;

    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        std::size_t n = file.gcount();

        for (std::size_t i = 0; i < n; ++i) {
            char c = buffer[i];

            // the logic
        }
    }

    return 0;
}