#include <iostream>
#include <fstream>

#include "token.h"
#include "lexer.h"

enum class ParserState {
    ExpectingString, // for when inside of an object
    ExpectingColon, // after object key string
    ExpectingValue, // here I can get:
                    // - string
                    // - number
                    // - true
                    // - false
                    // - null
                    // - OpenObject
                    // - OpenArray
};

bool parser(const std::vector<Token>& tokens) {
    ParserState state = ParserState::ExpectingValue;

    for (const auto& token : tokens) {
        std::cout << token << "\n";

        switch (state) {
            case ParserState::ExpectingValue:
                

                break;
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    std::vector<Token> tokens;

    std::ifstream file(argv[1]);

    tokens = lexer(file);

    parser(tokens);

    return 0;
}