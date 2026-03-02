#include <iostream>
#include <vector>

struct LargeThing {
    int data[10]; // int = 4 bytes, 10 ints is 40 bytes
};

int main() {
    std::vector<LargeThing> v(3);

    std::cout << v.data() << "\n";
    std::cout << v.data() + 1 << "\n";

    return 0;
}