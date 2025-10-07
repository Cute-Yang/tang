#pragma once
#include "ElaListView.h"
#include "ElaPlainTextEdit.h"
#include "ElaToolButton.h"
#include "ElaScrollPage.h"

namespace tang {
namespace client {
class ChatRoomUI {
public:
    ElaListView*      chat_view;
    ElaPlainTextEdit* message_text_edit;
    ElaToolButton*    send_button;

    void setup_ui(ElaScrollPage* parent=nullptr);
};

}   // namespace client
}   // namespace tang