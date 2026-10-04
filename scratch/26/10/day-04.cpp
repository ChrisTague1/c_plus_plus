#include <iostream>
#include <memory>

struct Item {
    int id;
    double value;
};

struct Message {
    size_t count;

    static constexpr size_t items_offset() {
        /*
        if sizeof message = 18
        alignof item = 8

        item - 1 = 8 - 1 = 7
        0000 0111
        ~ will flip
        1111 1000
        last 3 are zero, and 2^3 = 8, anything divisible by 8 has last 3 zero
        8 =  0000 1000
        16 = 0001 0000
        24 = 0001 1000

        18 + 8 = 26, -1 = 25
        0001 1001
        & against a bunch of 1's and last three 0's
        1111 1000
        0001 1001
        0001 1000 = 24

        this only works when alignof is a power of 2, which it always will be
        */
        return (sizeof(Message) + alignof(Item) - 1) & ~(alignof(Item) - 1);
    }
    
    Item* items() {
        auto* bytes = reinterpret_cast<std::byte*>(this);
        return reinterpret_cast<Item*>(bytes + items_offset());
    }

    const Item* items() const {
        auto* bytes = reinterpret_cast<const std::byte*>(this);
        return reinterpret_cast<const Item*>(bytes + items_offset());
    }
};

struct MessageDeleter {
    void operator()(Message* msg) const noexcept {
        if (!msg) return;

        for (size_t i = 0; i < msg->count; ++i) {
            msg->items()[i].~Item();
        }

        msg->~Message();

        ::operator delete(msg);
    }
};

using MessagePtr = std::unique_ptr<Message, MessageDeleter>;

MessagePtr make_message(size_t n) {
    size_t bytes = Message::items_offset() + n * sizeof(Item);

    void* memory = ::operator new(bytes);

    Message* msg = new (memory) Message{n};

    for (size_t i = 0; i < n; ++i) {
        new (&msg->items()[i]) Item{};
    }

    return MessagePtr(msg);
}

Message* make_message_raw(size_t n) {
    size_t bytes = Message::items_offset() + n * sizeof(Item);

    void* memory = ::operator new(bytes);

    auto* msg = new (memory) Message{n};

    for (size_t i = 0; i < n; ++i) {
        new (&msg->items()[i]) Item{};
    }

    return msg;
}

int main() {
    auto message = make_message(4);

    message->items()[2].id = 5;

    std::cout << "item two has id " << message->items()[2].id << std::endl;

    return 0;
}