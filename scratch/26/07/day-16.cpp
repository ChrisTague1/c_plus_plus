#include <iostream>
#include <variant>
#include <string>

using Value = std::variant<std::monostate, int, std::string>;

template<class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

int main() {
    Value value;

    std::cout << value.index() << std::endl;

    if (std::holds_alternative<int>(value)) {
        std::cout << "The value is an int" << std::endl;
    }
    if (std::holds_alternative<std::monostate>(value)) {
        std::cout << "The value is a monostate" << std::endl;
    }

    if (false) { // throws an error
        int x = std::get<int>(value);

        std::cout << "x = " << x << std::endl;
    }

    value = 25;

    if (int* i = std::get_if<int>(&value)) {
        std::cout << "i = " << *i << std::endl;
    }

    // value.emplace<std::monostate>();
    // value.emplace<int>(6);
    value.emplace<std::string>(5, 'x');

    std::visit([](const auto& x) {
        // std::cout << typeid(x).name() << std::endl; // prints mangled type name

        using T = std::decay_t<decltype(x)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            // evaluate if you should enter this if statement at compile time
            // there is no branching logic at runtime
            std::cout << "x = Empty\n";
        } else {
            std::cout << "x = " << x << "\n";
        }
    }, value);

    return 0;
}