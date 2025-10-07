#include "chat_room.h"
#include "chat_delegate.h"
#include "chat_view_model.h"
// #include "client_singleton.h"
#include "client_singleton.h"
#include "common/chat_data.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>



using namespace tang::common;

namespace tang {
namespace client {

ChatRoom::ChatRoom(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new ChatRoomUI()) {
    ui->setup_ui(this);
    this->setTitleVisible(false);
    // the max message size is 64!
    model = new ChatViewModel(8);
    // test datas
    QStringList messages = {"你好，今天过得怎么样？<br>你好，今天过得怎么样",
                            "我很好，谢谢！<br>你好，",
                            "这是一个带有格式的长消息示例。",
                            "当然没问题！<br>你好，今天过得怎么样"};
    for (size_t i = 0; i < messages.size(); ++i) {
        ChatMessage msg = {messages[i], i % 2 ? "layzdog" : "黄蓉", "2025-09-11 13:28:49"};
        model->append_message(msg);
    }

    ChatDelegate* delegate = new ChatDelegate(ui->chat_view);
    ui->chat_view->setItemDelegate(delegate);
    ui->chat_view->setModel(model);
    ui->chat_view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    this->init_connects();
}

ChatRoom::~ChatRoom() {}

void ChatRoom::init_connects() {
    connect(ui->send_button, &ElaToolButton::clicked, this, &ChatRoom::prepare_chat_message);
}


void ChatRoom::prepare_chat_message() {
    auto text = ui->message_text_edit->toPlainText();
    ui->message_text_edit->clear();
    text                  = text.replace("\n", "<br>");
    auto&       user_info = ClientSingleton::get_cache_user_info_instance();
    QJsonObject json_data;
    json_data[ChatJsonKeys::message_kind_key]    = static_cast<int>(ChatMessageKind::kNormalChat);
    json_data[NormalChatJsonKeys::message_key]   = text;
    json_data[NormalChatJsonKeys::send_user_key] = user_info.user_name;
    json_data[NormalChatJsonKeys::send_time_key] =
        QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QJsonDocument json_doc(json_data);
    emit          send_chat_message(json_doc.toJson(QJsonDocument::JsonFormat::Compact));
}

void ChatRoom::append_chat_message(const ChatMessage& message) {
    model->append_message(message);
    ui->chat_view->scrollToBottom();
}

}   // namespace client
}   // namespace tang