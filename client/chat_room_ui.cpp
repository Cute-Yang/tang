#include "chat_room_ui.h"
#include <QIcon>
#include <QVBoxLayout>

namespace tang {
namespace client {
void ChatRoomUI::setup_ui(ElaScrollPage* parent) {
    QWidget*     chat_container = new QWidget(parent);
    QVBoxLayout* chat_layout    = new QVBoxLayout(chat_container);

    chat_view = new ElaListView(parent);
    chat_layout->addWidget(chat_view);

    // QWidget* msg_container = new QWidget(parent);
    // chat_layout->addWidget(msg_container);

    // QHBoxLayout* msg_layout = new QHBoxLayout(msg_container);

    message_text_edit = new ElaPlainTextEdit(parent);
    message_text_edit->setMaximumHeight(80);
    chat_layout->addWidget(message_text_edit);
    message_text_edit->setLineWidth(120);

    chat_layout->addSpacing(8);
    // chat_layout->addStretch();
    send_button = new ElaToolButton(parent);
    send_button->setText("发送");
    send_button->setIcon(QIcon(":icons/images/sea_leaf.svg"));
    send_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QHBoxLayout* tt = new QHBoxLayout();
    tt->addStretch();
    tt->addWidget(send_button);
    tt->addStretch();
    chat_layout->addLayout(tt);
    parent->addCentralWidget(chat_container);
}
}   // namespace client
}   // namespace tang