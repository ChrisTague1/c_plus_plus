#pragma once

#include <iostream>
#include <vector>
#include <exception>
#include <source_location>

#include "token.h"

enum class LexerState {
    Default,
    InString,
    InEscape,
    InNumber,
    InKeyword,
};

std::ostream& operator<<(std::ostream& os, LexerState state);

enum class Keyword {
    True,
    False,
    Null,
};

TokenStream lexer(std::istream& in);

class LexerIllegalCharacter : public std::exception {
    std::string message;
    std::source_location location;

   public:
    LexerIllegalCharacter(
        std::string msg,
        std::source_location loc = std::source_location::current());
    [[nodiscard]] const char* what() const noexcept override;
    const std::source_location& where() const noexcept;
};
