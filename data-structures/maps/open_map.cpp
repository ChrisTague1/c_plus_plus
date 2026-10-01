#include <iostream>
#include <string>
#include <memory>
#include <optional>

enum class State: uint8_t {
    EMPTY,
    OCCUPIED,
    DELETED
};

template <typename K, typename V>
class OpenMap {
    struct Entry {
        K key;
        V value;
        State state = State::EMPTY;
    };
    std::unique_ptr<Entry[]> data;
    std::hash<K> hasher;
    size_t capacity;
    size_t size;
public:
    OpenMap(size_t capacity = 16): capacity(capacity), size(0) {
        data = std::make_unique<Entry[]>(capacity);
    }
    
    void insert(K key, V value) {
        if (size + 1 > capacity) {
            capacity *= 2;
            auto new_data = std::make_unique<Entry[]>(capacity);

            for (size_t i = 0; i < size; ++i) {
                K& k = data[i].key;
                size_t hash = hasher(k);

                for (size_t offset = 0; offset < capacity; ++offset) {
                    size_t loc = (hash + offset) % capacity;

                    if (new_data[loc].state != State::OCCUPIED) {
                        new_data[loc] = std::move(data[i]);
                        break;
                    }
                }
            }

            data = std::move(new_data);
        }

        size_t hash = hasher(key);
        for (size_t offset = 0; offset < capacity; ++offset) {
            size_t loc = (hash + offset) % capacity;

            if (data[loc].state != State::OCCUPIED) {
                data[loc] = std::move(Entry {
                    .key = key,
                    .value = value,
                    .state = State::OCCUPIED,
                });
                break;
            }
        }
        size++;

        return;
    }

    std::optional<V> remove(K& key) {
        size_t hash = hasher(key);
        for (size_t offset = 0; offset < capacity; ++offset) {
            size_t loc = (hash + offset) % capacity;

            if (data[loc].state == State::OCCUPIED && data[loc].key == key) {
                data[loc].state = State::DELETED;
                size--;
                return data[loc].value;
            }
        }

        return std::nullopt;
    }

    void print_mem() {
        std::cout << "capacity = " << capacity << "\n";
        std::cout << "size = " << size << "\n";
        std::cout << "{\n";
        bool first = true;
        for (size_t i = 0; i < capacity; ++i) {
            const auto& entry = data[i];

            if (!first) {
                std::cout << ",\n";
            }
            first = false;

            if (entry.state == State::DELETED) {
                std::cout << "DELETED";
            } else if (entry.state == State::EMPTY) {
                std::cout << "EMPTY";
            } else {
                std::cout << "\t" << entry.key << ": " << entry.value;
            }
        }

        std::cout << "\n}\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const OpenMap& map) {
        os << "{\n";
        bool first = true;

        for (size_t i = 0; i < map.capacity; ++i) {
            const auto& entry = map.data[i];
            if (entry.state != State::OCCUPIED) continue;

            if (!first) os << ",\n";
            os << " " << entry.key << ": " << entry.value;
            first = false;
        }

        os << (first ? "}" : "\n}");
        return os;
    }
};

int main() {
    /*
    Improvements:
    - when rehashing, don't move deleted or empty items
    - you can exit early in delete if you see something empty
    - rehash at a threshold, not when full (maybe 75% capacity)
    - insert appends a new key atm
    - pass by const &
    - overwrite/replace/do nothing
    - bitmap instead of state
    */
    OpenMap<std::string, int> map(2);

    map.insert("a", 1);
    map.insert("b", 2);
    map.insert("c", 3);
    // map.print_mem();

    std::string c = "c";

    std::optional<int> val = map.remove(c);

    // if (val.has_value()) {
    //     std::cout << "value: " << val.value() << "\n";
    // } else {
    //     std::cout << "no value\n";
    // }

    // map.print_mem();

    map.insert("c", 3);
    map.insert("d", 3);
    map.insert("e", 3);
    map.insert("f", 3);
    map.insert("g", 3);
    map.insert("h", 3);
    map.insert("i", 3);
    map.insert("j", 3);
    map.insert("k", 3);

    map.print_mem();

    return 0;
}