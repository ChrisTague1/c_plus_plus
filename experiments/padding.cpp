#include <iostream>

struct MixedData {
    /**
     * 1 + 4 + 1 = 6
     * 
     * BUT
     * 
     * The CPU doesn't grap data 1 byte at a time, it grabs things in chunks depending on hardware
     * (This is why Mac is so goated, soldered RAM = HUGE chunks)
     * 
     * Lets assume things are padded to 4 bytes
     * 
     * The compiler doesn't want to spread things like an int across more chunks than it 
     * needs to padding will be added
     * 
     * a = byte 0, then 3 bytes of padding
     * b = bytes 4, 5, 6, 7
     * c = byte 8, then 9, 10, 11 because of padding
     * 
     * So this layout is 12 total bytes, not 6
     */
    char a; // 1 byte
    int b;  // 4 bytes
    char c; // 1 byte
};

struct OptimizedData {
    /**
     * Generally lay things out big to small
     * 
     * b = 4 bytes
     * a = 1 byte
     * c can be grouped with a
     * two bytes of padding
     * 
     * b = 0, 1, 2, 3
     * a = 4
     * c = 5
     * padding = 6, 7
     */
    int b;
    char a;
    char c;
};

int main() {
    std::cout << "--- MixedData (The 'Wasteful' one) ---" << std::endl;
    std::cout << "Total Size: " << sizeof(MixedData) << " bytes" << std::endl;
    std::cout << "Alignment : " << alignof(MixedData) << " bytes" << std::endl;
    std::cout << "Offset of a: " << offsetof(MixedData, a) << std::endl;
    std::cout << "Offset of b: " << offsetof(MixedData, b) << " (3 bytes padding before this)" << std::endl;
    std::cout << "Offset of c: " << offsetof(MixedData, c) << " (Ends at byte 9, plus 3 bytes trailing padding)" << std::endl;

    std::cout << "\n--- OptimizedData (The 'Tight' one) ---" << std::endl;
    std::cout << "Total Size: " << sizeof(OptimizedData) << " bytes" << std::endl;
    std::cout << "Alignment : " << alignof(OptimizedData) << " bytes" << std::endl;
    std::cout << "Offset of b: " << offsetof(OptimizedData, b) << std::endl;
    std::cout << "Offset of a: " << offsetof(OptimizedData, a) << std::endl;
    std::cout << "Offset of c: " << offsetof(OptimizedData, c) << " (Ends at byte 6, plus 2 bytes trailing padding)" << std::endl;

    // Proving the std::array stack allocation
    std::array<MixedData, 2> arr;
    std::cout << "\n--- Array Check ---" << std::endl;
    std::cout << "Distance between array elements: " 
              << (uintptr_t)&arr[1] - (uintptr_t)&arr[0] << " bytes" << std::endl;

    return 0;
}