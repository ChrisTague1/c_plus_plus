#include "json.h"

#include <iostream>
#include <exception>

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

std::ostream& operator<<(std::ostream& os, const Json& json) {
    return os << json.value;
}

Json::Json(JsonValue& val): value(val) {

}

Json Json::operator[](std::string& key) const {
    if (auto obj_ptr = std::get_if<std::unique_ptr<JsonObject>>(&value)) {
        auto obj = obj_ptr->get();
        
        auto it = obj->values.find(key);

        if (it != obj->values.end()) {
            return Json(it->second);
        } else {
            throw std::out_of_range("key not found: " + key);
        }
    } else {
        throw std::runtime_error("json is not an object");
    }
}

Json Json::operator[](int index) const {
    if (auto arr_ptr = std::get_if<std::unique_ptr<JsonArray>>(&value)) {
        auto arr = arr_ptr->get();
        auto& val = arr->values.at(index);

        return Json(val);
    }

    throw std::runtime_error("json is not an array");
}

Json::operator bool() const {
    struct JsonBoolEval {
        bool operator()(float f) const {
            return f != 0;
        }
        bool operator()(bool b) const {
            return b;
        }
        bool operator()(std::nullptr_t) const {
            return false;
        }
        bool operator()(const std::string& s) const {
            return !s.empty();
        }
        bool operator()(const std::unique_ptr<JsonArray>& ar) const {
            return !ar->values.empty();
        }
        bool operator()(const std::unique_ptr<JsonObject>& obj) const {
            return !obj->values.empty();
        }
    };
    
    return std::visit(JsonBoolEval{}, value);
}
