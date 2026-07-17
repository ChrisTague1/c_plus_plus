#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <sstream>

#include "token.h"
#include "lexer.h"

struct JsonArray;
struct JsonObject;

using JsonValue = std::variant<
    float,
    bool,
    std::nullptr_t,
    std::string,
    std::unique_ptr<JsonArray>,
    std::unique_ptr<JsonObject>
>;

struct JsonArray {
    std::vector<JsonValue> values;
};

struct JsonObject {
    std::unordered_map<std::string, JsonValue> values;
};

std::ostream& operator<<(std::ostream& os, const JsonValue& value);
std::ostream& operator<<(std::ostream& os, const JsonArray& array);
std::ostream& operator<<(std::ostream& os, const JsonObject& object);

struct JsonValuePrinter {
    std::ostream& os;

    void operator()(float f) const { os << f; }
    void operator()(bool b) const { os << (b ? "true" : "false"); }
    void operator()(std::nullptr_t) const { os << "null"; }
    void operator()(const std::string& s) const { os << '"' << s << '"'; }
    void operator()(const std::unique_ptr<JsonArray>& array) const { os << *array; }
    void operator()(const std::unique_ptr<JsonObject>& object) const { os << *object; }
};

std::ostream& operator<<(std::ostream& os, const JsonArray& array) {
    os << "[";
    for (std::size_t i = 0; i < array.values.size(); ++i) {
        if (i > 0) os << ", ";
        os << array.values[i];
    }
    return os << "]";
}

std::ostream& operator<<(std::ostream& os, const JsonObject& object) {
    os << "{";
    bool first = true;
    for (const auto& [key, value] : object.values) {
        if (!first) os << ", ";
        first = false;
        os << "\"" << key << "\": " << value;
    }
    return os << "}";
}

std::ostream& operator<<(std::ostream& os, const JsonValue& value) {
    std::visit(JsonValuePrinter{os}, value);
    return os;
}

enum class ParserState {
    ExpectingString,
    ExpectingColon,
    ExpectingValue,
    ExpectingCommaOrObjectClose,
    ExpectingCommaOrArrayClose,
};

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
        std::cout << token << ", State: " << state << std::endl;

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

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    TokenStream tokens = lexer(file);

    JsonValue value = parser(tokens);

    std::cout << value << std::endl;

    return 0;
}