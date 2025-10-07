#pragma once

// this header define the chat data between sever and client!
namespace tang {
namespace common {
enum class ChatMessageKind { kNormalChat = 0, kNotifyOtherVoter = 1, kNotifyUserStatus = 2 };

struct ChatJsonKeys {
    static constexpr const char* message_kind_key = "message_kind";
};

struct NormalChatJsonKeys : public ChatJsonKeys {
    static constexpr const char* message_key = "message";
    static constexpr const char* send_user_key = "send_user";
    static constexpr const char* send_time_key = "send_time";
};


}   // namespace common
}   // namespace tang