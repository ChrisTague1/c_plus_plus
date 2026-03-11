#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <functional>

template<typename K, typename V>
class MyHashMap {
   private:
    struct Entry {
        K key;
        V value;
    };
    std::vector<std::vector<Entry>> data;
    size_t numBuckets;
    std::hash<K> hasher;
   public:
    MyHashMap(size_t buckets = 20) : numBuckets(buckets) {
        data.resize(numBuckets);
    };

    std::optional<V> get(const K& key) {
        size_t key_hash = hasher(key);

        std::vector<Entry> bucket = data[key_hash % numBuckets];

        for (const auto& entry : bucket) {
            if (entry.key == key) {
                return entry.value;
            }
        }

        return std::nullopt;
    }

    bool set(const K& key, V value) {
        size_t key_hash = hasher(key);

        std::vector<Entry>& bucket = data[key_hash % numBuckets];

        for (auto& entry : bucket) {
            if (entry.key == key) {
                entry.value = value;
                return false;
            }
        }

        bucket.push_back(Entry {
            .key = key,
            .value = value,
        });

        return true;
    }
    friend std::ostream& operator<<(std::ostream& os, const MyHashMap& map) {
        os << "{\n";
        bool first = true;
        for (const auto& bucket : map.data) {
            for (const auto& entry : bucket) {
                if (!first) os << ",\n";
                os << "  " << entry.key << ": " << entry.value;
                first = false;
            }
        }
        os << (first ? "}" : "\n}");
        return os;
    }
};

int main() {
    MyHashMap<std::string, int> map;

    map.set("hey", 1);
    map.set("hey", 2);
    map.set("what are you doing?", 20);

    std::cout << map << "\n";

    return 0;
}