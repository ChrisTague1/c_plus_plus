#pragma once

#include <ostream>
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

class Token {
   public:
    TokenTypes type;
    std::variant<std::monostate, std::string, float> data;

    Token(TokenTypes type_);  // if you put explicit here things break
    explicit Token(TokenTypes type_, std::string data);
    explicit Token(TokenTypes type_, float data);

    friend std::ostream& operator<<(std::ostream& os, const Token& token);
};