#pragma once

#include <ostream>

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
    std::string s_data;  // use a variant here instead
    float n_data;

    Token(TokenTypes type_);  // if you put explicit here things break
    explicit Token(TokenTypes type_, std::string data);
    explicit Token(TokenTypes type_, float data);

    friend std::ostream& operator<<(std::ostream& os, const Token& token);
};