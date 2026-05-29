#include <iostream>
#include <unordered_map>
#include <string>

using std::cout;
using std::endl;

class MyClass {
   public:
    std::string name;
    int age;
    int count;

    MyClass(std::string name, int age, int count)
        : name(name), age(age), count(count) {
        cout << "MyClass normal constructor " << name << "\n";
    }

    MyClass(MyClass&& other) noexcept {
        cout << "MyClass move constructor " << other.name << "\n";
        name = std::move(other.name);
        age = std::move(other.age);
        count = std::move(other.count);
    }

    MyClass(const MyClass& other)
        : name(other.name), age(other.age), count(other.count) {
        cout << "MyClass copy constructor " << other.name << "\n";
    }

    ~MyClass() {
        cout << "Destroying " << name << "\n";
    }
    
    MyClass& operator=(const MyClass&) = default;
    MyClass& operator=(MyClass&&) noexcept = default;

    friend std::ostream& operator<<(std::ostream& os, const MyClass& value) {
        os << "MyClass{name=\"" << value.name << "\", age=" << value.age
           << ", count=" << value.count << "}";
        return os;
    }
};

template <typename K, typename V>
void print_map(const std::unordered_map<K, V>& map) {
    for (const auto& [key, value] : map) {
        cout << key << ": " << value << "\n";
    }
    cout << "\n";
}

int main() {
    std::unordered_map<std::string, MyClass> ages;

    MyClass me{"chris", 24, 5};

    // ages.try_emplace("chris", "chris", 24, 2);
    ages.try_emplace("tague", std::move(me));

    auto [it, inserted] = ages.try_emplace("chris", "chris", 24, 5);

    if (inserted) {
        cout << "I inserted: " << it->second.count << "\n";
    } else {
        cout << "Already inserted: " << it->second.count << "\n";
    }

    print_map(ages);

    return 0;
}
