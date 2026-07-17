#pragma once

#include <iostream>
#include <vector>

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
