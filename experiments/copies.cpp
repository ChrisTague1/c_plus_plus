#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

void print_thing(std::vector<int>& vec) { // without & it gets copied
    std::cout << "data ptr: " << vec.data() << std::endl;
    std::cout << "size:     " << vec.size() << std::endl;
    std::cout << "capacity: " << vec.capacity() << std::endl;
}

int main() {
    std::vector v{1, 2, 3};

    std::cout << "data ptr: " << v.data() << std::endl;
    std::cout << "size:     " << v.size() << std::endl;
    std::cout << "capacity: " << v.capacity() << std::endl;

    print_thing(v);

    return 0;
}