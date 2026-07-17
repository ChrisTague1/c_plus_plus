#include "json.h"

std::ostream& operator<<(std::ostream& os, const JsonArray& array) {
    os << "[";

    for (std::size_t i = 0; i < array.values.size(); i++) {
        if (i > 0) {
            os << ", ";
        }
        os << array.values[i];
    }

    return os << "]";
}

std::ostream& operator<<(std::ostream& os, const JsonObject& object) {
    os << "{";
    bool first = true;
    for (const auto& [key, value]: object.values) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << "\"" << key << "\": " << value;
    }

    return os << "}";
}

std::ostream& operator<<(std::ostream& os, const JsonValue& value) {
    struct JsonValuePrinter {
        std::ostream& os;

        void operator()(float f) const {
            os << f;
        }
        void operator()(bool b) const {
            if (b) {
                os << "true";
            } else {
                os << "false";
            }
        }
        void operator()(std::nullptr_t) const {
            os << "null";
        }
        void operator()(const std::string& s) const {
            os << '"' << s << '"';
        }
        void operator()(const std::unique_ptr<JsonArray>& array) const {
            os << *array;
        }
        void operator()(const std::unique_ptr<JsonObject>& object) const {
            os << *object;
        }
    };

    std::visit(JsonValuePrinter{os}, value);
    return os;
}
