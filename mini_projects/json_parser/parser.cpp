#include "parser.h"

#include <format>

std::ostream& operator<<(std::ostream& os, ParserState state) {
    switch (state) {
        case ParserState::ExpectingColon: return os << "ExpectingColon";
        case ParserState::ExpectingString: return os << "ExpectingString";
        case ParserState::ExpectingValue: return os << "ExpectingValue";
        case ParserState::ExpectingCommaOrObjectClose: return os << "ExpectingCommaOrObjectClose";
        case ParserState::ExpectingCommaOrArrayClose: return os << "ExpectingCommaOrArrayClose";
        case ParserState::ExpectingStringOrCloseObject: return os << "ExpectingStringOrCloseObject";
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

        switch (state) {
            case ParserState::ExpectingStringOrCloseObject:
                switch (token.type) {
                    case TokenTypes::String: 
                        temp_key = std::get<std::string>(token.data);
                        state = ParserState::ExpectingColon;
                        break;
                    case TokenTypes::CloseObject: return map;
                    default:
                        throw ParserIllegalToken(std::format("Expected a string or object close, found {}", token.type));
                }
                break;
            case ParserState::ExpectingCommaOrArrayClose:
                switch (token.type) {
                    case TokenTypes::CloseArray: return array;
                    case TokenTypes::Comma:
                        array->values.push_back(parser(tokens));
                        break;
                    default:
                        throw ParserIllegalToken(std::format("Expected a comma or closed array, found {}", token.type));
                }
                break;
            case ParserState::ExpectingCommaOrObjectClose:
                switch (token.type) {
                    case TokenTypes::CloseObject: return map;
                    case TokenTypes::Comma:
                        state = ParserState::ExpectingString;
                        break;
                    default:
                        throw ParserIllegalToken(std::format("Expected a comma or closed object, found {}", token.type));
                }
                break;
            case ParserState::ExpectingColon:
                switch (token.type) {
                    case TokenTypes::Colon:
                        state = ParserState::ExpectingCommaOrObjectClose;
                        map->values[temp_key] = parser(tokens); // is this copying it, I ideally want it passed by reference
                        break;
                    default:
                        throw ParserIllegalToken(std::format("Expected a colon, found {}", token.type));
                }
                break;
            case ParserState::ExpectingString:
                switch (token.type) {
                    case TokenTypes::String:
                        temp_key = std::get<std::string>(token.data);
                        state = ParserState::ExpectingColon;
                        break;
                    default:
                        throw ParserIllegalToken(std::format("Expected a string for an object key, found {}", token.type));
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
                        state = ParserState::ExpectingStringOrCloseObject;
                        break;
                    case TokenTypes::OpenArray:
                        state = ParserState::ExpectingCommaOrArrayClose;
                        array->values.push_back(parser(tokens));
                        break;
                    default:
                        throw ParserIllegalToken(std::format("Expected a JSON value or start of object or array, found {}", token.type));
                }
                break;
        }
    }

    throw ParserIllegalToken("Failed to build a valid JSON value");
}

ParserIllegalToken::ParserIllegalToken(std::string msg,
                                       std::source_location loc)
    : message(msg), location(loc) {}
[[nodiscard]] const char* ParserIllegalToken::what() const noexcept {
    return message.c_str();
}
const std::source_location& ParserIllegalToken::where() const noexcept {
    return location;
}