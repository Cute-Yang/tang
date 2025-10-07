#include "chat_view_model.h"
#include "chat_role.h"

namespace tang {
namespace client {
ChatViewModel::ChatViewModel(size_t max_message_size_, QObject* parent)
    : QAbstractListModel(parent)
    , max_message_size(max_message_size_) {
    assert(max_message_size_ > 0);
    chat_messages.reserve(max_message_size_);
}


ChatViewModel::~ChatViewModel() {}

// here we split the data to 2 part!
//  the top part is the newset data
//  and the next part is previous part!
void ChatViewModel::append_message(const ChatMessage& message) {
    if (chat_messages.size() == max_message_size) {
        // not full
        chat_messages.clear();
    }
    auto n = chat_messages.size();
    beginInsertRows(QModelIndex(), n, n);
    chat_messages.push_back(message);
    endInsertRows();
}

void ChatViewModel::append_message(ChatMessage&& message) {
    if (chat_messages.size() == max_message_size) {
        // not full
        chat_messages.clear();
    }
    auto n = chat_messages.size();
    beginInsertRows(QModelIndex(), n, n);
    chat_messages.push_back(message);
    endInsertRows();
}

int ChatViewModel::rowCount(const QModelIndex& parent) const {
    return chat_messages.size();
}

QVariant ChatViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return {};
    }
    int row = index.row();
    // display the front part!
    // symmetric,perfect!
    auto& data = chat_messages[row];
    if (role == ChatRole::MessageTextRole) {
        return data.message;
    } else if (role == ChatRole::NicknameTextRole) {
        return data.nickname;
    } else if (role == ChatRole::MessageTimeRole) {
        return data.send_time;
    }
    return {};
}

}   // namespace client
}   // namespace tang