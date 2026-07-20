#include "lexer.h"

#include <sstream>
#include <format>

std::ostream& operator<<(std::ostream& os, LexerState state) {
    switch (state) {
        case LexerState::Default: return os << "Default";
        case LexerState::InString: return os << "InString";
        case LexerState::InEscape: return os << "InEscape";
        case LexerState::InNumber: return os << "InNumber";
        case LexerState::InKeyword: return os << "InKeyword";
        case LexerState::InNumberAfterPeriod: return os << "InNumberAfterPeriod";
    }
    return os;
}


TokenStream lexer(std::istream& in) {
    std::vector<Token> tokens;
    LexerState state = LexerState::Default;

    const std::string true_keyword = "true";
    const std::string false_keyword = "false";
    const std::string null_keyword = "null";
    size_t keyword_index = 0;
    Keyword keyword;
    std::stringstream ss;

    char c;
    while (in.get(c)) {
        switch (state) {
            case LexerState::InEscape:
                switch (c) {
                    case 'n':
                        ss << '\n';
                        state = LexerState::InString;
                        break;
                    case 't':
                        ss << '\t';
                        state = LexerState::InString;
                        break;
                    case '\\':
                        ss << '\\';
                        state = LexerState::InString;
                        break;
                    case '"':
                        ss << '"';
                        state = LexerState::InString;
                        break;
                    default:
                        throw LexerIllegalCharacter(std::format("Illegal escape character: {}", c));
                }
                break;
            case LexerState::InNumberAfterPeriod: {
                ss << c;
                int peeked = in.peek();
                if (peeked < static_cast<int>('0') || peeked > static_cast<int>('9')) {
                    float data = stof(ss.str());
                    ss = std::stringstream{};
                    tokens.push_back(Token(TokenTypes::Number, data));
                    state = LexerState::Default;
                }
                break;
            }
            case LexerState::InNumber: {
                ss << c;
                int peeked = in.peek();
                if (peeked == static_cast<int>('.')) {
                    state = LexerState::InNumberAfterPeriod;
                } else if (peeked < static_cast<int>('0') ||
                           peeked > static_cast<int>('9')) {
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
                        if (peeked == static_cast<int>('.')) {
                            state = LexerState::InNumberAfterPeriod;
                        } else if (peeked < static_cast<int>('0') ||
                                   peeked > static_cast<int>('9')) {
                            float data = stof(ss.str());
                            ss = std::stringstream{};
                            tokens.push_back(Token(TokenTypes::Number, data));
                            state = LexerState::Default;
                        }
                        break;
                    }
                    default:
                        throw LexerIllegalCharacter(std::format("Found an illegal character: '{}'", c));
                }
                break;
            case LexerState::InKeyword: {
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
                    state = LexerState::Default;
                    break;
                }

                if (++keyword_index == comparing.size()) {
                    tokens.push_back(token_type);
                    state = LexerState::Default;
                }

                break;
            }
        }
    }

    return TokenStream(tokens);
}

LexerIllegalCharacter::LexerIllegalCharacter(std::string msg,
                                             std::source_location loc)
    : message(std::move(msg)), location(loc) {}
[[nodiscard]] const char* LexerIllegalCharacter::what() const noexcept {
    return message.c_str();
}
const std::source_location& LexerIllegalCharacter::where() const noexcept {
    return location;
}
