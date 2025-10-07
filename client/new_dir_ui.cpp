#include "new_dir_ui.h"
#include <QHBoxLayout>
#include <QVBoxLayout>


namespace tang {
namespace client {
void NewDirUi::setup_ui(QWidget* parent) {
    QVBoxLayout* layout = new QVBoxLayout(parent);

    QWidget* text_container = new QWidget(parent);
    layout->addWidget(text_container);
    QHBoxLayout* text_layout = new QHBoxLayout(text_container);
    text                     = new ElaText(parent);
    auto font                = text->font();
    font.setPointSize(10);
    text->setFont(font);
    text->setText("文件夹名称:");
    text_layout->addWidget(text);
    text_layout->addSpacing(8);

    line_edit = new ElaLineEdit(parent);
    line_edit->setPlaceholderText("new dir...");
    line_edit->setFont(font);
    text_layout->addWidget(line_edit);

    QWidget*     button_container = new QWidget(parent);
    QHBoxLayout* button_layout    = new QHBoxLayout(button_container);
    layout->addWidget(button_container);


    button_layout->addStretch();
    ok_button                       = new ElaToolButton(parent);
    QString         ok_button_style = R"(
        QToolButton#ok_button{
            background-color:#20B2AA;
            font-size: 12px;
            font-weight: normal;
            border: 2px solid #b9b9b9; 
            border-radius: 3px;
            qproperty-icon: url(":icons/images/leaf.svg") center;
            qproperty-iconSize: 16px 16px;
            padding-left: 15px;
        }
    )";
    constexpr QSize button_size(80, 30);
    ok_button->setFixedSize(button_size);
    ok_button->setText("确定");
    ok_button->setObjectName("ok_button");
    ok_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ok_button->setStyleSheet(ok_button_style);
    ok_button->setFont(font);

    button_layout->addWidget(ok_button);

    button_layout->addSpacing(20);

    cancle_button               = new ElaToolButton(parent);
    QString cancle_button_style = R"(
        QToolButton#cancle_button{
            background-color:#20B2AA;
            font-size: 12px;
            font-weight: normal;
            border: 2px solid #b9b9b9; 
            border-radius: 3px;
            qproperty-icon: url(":icons/images/leaf.svg") center;
            qproperty-iconSize: 16px 16px;
            padding-left: 15px;
        }
    )";
    cancle_button->setFixedSize(button_size);
    cancle_button->setObjectName("cancle_button");
    cancle_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    cancle_button->setStyleSheet(cancle_button_style);
    cancle_button->setText("取消");
    cancle_button->setFont(font);
    button_layout->addWidget(cancle_button);


    button_layout->addStretch();
}
}   // namespace client
}   // namespace tang