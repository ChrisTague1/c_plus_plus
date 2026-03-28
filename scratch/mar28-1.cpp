#include <iostream>

/*
clang++ -std=c++20 -S -o output.s scratch/mar28-1.cpp
clang++ -std=c++20 -S -O0 -o output.s scratch/mar28-1.cpp
clang++ -std=c++20 -S -O2 -o output.s scratch/mar28-1.cpp

clang++ -std=c++20 -S -O0 -o - scratch/mar28-1.cpp 2>/dev/null | c++filt > output.s
clang++ -std=c++20 -S -O2 -o - scratch/mar28-1.cpp 2>/dev/null | c++filt > output.s
*/

int add(int a, int b) {
    return a + b;
}

int main() {
    int x = 10;
    int y = 32;
    int z = add(x, y);
    std::cout << z << std::endl;

    return 0;
}