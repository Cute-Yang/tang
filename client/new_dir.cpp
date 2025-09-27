#include "new_dir.h"
namespace tang {
namespace client {
NewDir::NewDir(QWidget* parent)
    : ElaDialog(parent)
    , ui(new NewDirUi()) {
    ui->setup_ui(this);
    this->setWindowTitle("新建文件夹");
    this->setFixedSize(QSize(400, 200));
    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MinimizeButtonHint);
}
void NewDir::display() {
    this->show();
    this->raise();
    this->activateWindow();
    this->moveToCenter();
}

void NewDir::clear() {
    this->ui->line_edit->clear();
    this->close();
}
};   // namespace client
}   // namespace tang