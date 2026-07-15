#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>

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

std::ostream& operator<<(std::ostream& os, TokenTypes type) {
    switch (type) {
        case TokenTypes::OpenObject: return os << "OpenObject";
        case TokenTypes::CloseObject: return os << "CloseObject";
        case TokenTypes::OpenArray: return os << "OpenArray";
        case TokenTypes::CloseArray: return os << "CloseArray";
        case TokenTypes::Comma: return os << "Comma";
        case TokenTypes::Colon: return os << "Colon";
        case TokenTypes::Null: return os << "Null";
        case TokenTypes::True: return os << "True";
        case TokenTypes::False: return os << "False";
        case TokenTypes::String: return os << "String";
        case TokenTypes::Number: return os << "Number";
    }
    return os;
}

class Token {
    public:
        TokenTypes type;
        std::string s_data; // use a variant here instead

        Token(TokenTypes type_): type(type_) {} // if I put explicit here some shit breaks. The fact that this works is cursed
        explicit Token(TokenTypes type_, std::string data): type(type_), s_data(std::move(data)) {
            // take a closer look at what exactly is happening with the memory around data, check the caller too
        }

        friend std::ostream& operator<<(std::ostream& os, const Token& token) {
            switch (token.type) {
                case TokenTypes::String: return os << token.type << "(" << token.s_data << ")";
            }

            return os << token.type;
        }
};

enum class LexerState {
    Default,
    InString,
    InEscape,
    InNumber,
    InKeyword,
};

std::ostream& operator<<(std::ostream& os, LexerState state) {
    switch (state) {
        case LexerState::Default: return os << "Default";
        case LexerState::InString: return os << "InString";
        case LexerState::InEscape: return os << "InEscape";
        case LexerState::InNumber: return os << "InNumber";
        case LexerState::InKeyword: return os << "InKeyword";
    }
    return os;
}

enum class Keyword {
    True,
    False,
    Null,
};

std::vector<Token> lexer(std::istream& in) {
    std::vector<Token> tokens;
    LexerState state = LexerState::Default;

    const std::string true_keyword = "true";
    const std::string false_keyword = "false";
    const std::string null_keyword = "null";
    char keyword_index = 0;
    Keyword keyword;
    std::stringstream ss;

    char c;
    while (in.get(c)) {
        // std::cout << c << ", state is " << state << "\n";

        switch (state) {
            case LexerState::InString:
                switch (c) {
                    case '\\':
                        state = LexerState::InEscape;
                        break;
                    case '"':
                        state = LexerState::Default;
                        tokens.push_back(Token(TokenTypes::String, ss.str()));
                        ss = std::stringstream{};
                        break;
                }
                ss << c;
                break;
            case LexerState::Default:
                switch (c) {
                    // TODOAI Are there more whitespaces that I need to worry
                    // about? Is this os dependant, is there an is_whitespace
                    // function?
                    case '\n':
                    case '\t':
                    case ' ':
                        break;
                    case '{':
                        tokens.push_back(TokenTypes::OpenObject);
                        break;
                    case '}':
                        tokens.push_back(TokenTypes::CloseObject);
                        break;
                    case '[':
                        tokens.push_back(TokenTypes::OpenArray);
                        break;
                    case ']':
                        tokens.push_back(TokenTypes::CloseArray);
                        break;
                    case ',':
                        tokens.push_back(TokenTypes::Comma);
                        break;
                    case ':':
                        tokens.push_back(TokenTypes::Colon);
                        break;
                    case 't':
                        keyword = Keyword::True;
                        state = LexerState::InKeyword;
                        keyword_index = 1;
                        break;
                    case 'f':
                        keyword = Keyword::False;
                        state = LexerState::InKeyword;
                        keyword_index = 1;
                        break;
                    case 'n':
                        keyword = Keyword::Null;
                        state = LexerState::InKeyword;
                        keyword_index = 1;
                        break;
                    case '"':
                        state = LexerState::InString;
                        break;
                }
                break;
            case LexerState::InKeyword:
                std::string comparing;
                TokenTypes token_type;

                switch (keyword) {
                    case Keyword::True:
                        comparing = true_keyword;
                        token_type = TokenTypes::True;
                        break;
                    case Keyword::False:
                        comparing = false_keyword;
                        token_type = TokenTypes::False;
                        break;
                    case Keyword::Null:
                        comparing = null_keyword;
                        token_type = TokenTypes::Null;
                        break;
                }

                if (c != comparing[keyword_index]) {
                    // throw an error
                    state = LexerState::Default;
                    break;
                }

                if (++keyword_index == comparing.size()) {
                    // This only works because there is no overlap,
                    // if no and noop were two different keywords we'd need to
                    // look ahead
                    tokens.push_back(token_type);
                    // TODO we technically need to look ahead for this, trueahhh
                    // isn't valid
                    state = LexerState::Default;
                }

                break;
        }
    }

    std::cout << "\n";

    return tokens;
}

int main(int argc, char* argv[]) {
    // std::cout << static_cast<int>('2') << std::endl;
    std::vector<Token> tokens;

    // std::ifstream file(argv[1]);

    // tokens = lexer(file);

    // for (const auto& token : tokens) {
    //     std::cout << token << "\n";
    // }

    std::istringstream stream("{\"hey\"}");
    tokens = lexer(stream);

    for (const auto& token : tokens) {
        std::cout << token << "\n";
    }

    return 0;
}