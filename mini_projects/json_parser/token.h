#pragma once

#include <format>
#include <ostream>
#include <sstream>
#include <variant>

enum class TokenTypes {
    OpenObject,   // {
    CloseObject,  // }
    OpenArray,    // [
    CloseArray,   // ]
    Comma,        // ,
    Colon,        // :
    Null,
    True,
    False,
    String,
    Number,
    Illegal,
};

std::ostream& operator<<(std::ostream& os, TokenTypes type);

template <>
struct std::formatter<TokenTypes> : std::formatter<std::string> {
    auto format(TokenTypes type, std::format_context& ctx) const {
        std::ostringstream ss;
        ss << type;
        return std::formatter<std::string>::format(ss.str(), ctx);
    }
};

class Token {
   public:
    std::variant<std::monostate, std::string, float> data;
    TokenTypes type;
    int line;
    int position;

    explicit Token(TokenTypes type_, int line_, int position_);
    explicit Token(TokenTypes type_, int line_, int position_, std::string data);
    explicit Token(TokenTypes type_, int line_, int position_, float data);

    friend std::ostream& operator<<(std::ostream& os, const Token& token);
};

class TokenStream {
    std::vector<Token> tokens_;
    std::size_t pos_ = 0;

   public:
    explicit TokenStream(std::vector<Token> tokens);
    const Token& peek() const;
    const Token& next();
    bool has_next() const;

    friend std::ostream& operator<<(std::ostream& os, const TokenStream& ts);
};
