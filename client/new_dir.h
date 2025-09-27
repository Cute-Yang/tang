#pragma once
#include "ElaDialog.h"
#include "new_dir_ui.h"
#include <QWidget>


namespace tang {
namespace client {
class NewDir : public ElaDialog {
    Q_OBJECT

public:
    NewDirUi* ui;
    NewDir(QWidget* parent = nullptr);

    void display();

    void clear();
};

}   // namespace client
}   // namespace tang