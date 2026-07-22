#include "lexer.h"

#include <format>
#include <charconv>

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

    size_t i;

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

    float data;

    std::from_chars(s.data(), s.data() + s.size(), data);

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

    throw LexerIllegalCharacter("Found unexpected EOF while parsing a string");
}

TokenStream lexer(std::istream& in) {
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
            case ' ':
            case '\n':
            case '\t':
                break;
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
            default:
                throw LexerIllegalCharacter(std::format("Found illegal character {} at line {} position {}", c, position.line, position.position));
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
