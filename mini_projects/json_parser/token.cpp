#include "token.h"
#include <iostream>

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
        case TokenTypes::Illegal: return os << "Illegal";
    }
    return os;
}

Token::Token(TokenTypes type_, int line_, int position_)
    : data(), type(type_), line(line_), position(position_) {}
Token::Token(TokenTypes type_, int line_, int position_, std::string data)
    : data(std::move(data)), type(type_), line(line_), position(position_) {
    // take a closer look at exactly what happens with the memory here
}
Token::Token(TokenTypes type_, int line_, int position_, float data)
    : data(data), type(type_), line(line_), position(position_) {}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    switch (token.type) {
        case TokenTypes::String: return os << token.type << "(" << std::get<std::string>(token.data) << ")";
        case TokenTypes::Number: return os << token.type << "(" << std::get<float>(token.data) << ")";
        default:
            break;
    }

    return os << token.type;
}

TokenStream::TokenStream(std::vector<Token> tokens): tokens_(std::move(tokens)) {}
const Token& TokenStream::peek() const {
    return tokens_[pos_];
}
const Token& TokenStream::next() {
    return tokens_[pos_++];
}
bool TokenStream::has_next() const {
    return pos_ < tokens_.size();
}

std::ostream& operator<<(std::ostream& os, const TokenStream& ts) {
    bool first = true;
    for (const auto& token : ts.tokens_) {
        if (!first) os << ", ";
        first = false;
        os << token;
    }

    return os;
}