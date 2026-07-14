#include <iostream>
#include <fstream>

class Reader {
   public:
    explicit Reader(std::istream& in) : in_(in) {}

    int peek() {
        in_.peek();
    }

    int get() {
        in_.get();
    }

   private:
    std::istream& in_;
};

int main() {
    std::ifstream file("experiments/files/file.txt");

    char c;
    while (file.get(c)) {
        std::cout << c;
    }

    return 0;
}