#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

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
