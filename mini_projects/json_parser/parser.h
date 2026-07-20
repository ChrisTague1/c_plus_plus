#pragma once

#include <iostream>
#include <exception>
#include <source_location>

#include "json.h"
#include "token.h"

enum class ParserState {
    ExpectingStringOrCloseObject, // when you see { you can close right away
    ExpectingString, // if you see a comma then you must have another key
    ExpectingColon,
    ExpectingValue,
    ExpectingCommaOrObjectClose,
    ExpectingCommaOrArrayClose,
};

std::ostream& operator<<(std::ostream& os, ParserState state);

JsonValue parser(TokenStream& tokens);

class ParserIllegalToken : public std::exception {
    std::string message;
    std::source_location location;

public:
    // investigate exactly what source_location is doing, and why the default has to go here
    ParserIllegalToken(std::string msg, std::source_location loc = std::source_location::current());
    [[nodiscard]] const char* what() const noexcept override;
    const std::source_location& where() const noexcept;
};
