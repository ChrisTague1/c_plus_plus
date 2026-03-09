#include <iostream>
#include <cstddef>

#pragma pack(push, 1) // forced compact packing
struct PackedData {
    /**
     * This would be useful for:
     * - network protocols storing things in extremely specific places
     *   ex. first byte is a header or something
     * - file formats with specific placement as well
     */
    char a;
    int b;
    char c;
};
#pragma pack(pop)

int main() {
    std::cout << "--- PackedData (The 'Illegal' one) ---" << std::endl;
    std::cout << "Total Size: " << sizeof(PackedData) << " bytes" << std::endl;
    std::cout << "Alignment : " << alignof(PackedData) << " bytes" << std::endl;
    
    std::cout << "Offset of a: " << offsetof(PackedData, a) << std::endl;
    std::cout << "Offset of b: " << offsetof(PackedData, b) << " (Starts at byte 1!)" << std::endl;
    std::cout << "Offset of c: " << offsetof(PackedData, c) << " (Starts at byte 5!)" << std::endl;

    return 0;
}