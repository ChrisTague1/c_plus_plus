#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

int main() {
    auto greet = []() {
        std::cout << "Hey there\n";
    };
    greet();

    std::cout << "\n\n" << std::endl;

    auto add = [](int a, int b) {
        return a + b;
    };
    std::cout << "3 + 4 = " << add(3, 4) << std::endl;

    std::cout << "\n\n" << std::endl;

    // explicit return type
    auto divide = [](double a, double b) -> double {
        return a / b;
    };
    std::cout << divide(9, 4) << std::endl;

    std::cout << "\n\n" << std::endl;

    {
        int x = 10;
        int y = 20;

        auto byValue = [=]() {
            std::cout << "by value: x=" << x << " y=" << y << std::endl;
        };
        byValue();

        auto byRef = [&]() {
            x += 5;
            std::cout << "by ref: x=" << x << " y=" << y << std::endl;
        };
        byRef();

        auto specific = [x, &y]() {
            y += 100;
            std::cout << "specific: x=" << x << " y=" << y << std::endl;
        };
        specific();

        std::cout << "at the end: x=" << x << " y=" << y << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    {
        int count = 0;
        // creates a copy for THIS LAMBDA, NOT PER EXECUTION
        auto counter = [count]() mutable {
            ++count;
            std::cout << "internal count: " << count << std::endl;
        };

        counter();
        counter();
        counter();
        std::cout << "outer count: " << count << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    {
        std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};

        std::sort(nums.begin(), nums.end(), [](int a, int b) { return a < b; });
        for (int n : nums) std::cout << n << " ";
        std::cout << std::endl;

        std::vector<int> evens;
        std::copy_if(
            nums.begin(),
            nums.end(),
            std::back_inserter(evens),
            [](int n) { return n % 2 == 0; }
        );
        for (int n : evens) std::cout << n << " ";
        std::cout << std::endl;

        std::vector<int> squared(nums.size());
        std::transform(
            nums.begin(),
            nums.end(),
            squared.begin(), // since we know size we can just use the iterator instead of back_inserter
            [](int n) {
                return n * n;
            }
        );
        for (int n : squared) std::cout << n << " ";
        std::cout << std::endl;

        int sumOfSquares = std::accumulate(
            nums.begin(),
            nums.end(),
            0,
            [](int acc, int n) {
                return acc + n * n;
            }
        );
        std::cout << "sum of squares: " << sumOfSquares << std::endl;

        int threshold = 5;
        auto count = std::count_if(
            nums.begin(),
            nums.end(),
            [threshold](int n) { // if it isn't in there you can't use it
                return n > threshold;
            }
        );
        std::cout << "elements > " << threshold << ": " << count << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    {
        // doing this has a type erasure and a heap allocation so it is a little slower
        // stores a pointer to the function you want to call internally
        // cannot be optimized by compiler to be inlined or what not
        std::function<int(int, int)> op;

        op = [](int a, int b) {
            return a + b;
        };

        std::cout << "add: " << op(3, 4) << std::endl;

        op = [](int a, int b) {
            return a * b;
        };

        std::cout << "multiply: " << op(3, 4) << std::endl;

        auto applyTwice = [](std::function<int(int)> f, int x) {
            return f(f(x));
        };
        std::cout << "applied twice: " << applyTwice(
            [](int x) {
                return x * 2;
            },
            3
        ) << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    {
        auto print = [](auto val) {
            std::cout << val << std::endl;
        };
        print(42);
        print(3.14);
        print("hey there");

        auto add = [](auto a, auto b) {
            return a + b;
        };
        std::cout << add(1, 2) << std::endl;
        std::cout << add(1.2, 3.4) << std::endl;
        std::cout << add(std::string("hello "), std::string("there")) << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    {
        std::function<int(int)> fib = [&fib](int n) -> int {
            return n <= 1 ? n : fib(n - 1) + fib(n - 2);
        };
        
        // in c++23 auto fib = [](int auto self) { self(); } would work for this
        // our current implementation has the overhead of std::function

        std::cout << "fib of 7 = " << fib(7) << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    {
        int x = 14;
        auto lambda = [y = x * 2]() {
            std::cout << "y = " << y << std::endl;
        };
        lambda();

        auto ptr = std::make_unique<int>(99);
        auto owner = [p = std::move(ptr)]() {
            std::cout << "owned value: " << *p << std::endl;
        };
        owner();

        std::cout << "ptr after the move: " << (ptr ? "valid" : "null") << std::endl;
    }

    return 0;
}