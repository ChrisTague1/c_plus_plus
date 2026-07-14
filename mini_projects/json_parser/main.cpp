#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

enum class TokenTypes {
    OpenObject,  // {
    CloseObject, // }
    OpenArray,   // [
    CloseArray,  // ]
    Comma,       // ,
    Colon,       // :
    Null,
    True,
    False,
    String,
    Number,
};

class Token {
    public:
        TokenTypes type;
};

enum class LexerState {
    ExpectingKeyQuote, // Looking for " after {
    InQuotes, // Currently inside of "
    ExpectingObjectOrArray, // Looking for { or [
};

std::vector<Token> lexer(std::istream& in) {
    LexerState state = LexerState::ExpectingObjectOrArray;

    char c;
    while (in.get(c)) {
        std::cout << c;

        switch (state) {
            case LexerState::ExpectingObjectOrArray:
                switch (c) {
                    case '\n':
                    case '\t':
                    case ' ':
                    break;
                }
                break;
        }
    }

    return {};
}

int main(int argc, char* argv[]) {
    std::ifstream file(argv[1]);

    std::vector<Token> tokens = lexer(file);

    // char c;
    // while (file.get(c)) {
    //     switch (c) {
    //         case '{':
    //             std::cout << "Object starting\n";
    //             break;
    //         case '}':
    //             std::cout << "Object ending\n";
    //             break;
    //         case '[':
    //             std::cout << "Array starting\n";
    //             break;
    //         case ']':
    //             std::cout << "Array ending\n";
    //             break;
    //         case '"':
    //             std::cout << "Quote\n";
    //             break;
    //     }
    // }

    return 0;
}