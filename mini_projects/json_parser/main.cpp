#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <sstream>

#include "token.h"
#include "lexer.h"
#include "json.h"
#include "parser.h"

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);

    // std::stringstream ss("\"hey\"");

    try {
        TokenStream tokens = lexer(file);

        std::cout << tokens << std::endl;

        // JsonValue value = parser(tokens);

        // std::cout << value << std::endl;
    }
    catch (const LexerIllegalCharacter& e) {
        std::cerr << "Illegal character: " << e.what() << "\n";
    }

    return 0;
}