#pragma once
#include "ElaLineEdit.h"
#include "ElaText.h"
#include "ElaToolButton.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include <QIntValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>



namespace tang {
namespace client {
class LoginSettingUi {
public:
    ElaText* server_addr_text;
    ElaText* server_http_port_text;

    ElaLineEdit* server_addr_line_edit;
    ElaLineEdit* server_http_port_line_edit;

    ElaToolButton* confirm_button;

    void setup_ui(QWidget* parent) {
        QVBoxLayout* main_layout = new QVBoxLayout(parent);
        main_layout->setContentsMargins({0, 0, 0, 0});

        QWidget* central_container = new QWidget(parent);
        central_container->setObjectName("login_setting_central");
        QString      central_container_style = R"style(
        QWidget#login_setting_central{
            background-color:#ADD8E6;
            }
        )style";
        QVBoxLayout* layout                  = new QVBoxLayout(central_container);
        central_container->setStyleSheet(central_container_style);

        main_layout->addWidget(central_container);


        QWidget*     server_addr_container = new QWidget(parent);
        QHBoxLayout* server_addr_layout    = new QHBoxLayout(server_addr_container);
        layout->addWidget(server_addr_container);

        server_addr_text = new ElaText(parent);
        auto font        = server_addr_text->font();
        font.setFamily("SimSun");
        font.setPointSize(10);
        font.setBold(true);
        constexpr int text_fixed_width = 100;
        server_addr_text->setFont(font);
        server_addr_text->setFixedWidth(text_fixed_width);
        server_addr_text->setText("服务器地址:");
        server_addr_layout->addWidget(server_addr_text);

        constexpr int line_edit_width = 200;

        server_addr_line_edit = new ElaLineEdit(parent);
        server_addr_line_edit->setPlaceholderText("127.0.0.1");
        server_addr_line_edit->setFont(font);
        server_addr_line_edit->setFixedWidth(line_edit_width);
        server_addr_layout->addWidget(server_addr_line_edit);

        // QRegularExpression ip_regex(
        //     R"(^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
        // QRegularExpressionValidator* ip_validator =
        //     new QRegularExpressionValidator(ip_regex, parent);
        // server_addr_line_edit->setValidator(ip_validator);

        QWidget*     server_http_port_container = new QWidget(parent);
        QHBoxLayout* server_http_port_layout    = new QHBoxLayout(server_http_port_container);
        layout->addWidget(server_http_port_container);

        server_http_port_text = new ElaText(parent);
        server_http_port_text->setFont(font);
        server_http_port_text->setFixedWidth(text_fixed_width);
        server_http_port_text->setText("端口:");
        server_http_port_layout->addWidget(server_http_port_text);

        server_http_port_line_edit = new ElaLineEdit(parent);
        server_http_port_line_edit->setFont(font);
        server_http_port_line_edit->setFixedWidth(line_edit_width);
        server_http_port_line_edit->setPlaceholderText("9000");
        QIntValidator* port_validator = new QIntValidator(1, 65535, parent);
        server_http_port_line_edit->setValidator(port_validator);

        server_http_port_layout->addWidget(server_http_port_line_edit);
        layout->addSpacing(20);

        QWidget*     confirm_button_container = new QWidget(parent);
        QHBoxLayout* confirm_button_layout    = new QHBoxLayout(confirm_button_container);
        layout->addWidget(confirm_button_container);
        confirm_button = new ElaToolButton(parent);
        confirm_button->setFixedSize(QSize(80, 30));
        confirm_button->setText("确认");
        confirm_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        confirm_button->setObjectName("confirm_button");
        // not support constexpr in qt style sheet
        //  so we can only use a normal string!
        QString confirm_button_style = R"(
            QToolButton#confirm_button{
                background-color:#20B2AA;
                font-size: 12px;
                font-weight: normal;
                border: 2px solid #b9b9b9; 
                border-radius: 3px;
                qproperty-icon: url(":icons/images/confirm.svg") center;
                qproperty-iconSize: 16px 16px;
                padding-left: 15px;
            }
        )";
        confirm_button->setStyleSheet(confirm_button_style);
        confirm_button->setFont(font);

        confirm_button_layout->addWidget(confirm_button);

        layout->addStretch();
    }
};
}   // namespace client
}   // namespace tang