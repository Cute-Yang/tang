#pragma once
#include "ElaLineEdit.h"
#include "ElaText.h"
#include "ElaToolButton.h"
#include <QAction>
#include <QRegularExpressionValidator>
#include <QString>
#include <QVBoxLayout>


namespace tang {
namespace client {
class SignUpUi {
public:
    ElaText* user_text;
    ElaText* password_text;
    ElaText* confirm_password_text;
    ElaText* email_text;

    ElaLineEdit* user_line_edit;

    ElaLineEdit* password_line_edit;
    QAction*     password_eye_action;

    ElaLineEdit* confirm_password_line_edit;
    QAction*     confirm_password_eye_action;

    ElaLineEdit*   email_line_edit;
    ElaToolButton* sign_up_button;

    QToolButton* confirm_button;
    QToolButton* back_button;

    void setup_ui(QWidget* window) {
        // QVBoxLayout* laytou = 32;
        QVBoxLayout* window_layout = new QVBoxLayout(window);
        window_layout->setContentsMargins({0, 0, 0, 0});

        // support change the style sheeT!
        QWidget* central_container = new QWidget(window);
        central_container->setObjectName("signup_central");
        // the method of QString -> std::string using the code of current env,for win,maybe ANSI
        QString central_container_style = R"style(
        QWidget#signup_central{
            background-color:#F5F5DC;
        }
    )style";
        central_container->setStyleSheet(central_container_style);
        QVBoxLayout* central_layout = new QVBoxLayout(central_container);
        // central_layout->addStretch();


        QWidget*     user_container = new QWidget(window);
        QHBoxLayout* user_layout    = new QHBoxLayout(user_container);
        user_layout->addStretch();
        constexpr int h_space    = 8;
        constexpr int text_width = 80;
        // user name
        user_text = new ElaText(window);
        auto font = user_text->font();
        font.setBold(true);
        font.setFamily("SimSun");
        font.setPointSize(10);

        user_text->setText("用户名:");
        user_text->setFont(font);
        user_text->setFixedWidth(text_width);
        user_layout->addWidget(user_text);
        user_layout->addSpacing(h_space);

        // 200
        QIcon         hide_eye_icon   = QIcon(":/icons/images/hide_password.svg");
        constexpr int line_edit_width = 200;

        user_line_edit = new ElaLineEdit(window);
        user_line_edit->setFixedWidth(line_edit_width);
        user_line_edit->setFont(font);
        user_layout->addWidget(user_line_edit);
        user_layout->addStretch();
        central_layout->addWidget(user_container);


        QWidget*     email_container = new QWidget(window);
        QHBoxLayout* email_layout    = new QHBoxLayout(email_container);
        email_layout->addStretch();
        // email!
        email_text = new ElaText(window);
        email_text->setText("邮箱:");
        email_text->setFont(font);
        email_text->setFixedWidth(text_width);
        email_layout->addWidget(email_text);
        email_layout->addSpacing(h_space);

        email_line_edit = new ElaLineEdit(window);
        email_line_edit->setPlaceholderText("example@163.com");
        QRegularExpression emaile_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        QRegularExpressionValidator* email_validator =
            new QRegularExpressionValidator(emaile_regex, window);
        email_line_edit->setValidator(email_validator);
        email_line_edit->setFixedWidth(line_edit_width);
        email_line_edit->setFont(font);

        email_layout->addWidget(email_line_edit);
        email_layout->addStretch();
        central_layout->addWidget(email_container);

        // password
        QWidget*     password_container = new QWidget(window);
        QHBoxLayout* password_layout    = new QHBoxLayout(password_container);
        password_layout->addStretch();

        password_text = new ElaText(window);
        password_text->setFont(font);
        password_text->setFixedWidth(text_width);
        password_text->setText("密码:");
        password_layout->addWidget(password_text);
        password_layout->addSpacing(h_space);

        password_line_edit = new ElaLineEdit(window);
        password_line_edit->setFont(font);
        // password is not visible defualt
        password_line_edit->setEchoMode(ElaLineEdit::Password);
        password_line_edit->setFixedWidth(line_edit_width);
        QRegularExpression           password_regexp("^[a-zA-Z0-9\\W]{6,12}$");
        QRegularExpressionValidator* password_validator =
            new QRegularExpressionValidator(password_regexp, window);
        password_line_edit->setValidator(password_validator);
        password_eye_action = new QAction(hide_eye_icon, "show/hide", window);
        password_eye_action->setCheckable(true);
        password_eye_action->setChecked(true);
        password_line_edit->addAction(password_eye_action, ElaLineEdit::TrailingPosition);
        password_layout->addWidget(password_line_edit);
        password_layout->addStretch();
        central_layout->addWidget(password_container);

        // confirm
        QWidget*     confirm_password_container = new QWidget(window);
        QHBoxLayout* confirm_password_layout    = new QHBoxLayout(confirm_password_container);
        confirm_password_layout->addStretch();

        confirm_password_text = new ElaText(window);
        confirm_password_text->setFont(font);
        confirm_password_text->setText("确认密码:");
        confirm_password_text->setFixedWidth(text_width);
        confirm_password_layout->addWidget(confirm_password_text);
        confirm_password_layout->addSpacing(h_space);

        confirm_password_line_edit = new ElaLineEdit(window);
        confirm_password_line_edit->setFixedWidth(line_edit_width);
        confirm_password_line_edit->setEchoMode(ElaLineEdit::Password);
        confirm_password_line_edit->setFont(font);
        confirm_password_eye_action = new QAction(hide_eye_icon, "show/hide", window);
        confirm_password_eye_action->setCheckable(true);
        confirm_password_eye_action->setChecked(true);
        confirm_password_line_edit->addAction(confirm_password_eye_action,
                                              ElaLineEdit::TrailingPosition);
        confirm_password_layout->addWidget(confirm_password_line_edit);
        confirm_password_layout->addStretch();
        central_layout->addWidget(confirm_password_container);

        QWidget*     button_container = new QWidget(window);
        QHBoxLayout* button_layout    = new QHBoxLayout(button_container);
        button_layout->addStretch();

        confirm_button = new QToolButton(window);
        confirm_button->setObjectName("confirm_button");
        QSize button_size(80, 30);

        QString confirm_button_style = R"(
        QToolButton#confirm_button{
            background-color:#20B2AA;
            font-size: 12px;
            font-weight: normal;
            border: 2px solid #b9b9b9; 
            border-radius: 3px;
            qproperty-icon: url(":icons/images/signup.svg") center;
            qproperty-iconSize: 16px 16px;
            padding-left: 15px;
        }
    )";
        // QIcon sigup_icon = QIcon(":icons/images/signup.svg");

        confirm_button->setStyleSheet(confirm_button_style);
        confirm_button->setFixedSize(button_size);
        confirm_button->setText("注册");
        confirm_button->setFont(font);
        // confirm_button->setIcon(sigup_icon);
        confirm_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button_layout->addWidget(confirm_button);
        button_layout->addSpacing(12);


        back_button = new QToolButton(window);
        back_button->setObjectName("back_button");
        QString back_button_style = R"(
        QToolButton#back_button{
            background-color:#20B2AA;
            font-size: 12px;
            font-weight: normal;
            border: 2px solid #b9b9b9; 
            border-radius: 3px;
            qproperty-icon: url(":icons/images/back.svg") center;
            qproperty-iconSize: 16px 16px;
            padding-left: 15px;
        }
    )";
        back_button->setStyleSheet(back_button_style);
        back_button->setFixedSize(button_size);
        back_button->setFont(font);
        back_button->setText("返回");
        // QIcon back_icon = QIcon(":icons/images/back.svg");
        // back_button->setIcon(back_icon);
        back_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


        button_layout->addWidget(back_button);
        button_layout->addStretch();
        central_layout->addWidget(button_container);

        central_layout->addStretch();

        window_layout->addWidget(central_container);
    }
};
}   // namespace client
}   // namespace tang