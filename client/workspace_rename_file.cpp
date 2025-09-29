#include "workspace_rename_file.h"
#include "client_global_config.h"
#include <QHBoxLayout>
#include <QVBoxLayout>


namespace tang {
namespace client {
WorkspaceRenameFile::WorkspaceRenameFile(QWidget* parent)
    : ElaDialog(parent)
    , file_index(0) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    constexpr int fixed_key_width        = 80;
    constexpr int fixed_container_height = 48;

    QWidget* previous_filename_container = new QWidget(this);
    previous_filename_container->setFixedHeight(fixed_container_height);
    layout->addWidget(previous_filename_container);
    QHBoxLayout* previous_filename_layout = new QHBoxLayout(previous_filename_container);

    previous_filename_key = new ElaText(this);
    auto font             = previous_filename_key->font();
    font.setPointSize(10);
    previous_filename_key->setFixedWidth(fixed_key_width);
    previous_filename_key->setEnabled(false);
    previous_filename_key->setFont(font);
    previous_filename_key->setText("原文件名:");
    previous_filename_layout->addWidget(previous_filename_key);
    previous_filename_layout->addSpacing(4);

    previous_filename_value = new ElaText(this);
    previous_filename_value->setFont(font);
    previous_filename_value->setText("天空之城.png");
    previous_filename_value->setEnabled(false);
    previous_filename_layout->addWidget(previous_filename_value);


    QWidget* current_filename_container = new QWidget(this);
    current_filename_container->setFixedHeight(fixed_container_height);
    layout->addWidget(current_filename_container);
    QHBoxLayout* current_filename_layout = new QHBoxLayout(current_filename_container);

    current_filename_key = new ElaText(this);
    current_filename_key->setFont(font);
    current_filename_key->setFixedWidth(fixed_key_width);
    current_filename_key->setText("新文件名:");
    current_filename_key->setEnabled(false);
    current_filename_layout->addWidget(current_filename_key);

    current_filename_line_edit = new ElaLineEdit(this);
    current_filename_line_edit->setFont(font);
    current_filename_line_edit->setPlaceholderText("new filename...");
    current_filename_layout->addWidget(current_filename_line_edit);


    QWidget* button_container = new QWidget(this);
    layout->addWidget(button_container);
    QHBoxLayout* button_layout = new QHBoxLayout(button_container);
    button_layout->addStretch();

    constexpr QSize button_size(80, 30);
    ok_button = new ElaToolButton(this);
    ok_button->setObjectName("rename_ok_button");
    QString ok_button_style = ClientGlobalConfig::button_style_sheet_fmt.arg("rename_ok_button")
                                  .arg(":icons/images/leaf.svg");
    ok_button->setFixedSize(button_size);
    ok_button->setText("确认");
    ok_button->setStyleSheet(ok_button_style);
    ok_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button_layout->addWidget(ok_button);

    cancle_button = new ElaToolButton(this);
    cancle_button->setObjectName("rename_cancle_button");
    QString cancle_button_style =
        ClientGlobalConfig::button_style_sheet_fmt.arg("rename_cancle_button")
            .arg(":icons/images/leaf.svg");
    cancle_button->setFixedSize(button_size);
    cancle_button->setText("取消");
    cancle_button->setStyleSheet(cancle_button_style);
    cancle_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button_layout->addWidget(cancle_button);
    button_layout->addStretch();

    this->setWindowTitle("重命名");
    this->setFixedSize(QSize(350, 270));
    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MinimizeButtonHint);
}

WorkspaceRenameFile::~WorkspaceRenameFile() {}

void WorkspaceRenameFile::display() {
    this->show();
    this->raise();
    this->activateWindow();
    this->moveToCenter();
}


// not refresh the content

void WorkspaceRenameFile::set_file_index(size_t index) {
    file_index = index;
}

size_t WorkspaceRenameFile::get_file_index() const {
    return file_index;
}
}   // namespace client
}   // namespace tang