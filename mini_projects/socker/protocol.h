#pragma once

constexpr int MAX_NAME_LEN = 32;

enum class MessageType : uint8_t {
    JoinServer,
};

#pragma pack(push, 1)
struct Message {
    MessageType message_type;
    union {
        struct {
            char name[MAX_NAME_LEN];
        } join_server;
    };
};
#pragma pack(pop)
