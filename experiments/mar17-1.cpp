#include <iostream>
#include <string>
#include <iomanip>
#include <functional>

std::string rtrim1(const std::string& str) {
    std::string s(str);

    std::cout << *s.begin() << "\n";
    std::cout << (void*)&(*s.begin()) << "\n";
    std::cout << *s.rbegin() << "\n";
    std::cout << *s.rend() << "\n";

    s.erase(1, 3);

    return s;
}

std::string rtrim2(const std::string& str) {
    std::string s1(str);
    std::string s2(str);

    s1.erase(
        s1.begin(),
        s1.end()
    );

    std::cout << "S1 = " << std::quoted(s1) << "\n";

    s2.erase(
        std::find_if(s2.rbegin(), s2.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
        s2.end() 
    );
    // ^^^ the above weird stuff is deprecated, don't use. I think removed in c++20?

    return s2;
}

std::string rtrim3(const std::string& str) {
    std::string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), // rbegin is a reverse iterator, base makes it point in the right direction
        // this is determined using those weird tag things
        s.end()
    );

    return s;
}

int main() {
    std::string s = "Hello, world!";
    std::cout << s << std::endl;
    std::cout << rtrim1(s) << std::endl;

    std::cout << "\nnext example\n\n\n\n";

    s = "Hello, world!      ";
    std::cout << std::quoted(s) << std::endl;
    std::cout << std::quoted(rtrim2(s)) << std::endl;

    std::cout << "\nnext example\n\n\n\n";

    std::cout << std::quoted(s) << std::endl;
    std::cout << std::quoted(rtrim3(s)) << std::endl;

    return 0;
}