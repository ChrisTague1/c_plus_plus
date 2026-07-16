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

/*
class... Ts means Ts is a "list" of the possible classes

The Overloaded struct should inherit from each of its members (struct Overloaded : Ts...)

Anywhere you see Ts... think of it getting expanded.

INCLUDING:
using Ts::operator()...;

would become:
using LambdaA::operator();
using LambdaB::operator();

You could make it overload other things too:
using Ts::f...;
using Ts::operator<<...;

the next part feels hacky to me...

It is saying "infer types from arguments"
*/

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

    std::visit(
        Overloaded{
            [](std::monostate) {
                std::cout << "empty";
            },
            [](int i) {
                std::cout << "int i = " << i << "\n";
            },
            [](const std::string& s) {
                std::cout << "string s = " << s << "\n";
            }
        },
        value
    );
    /*
    - Call Overloaded with three lambdas (classes with () operator) 
    - told it to infer types from arguments
    - with those types, grab all of their () methods
    */

    // or just inline it
    struct Eval {
        void operator()(std::monostate) {
            std::cout << "empty";
        }
        void operator()(int i) {
            std::cout << "int i = " << i << "\n";
        }
        void operator()(const std::string& s) {
            std::cout << "string s = " << s << "\n";
        }
    };
    std::visit(Eval{}, value);

    return 0;
}