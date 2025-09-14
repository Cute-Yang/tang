#include "ElaText.h"
#include "ElaLineEdit.h"
// #include "ElaPromotionCard.h"
#include "QLabel"
#include <QHBoxLayout>
#include <QToolButton>
namespace tang {
namespace client {
class SigninUi {
public:
    // ElaPromotionCard* card;
    // QLabel* img_label;
    ElaText* user_text;
    ElaText* password_text;

    ElaLineEdit* user_line_edit;
    ElaLineEdit* password_line_edit;

    QToolButton* sign_in_button;
    QToolButton* sign_up_button;

    QAction* password_eye_action;

void setup_ui(QWidget* window){
    QVBoxLayout* window_layout = new QVBoxLayout(window);
    window_layout->setContentsMargins({0,0,0,0});
    QWidget* login_container = new QWidget(window);

    //the object name should equal to the style sheet's name
    login_container->setObjectName("login_container");
    //            background-color:#F5F5DC;
    // set the background!
    QString login_style = R"style(
        #login_container{
            background-color:#F5F5DC;
        }
    )style";
    login_container->setStyleSheet(login_style);
    // login_container->setStyleSheet("background-color:#009ACD;");
    QVBoxLayout* login_container_layout = new QVBoxLayout(login_container);
    login_container_layout->addStretch();

    QWidget* user_container = new QWidget(window);
    QHBoxLayout* user_layout = new QHBoxLayout(user_container);
    const int text_width = 80;
    user_text = new ElaText(window);
    user_text->setFixedWidth(text_width);
    user_text->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    user_text->setText("用户名");
    auto font = user_text->font();
    font.setBold(false);
    font.setFamily("SimSun");
    font.setPointSize(10);
    font.setBold(true);
    user_text->setFont(font);
    user_layout->addStretch();
    user_layout->addWidget(user_text);
    user_layout->addSpacing(8);

    const int line_edit_width = 200;

    user_line_edit = new ElaLineEdit(window);
    user_line_edit->setFixedWidth(line_edit_width);
    user_line_edit->setFont(font);
    user_layout->addWidget(user_line_edit);
    user_layout->addStretch();
    login_container_layout->addWidget(user_container);

    QWidget* password_container = new QWidget(window);
    QHBoxLayout* password_layout = new QHBoxLayout(password_container);
    password_layout->addStretch();
    password_text = new ElaText(window);
    password_text->setFixedWidth(text_width);
    password_text->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    password_text->setText("密码");
    password_text->setFont(font);
    password_layout->addWidget(password_text);
    password_layout->addSpacing(8);

    password_line_edit = new ElaLineEdit(window);
    password_line_edit->setFixedWidth(line_edit_width);
    password_line_edit->setFont(font);
    password_line_edit->setEchoMode(ElaLineEdit::Password);
    password_eye_action = new QAction(QIcon(":/icons/images/hide_password.svg"),"show/hide",window);
    password_eye_action->setCheckable(true);
    password_eye_action->setChecked(true);
    password_line_edit->addAction(password_eye_action,ElaLineEdit::TrailingPosition);
    password_layout->addWidget(password_line_edit);
    password_layout->addSpacing(2);
    // hide_password_button = new ElaToolButton(window);
    // //default -> hide
    // hide_password_button->setCheckable(true);
    // hide_password_button->setChecked(true);

    
    // hide_password_button->setIcon(QIcon(":/icons/images/hide_password.svg"));
    // hide_password_button->setIconSize(QSize(16,16));
    password_layout->addStretch();

    // password_layout->addWidget(hide_password_button);

    login_container_layout->addWidget(password_container);
    // login_container_layout->addStretch();


    QWidget* sign_container = new QWidget(window);
    QHBoxLayout* sign_layout = new QHBoxLayout(sign_container);
    sign_layout->addStretch();
    
    // const int button_width = 60;
    QSize sign_button_size(80,30);
    sign_in_button = new QToolButton(window);
    sign_in_button->setFixedSize(sign_button_size);
    sign_in_button->setObjectName("sign_in_button");
    
    QString signin_button_style = R"(
        QToolButton#sign_in_button{
            background-color:#20B2AA;
            font-size: 12px;
            font-weight: normal;
            border: 2px solid #b9b9b9; 
            border-radius: 3px;
            qproperty-icon: url(":icons/images/signin.svg") center;
            qproperty-iconSize: 16px 16px;
            padding-left: 15px;
        }
    )";
    sign_in_button->setStyleSheet(signin_button_style);
    sign_in_button->setText("登录");
    font.setPointSize(9);
    sign_in_button->setFont(font);
    sign_in_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sign_layout->addWidget(sign_in_button);
    sign_layout->addSpacing(40);
    
        
    QString signup_button_style = R"(
        QToolButton#sign_up_button{
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
    sign_up_button = new QToolButton(window);
    sign_up_button->setText("注册");
    sign_up_button->setFixedSize(sign_button_size);
    sign_up_button->setObjectName("sign_up_button");
    sign_up_button->setStyleSheet(signup_button_style);
    sign_up_button->setFont(font);
    sign_up_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    // sign_up_button->setFixedWidth(button_width);
    sign_layout->addWidget(sign_up_button);
    
    sign_layout->addStretch();

    login_container_layout->addWidget(sign_container);
    login_container_layout->addStretch();

    
    
    window_layout->addWidget(login_container);
    
}

};
}   // namespace client
}   // namespace tang