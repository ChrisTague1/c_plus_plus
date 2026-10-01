#include <iostream>
#include <string>
#include <memory>

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
            std::cout << "reindexing..." << std::endl;
            capacity *= 2;
            auto new_data = std::make_unique<Entry[]>(capacity);

            for (size_t i = 0; i < size; ++i) {
                K& k = data[i].key;
                size_t hash = hasher(k);

                for (size_t offset = 0; offset < capacity; ++offset) {
                    size_t loc = (hash + offset) % capacity;

                    if (data[loc].state != State::OCCUPIED) {
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
    - overwrite/replace/do nothing
    - way to view complete memory layout
    - bitmap instead of state
    - delete
    */
    OpenMap<std::string, int> map(1);

    map.insert("hi", 50);
    map.insert("bye", 40);

    std::cout << map << std::endl;

    return 0;
}