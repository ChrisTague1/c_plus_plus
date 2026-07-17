#include "parser.h"

std::ostream& operator<<(std::ostream& os, ParserState state) {
    switch (state) {
        case ParserState::ExpectingColon: return os << "ExpectingColon";
        case ParserState::ExpectingString: return os << "ExpectingString";
        case ParserState::ExpectingValue: return os << "ExpectingValue";
        case ParserState::ExpectingCommaOrObjectClose: return os << "ExpectingCommaOrObjectClose";
        case ParserState::ExpectingCommaOrArrayClose: return os << "ExpectingCommaOrArrayClose";
    }
    return os;
}

JsonValue parser(TokenStream& tokens) {
    ParserState state = ParserState::ExpectingValue;

    std::unique_ptr<JsonObject> map = std::make_unique<JsonObject>();
    std::unique_ptr<JsonArray> array = std::make_unique<JsonArray>();
    std::string temp_key;

    while (tokens.has_next()) {
        Token token = tokens.next();
        // std::cout << token << ", State: " << state << std::endl;

        switch (state) {
            case ParserState::ExpectingCommaOrArrayClose:
                switch (token.type) {
                    case TokenTypes::CloseArray: return array;
                    case TokenTypes::Comma:
                        array->values.push_back(parser(tokens));
                        break;
                }
                break;
            case ParserState::ExpectingCommaOrObjectClose:
                switch (token.type) {
                    case TokenTypes::CloseObject: return map;
                    case TokenTypes::Comma:
                        state = ParserState::ExpectingString;
                        break;
                }
            case ParserState::ExpectingColon:
                switch (token.type) {
                    case TokenTypes::Colon:
                        state = ParserState::ExpectingCommaOrObjectClose;
                        map->values[temp_key] = parser(tokens); // is this copying it, I ideally want it passed by reference
                        break;
                }
            case ParserState::ExpectingString:
                switch (token.type) {
                    case TokenTypes::String:
                        temp_key = std::get<std::string>(token.data);
                        state = ParserState::ExpectingColon;
                        break;
                }
                break;
            case ParserState::ExpectingValue:
                switch (token.type) {
                    case TokenTypes::String: return std::get<std::string>(token.data);
                    case TokenTypes::Number: return std::get<float>(token.data);
                    case TokenTypes::Null: return nullptr;
                    case TokenTypes::True: return true;
                    case TokenTypes::False: return false;
                    case TokenTypes::OpenObject:
                        state = ParserState::ExpectingString;
                        break;
                    case TokenTypes::OpenArray:
                        state = ParserState::ExpectingCommaOrArrayClose;
                        array->values.push_back(parser(tokens));
                        break;
                }
                break;
        }
    }

    return true;
}