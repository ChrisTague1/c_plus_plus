#include <iostream>
#include <string>

class SimpleStream {
   private:
    std::string data;
    size_t cursor = 0;
    bool failed = false;
   public:
    SimpleStream(std::string str) : data(str) {}

    SimpleStream& operator>>(std::string& output) {
        output.clear();

        while (cursor < data.length() && std::isspace(data[cursor])) {
            cursor++;
        }

        if (cursor >= data.length()) {
            failed = true;
            return *this;
        }

        while (cursor < data.length() && !std::isspace(data[cursor])) {
            output.push_back(data[cursor]);
            cursor++;
        }
        
        return *this;
    }

    explicit operator bool() const { // when you check if this is a bool (only if it is strictly/explicitly a bool check) call this, and this won't change the underlying class
        return !failed;
    }
};

int main() {
    SimpleStream ss("5 42 9");

    std::string a, b, c;

    ss >> a >> b >> c;

    std::cout << "A: " << a << ", B: " << b << ", C: " << c << "\n";

    if (!ss) std::cout << "Stream is now empty\n";
}
