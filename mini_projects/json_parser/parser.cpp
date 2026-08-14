#include "parser.h"

#include <format>
#include <vector>
#include <variant>
#include <string>
#include <optional>

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

enum class NextExpectedItem {
    ValueOrClose,
    CommaOrClose,
    Value,
    KeyOrClose,
    Colon,
    Key,
    Done
};

class Frame {
    std::optional<std::string> pending_key;
    NextExpectedItem next_expected_item;

   public:
    JsonValue value;
    Frame(JsonValue value, NextExpectedItem next)
        : value(std::move(value)), pending_key(std::nullopt), next_expected_item(next) {}
    Frame(JsonValue value)
        : value(std::move(value)),
          pending_key(std::nullopt),
          next_expected_item(NextExpectedItem::Done) {}
    static Frame object() {
        return Frame(std::make_unique<JsonObject>(),
                     NextExpectedItem::KeyOrClose);
    }
    static Frame array() {
        return Frame(std::make_unique<JsonArray>(),
                     NextExpectedItem::ValueOrClose);
    }

    void accept_string(std::string& s) {
        if (auto v = std::get_if<JsonArray>(&value)) {
            switch (next_expected_item) {
                case NextExpectedItem::Value:
                case NextExpectedItem::ValueOrClose:
                    v->values.push_back(s);
                    next_expected_item = NextExpectedItem::CommaOrClose;
                    break;
                default:
                    throw std::runtime_error("Shit went wrong");
            }
            return;
        }

        if (std::holds_alternative<JsonObject>(value)) {
            switch (next_expected_item) {
                case NextExpectedItem::Value:
                case NextExpectedItem::ValueOrClose:
                    if (pending_key.has_value()) {
                        auto obj = std::get<JsonObject>(value);
                        obj.values.try_emplace(pending_key.value(), s);
                        pending_key = std::nullopt;
                    } else {
                        std::runtime_error(
                            "This might be impossible to reach idk");
                    }
                    break;
                case NextExpectedItem::Key:
                case NextExpectedItem::KeyOrClose:
                    pending_key = s;
                    break;
                default:
                    std::runtime_error("Shit went wrong");
            }
            return;
        }

        std::runtime_error(
            "Tried giving a string to something that was already a primative. "
            "Probably unreachable");
    }
};

enum class ParserState2 {
    ExpectingValue,
    ExpectingStringOrClose,
    ExpectingValueOrClose,
    ExpectingColon
};

void handle_string(std::string s, std::vector<Frame>& stack) {
    if (stack.empty()) {
        stack.push_back(Frame(s));
        return;
    }

    Frame& back = stack.back();

    back.accept_string(s);
}

JsonValue parser(TokenStream& tokens) {
    std::vector<Frame> stack;

    while (tokens.has_next()) {
        Token token = tokens.next();

        switch (token.type) {
            case TokenTypes::OpenObject:
                stack.push_back(Frame::object());
                break;
            case TokenTypes::OpenArray:
                stack.push_back(Frame::array());
                break;
            case TokenTypes::String:
                handle_string(std::get<std::string>(token.data), stack);
                break;
            case TokenTypes::Number: {
                auto num = std::get<float>(token.data);

                if (stack.empty()) {
                    stack.push_back(Frame(num));
                    break;
                }

                Frame& frame = stack.back();

                switch

                break;
            }
        }
    }
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
                    case TokenTypes::OpenArray: {
                        const Token& token = tokens.peek();

                        if (token.type == TokenTypes::CloseArray) {
                            tokens.next();
                            return array;
                        };

                        state = ParserState::ExpectingCommaOrArrayClose;
                        array->values.push_back(parser(tokens));
                        break;
                    }
                    default:
                        throw ParserIllegalToken(std::format(
                            "Expected a JSON value or start of object or "
                            "array, found {} at line {} position {}",
                            token.type, token.line, token.position));
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
