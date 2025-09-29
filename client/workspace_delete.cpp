#include "workspace_delete.h"
#include "client_global_config.h"
#include <QHBoxLayout>
#include <QVBoxLayout>



namespace tang {
namespace client {
WorkspaceDeleteFile::WorkspaceDeleteFile(QWidget* parent)
    : ElaDialog(parent)
    , delete_file_index(0) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QWidget* hint_container = new QWidget(this);
    layout->addWidget(hint_container);
    QHBoxLayout* hint_layout = new QHBoxLayout(hint_container);
    hint_text                = new ElaText(this);
    auto font                = hint_text->font();
    font.setPointSize(10);
    hint_text->setFont(font);
    hint_text->setText("确认删除文件?");
    hint_layout->addStretch();
    hint_layout->addWidget(hint_text);
    hint_layout->addStretch();

    QWidget* button_container = new QWidget(this);
    layout->addWidget(button_container);
    QHBoxLayout* button_layout = new QHBoxLayout(button_container);
    button_layout->addStretch();

    constexpr QSize button_size(80, 30);
    ok_button = new ElaToolButton(this);
    ok_button->setObjectName("delete_ok_button");
    QString ok_button_style = ClientGlobalConfig::button_style_sheet_fmt.arg("delete_ok_button")
                                  .arg(":icons/images/leaf.svg");
    ok_button->setFixedSize(button_size);
    ok_button->setText("确认");
    ok_button->setStyleSheet(ok_button_style);
    ok_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button_layout->addWidget(ok_button);

    cancle_button = new ElaToolButton(this);
    cancle_button->setObjectName("delete_cancle_button");
    QString cancle_button_style =
        ClientGlobalConfig::button_style_sheet_fmt.arg("delete_cancle_button")
            .arg(":icons/images/leaf.svg");
    cancle_button->setFixedSize(button_size);
    cancle_button->setText("取消");
    cancle_button->setStyleSheet(cancle_button_style);
    cancle_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button_layout->addWidget(cancle_button);
    button_layout->addStretch();

    this->setWindowTitle("删除文件");
    this->setFixedSize(QSize(400, 200));
    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MinimizeButtonHint);
}

WorkspaceDeleteFile::~WorkspaceDeleteFile() {}

void WorkspaceDeleteFile::display() {
    this->show();
    this->raise();
    this->activateWindow();
    this->moveToCenter();
}

void WorkspaceDeleteFile::set_delete_filename(const QString& filename) {
    delete_filename = filename;
}

const QString& WorkspaceDeleteFile::get_delete_fielname() const {
    return delete_filename;
}

void WorkspaceDeleteFile::set_delete_file_index(size_t index) {
    delete_file_index = index;
}

size_t WorkspaceDeleteFile::get_delete_file_index() const {
    return delete_file_index;
}

}   // namespace client
}   // namespace tang