#include <iostream>
#include <variant>
#include <string>
#include <iomanip>
#include <span>

using MyVariant = std::variant<int, double, std::string>;

void inspectVariant() {
    MyVariant v = 42; // Initialize with a value

    std::cout << "--- Variant Metrics ---\n";
    std::cout << "sizeof(MyVariant):     " << sizeof(MyVariant) << " bytes\n";
    std::cout << "alignof(MyVariant):    " << alignof(MyVariant) << " bytes\n";
    std::cout << "Current index:         " << v.index() << "\n\n";

    // Change the state to see how the payload changes
    v = 3.14159;
    std::cout << "After setting to double, index: " << v.index() << "\n\n";

    // Raw byte dump of the variant object itself
    std::cout << "--- Raw Memory Dump (Hex) ---\n";
    auto bytes = std::as_bytes(std::span{&v, 1});
    
    int i = 0;
    for (auto b : bytes) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(b) << " ";
        if (++i % 16 == 0) std::cout << "\n";
    }
    std::cout << std::dec << "\n\n";
}

int main() {
    inspectVariant();
    return 0;
}