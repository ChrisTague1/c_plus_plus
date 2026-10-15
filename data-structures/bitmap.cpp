#include <iostream>
#include <bitset>

int main() {
    std::cout << std::bitset<8>(static_cast<int8_t>(1)) << "\n";
    std::cout << std::bitset<8>(static_cast<int8_t>(0)) << "\n";
    std::cout << std::bitset<8>(static_cast<int8_t>(-1)) << "\n";
    std::cout << std::bitset<8>(static_cast<int8_t>(-2)) << "\n";
    std::cout << std::bitset<8>(static_cast<int8_t>(-3)) << "\n";
    std::cout << std::bitset<8>(static_cast<int8_t>(-4)) << "\n";

    return 0;
}