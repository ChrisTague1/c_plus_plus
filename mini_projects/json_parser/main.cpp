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

        JsonValue value = parser(tokens);

        std::cout << value << std::endl;
    }
    catch (const LexerIllegalCharacter& e) {
        std::cerr << "Illegal character: " << e.what() << "\n";
    }
    catch (const ParserIllegalToken& e) {
        std::cerr << "Illegal token: " << e.what() << "\n";
    }

    return 0;
}