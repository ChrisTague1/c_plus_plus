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

struct Position {
    int line;
    int position;
};

Token parse_keyword(char c, std::istream& in, Position& position) {
    constexpr std::string true_keyword = "true";
    constexpr std::string false_keyword = "false";
    constexpr std::string null_keyword = "null";

    int pos = 0;

    std::string keyword;
    TokenTypes keyword_type;

    switch (c) {
        case 'n':
            keyword = null_keyword;
            keyword_type = TokenTypes::Null;
            break;
        case 't':
            keyword = true_keyword;
            keyword_type = TokenTypes::True;
            break;
        case 'f':
            keyword = false_keyword;
            keyword_type = TokenTypes::False;
            break;
    }

    int i;

    for (i = 1; i < keyword.size() && in.get(c) && c == keyword[i]; i++) {}

    if (i == keyword.size()) {
        position.position += keyword.size() - 1;
        return Token(keyword_type, position.line, position.position);
    }

    throw LexerIllegalCharacter(
        std::format("Found illegal character {} at line {} position {} when "
                    "searching for {}",
                    c, position.line, position.position + i - 1, keyword));
}

Token parse_number(char c, std::istream& in, Position& position) {
    // TODO should support scientific notation as well
    std::string s;

    bool seen_period = false;

    while (true) {
        s.push_back(c);
        int next = in.peek();

        if (
            (seen_period || next != static_cast<int>('.')) &&
            (next < static_cast<int>('0') || next > static_cast<int>('9'))
        ) {
            break;
        }

        if (next == static_cast<int>('.')) seen_period = true;

        if (in.get(c)) {
            position.position++;
        } else {
            break;
        }
    }

    float data = stof(s);
    return Token(TokenTypes::Number, position.line, position.position, data);
}

char parse_escape(std::istream& in, Position& position) {
    char c;
    
    if (!in.get(c)) {
        throw LexerIllegalCharacter("Found EOF when expecting escape character");
    }

    position.position++;

    switch (c) {
        case 'n': return '\n';
        case '\\': return '\\';
        case '"': return '"';
        case 't': return '\t';
        default:
            throw LexerIllegalCharacter(
                std::format("Expected valid escape character, found {} at line "
                            "{} position {}",
                            c, position.line, position.position));
    }
}

Token parse_string(std::istream& in, Position& position) {
    std::string s;

    char c;
    while (in.get(c)) {
        position.position++;
        switch (c) {
            case '"': return Token(TokenTypes::String, position.line, position.position, s);
            case '\\':
                s.push_back(parse_escape(in, position));
                break;
            default:
                s.push_back(c);
        }
    }
}

TokenStream lexer2(std::istream& in) {
    std::vector<Token> tokens;

    Position position {};

    char c;
    while (in.get(c)) {
        if (c == '\n') {
            position.line++;
            position.position = 0;
        } else {
            position.position++;
        }

        switch (c) {
            case '{':
                tokens.push_back(Token(TokenTypes::OpenObject, position.line, position.position));
                break;
            case '}':
                tokens.push_back(Token(TokenTypes::CloseObject, position.line, position.position));
                break;
            case '[':
                tokens.push_back(Token(TokenTypes::OpenArray, position.line, position.position));
                break;
            case ']':
                tokens.push_back(Token(TokenTypes::CloseArray, position.line, position.position));
                break;
            case ',':
                tokens.push_back(Token(TokenTypes::Comma, position.line, position.position));
                break;
            case ':':
                tokens.push_back(Token(TokenTypes::Colon, position.line, position.position));
                break;
            case 't':
            case 'f':
            case 'n':
                tokens.push_back(parse_keyword(c, in, position));
                break;
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                tokens.push_back(parse_number(c, in, position));
                break;
            case '"':
                tokens.push_back(parse_string(in, position));
                break;
        }
    }

    return TokenStream(tokens);
}

TokenStream lexer(std::istream& in) {
    std::vector<Token> tokens;
    LexerState state = LexerState::Default;

    constexpr std::string true_keyword = "true";
    constexpr std::string false_keyword = "false";
    constexpr std::string null_keyword = "null";
    size_t keyword_index = 0;
    Keyword keyword;
    std::stringstream ss;

    int line = 1;
    int position = 0;

    char c;
    while (in.get(c)) {
        if (c == '\n') {
            line++;
            position = 0;
        } else {
            position++;
        }

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
                    tokens.push_back(Token(TokenTypes::Number, line, position, data));
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
                    tokens.push_back(Token(TokenTypes::Number, line, position, data));
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
                        tokens.push_back(Token(TokenTypes::String, line, position, ss.str()));
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
                        tokens.push_back(Token(TokenTypes::OpenObject, line, position));
                        break;
                    case '}':
                        tokens.push_back(Token(TokenTypes::CloseObject, line, position));
                        break;
                    case '[':
                        tokens.push_back(Token(TokenTypes::OpenArray, line, position));
                        break;
                    case ']':
                        tokens.push_back(Token(TokenTypes::CloseArray, line, position));
                        break;
                    case ',':
                        tokens.push_back(Token(TokenTypes::Comma, line, position));
                        break;
                    case ':':
                        tokens.push_back(Token(TokenTypes::Colon, line, position));
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
                    case '-': {
                        int peeked = in.peek();

                        if (peeked < static_cast<int>('0') || peeked > static_cast<int>('9')) {
                            throw LexerIllegalCharacter(std::format("Illegal character: - must be followed by a number to be made negative."));
                        }

                        ss << c;

                        state = LexerState::InNumber;

                        break;
                    }
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

                        if (
                            peeked != static_cast<int>('.') && (
                                peeked < static_cast<int>('0') ||
                                peeked > static_cast<int>('9')
                            )
                        ) {
                            float data = stof(ss.str());
                            ss = std::stringstream{};
                            tokens.push_back(Token(TokenTypes::Number, line, position, data));
                        } else {
                            state = LexerState::InNumber;
                        }

                        break;
                    }
                    default:
                        throw LexerIllegalCharacter(std::format("Found an illegal character: '{}' at line {} position {}", c, line, position));
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
                    tokens.push_back(Token(token_type, line, position));
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
