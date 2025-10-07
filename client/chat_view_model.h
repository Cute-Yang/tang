#pragma once
#include <QAbstractListModel>
#include <QList>
#include <QString>
namespace tang {
namespace client {

struct ChatMessage {
    QString message;
    QString nickname;
    QString send_time;
    // int     text_width{0};
};

class ChatViewModel : public QAbstractListModel {
    Q_OBJECT
public:
private:
    QList<ChatMessage> chat_messages;
    size_t             max_message_size;

public:
    ChatViewModel(size_t max_message_size_ = 500, QObject* parent = nullptr);
    ~ChatViewModel();
    void append_message(const ChatMessage& message);
    void append_message(ChatMessage&& message);

protected:
    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
};



}   // namespace client
}   // namespace tang