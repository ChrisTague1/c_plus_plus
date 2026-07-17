#pragma once

#include <iostream>

#include "json.h"
#include "token.h"

enum class ParserState {
    ExpectingString,
    ExpectingColon,
    ExpectingValue,
    ExpectingCommaOrObjectClose,
    ExpectingCommaOrArrayClose,
};

std::ostream& operator<<(std::ostream& os, ParserState state);

JsonValue parser(TokenStream& tokens);
