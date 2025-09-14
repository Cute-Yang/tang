#pragma once
#include "ElaLineEdit.h"
#include "ElaText.h"
#include "QHBoxLayout"
#include "QVBoxLayout"


namespace tang {
namespace client {
class LoginSettingUi {
public:
    ElaText* server_addr_text;
    ElaText* server_http_port_text;
    ElaText* server_websocket_port_text;

    ElaLineEdit* server_addr_line_edit;
    ElaLineEdit* server_http_port_line_edit;
    ElaLineEdit* server_websocket_port_line_edit;

    void setup_ui(QWidget* parent) {
        QVBoxLayout* main_layout = new QVBoxLayout(parent);
        main_layout->setContentsMargins({0,0,0,0});

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
        server_addr_text->setText("服务器地址");
        server_addr_layout->addWidget(server_addr_text);

        constexpr int line_edit_width = 200;

        server_addr_line_edit = new ElaLineEdit(parent);
        server_addr_line_edit->setPlaceholderText("127.0.0.1");
        server_addr_line_edit->setFont(font);
        server_addr_line_edit->setFixedWidth(line_edit_width);
        server_addr_layout->addWidget(server_addr_line_edit);


        QWidget*     server_http_port_container = new QWidget(parent);
        QHBoxLayout* server_http_port_layout    = new QHBoxLayout(server_http_port_container);
        layout->addWidget(server_http_port_container);

        server_http_port_text = new ElaText(parent);
        server_http_port_text->setFont(font);
        server_http_port_text->setFixedWidth(text_fixed_width);
        server_http_port_text->setText("http端口");
        server_http_port_layout->addWidget(server_http_port_text);

        server_http_port_line_edit = new ElaLineEdit(parent);
        server_http_port_line_edit->setFont(font);
        server_http_port_line_edit->setFixedWidth(line_edit_width);
        server_http_port_line_edit->setPlaceholderText("6027");
        server_http_port_layout->addWidget(server_http_port_line_edit);


        QWidget*     server_websocket_port_container = new QWidget(parent);
        QHBoxLayout* server_websocket_port_layout =
            new QHBoxLayout(server_websocket_port_container);
        layout->addWidget(server_websocket_port_container);

        server_websocket_port_text = new ElaText(parent);
        server_websocket_port_text->setFont(font);
        server_websocket_port_text->setFixedWidth(text_fixed_width);
        server_websocket_port_text->setText("websocket端口");
        server_websocket_port_layout->addWidget(server_websocket_port_text);

        server_websocket_port_line_edit = new ElaLineEdit(parent);
        server_websocket_port_line_edit->setFont(font);
        server_websocket_port_line_edit->setFixedWidth(line_edit_width);
        server_websocket_port_line_edit->setPlaceholderText("6028(选填)");
        server_websocket_port_layout->addWidget(server_websocket_port_line_edit);

        layout->addStretch();
    }
};
}   // namespace client
}   // namespace tang