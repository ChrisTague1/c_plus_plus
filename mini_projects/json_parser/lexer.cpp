#include "lexer.h"

#include <sstream>

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
        switch (state) {
            case LexerState::InEscape:
                switch (c) {
                    case 'n':
                        ss << '\n';
                        break;
                    case 't':
                        ss << '\t';
                        break;
                }
                break;
            case LexerState::InNumber: {
                // this makes it feel like the while get should be inside of the states
                ss << c;
                int peeked = in.peek();
                if ( // if the next character is a . or between 0-9
                    peeked == static_cast<int>('.') ||
                    (
                        peeked >= static_cast<int>('0') &&
                        peeked <= static_cast<int>('9')
                    )
                ) {
                    state = LexerState::InNumber;
                } else {
                    float data = stof(ss.str());
                    ss = std::stringstream{};
                    tokens.push_back(Token(TokenTypes::Number, data));
                    state = LexerState::Default;
                }
                break;
            }
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
                    default:
                        ss << c;
                }
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
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        ss << c;

                        int peeked = in.peek();

                        if ( // if the next character is a . or between 0-9
                            peeked == static_cast<int>('.') ||
                            (
                                peeked >= static_cast<int>('0') &&
                                peeked <= static_cast<int>('9')
                            )
                        ) {
                            state = LexerState::InNumber;
                        } else {
                            float data = stof(ss.str());
                            ss = std::stringstream{};
                            tokens.push_back(Token(TokenTypes::Number, data));
                        }

                        break;
                    }
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
