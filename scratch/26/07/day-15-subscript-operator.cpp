#include <iostream>

class MyClass {
    public:
        int operator[](std::string data) {
            std::cout << "[] operator was called with " << data << std::endl;

            return 0;
        }
};

int main() {
    MyClass my_class;

    my_class["what is up"];

    return 0;
}