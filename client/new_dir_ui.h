#pragma once
// #include "ElaDialog.h"
#include "ElaLineEdit.h"
#include "ElaText.h"
#include "ElaToolButton.h"

namespace tang {
namespace client {
class NewDirUi {
public:
    ElaText*       text;
    ElaLineEdit*   line_edit;
    ElaToolButton* ok_button;
    ElaToolButton* cancle_button;

public:
    void setup_ui(QWidget* parent);
};
}   // namespace client
}   // namespace tang