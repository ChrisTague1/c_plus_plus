#include <iostream>
#include <fstream>

#include "token.h"
#include "lexer.h"

enum class ParserState {
    ExpectingObject,    
};

bool parser(const std::vector<Token>& tokens) {
    ParserState state = ParserState::ExpectingObject;

    for (const auto& token : tokens) {
        std::cout << token << "\n";

        switch (state) {
            case ParserState::ExpectingObject:
                // I can have either { or [ here

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