#include <QApplication>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QDateTime>
#include <QTextEdit>
#include <QScrollBar>
#include <QTimer>

// 每条消息的自定义控件
class ChatMessageWidget : public QWidget {
public:
    ChatMessageWidget(const QString &nickname, const QString &text,
                      const QString &avatarPath, bool isSelf, QWidget *parent = nullptr)
        : QWidget(parent) {

        // 主布局
        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(8, 4, 8, 4);
        mainLayout->setSpacing(8);

        // 头像
        QLabel *avatarLabel = new QLabel;
        avatarLabel->setFixedSize(36, 36);
        avatarLabel->setStyleSheet("QLabel { border-radius: 18px; border: 1px solid #ddd; }");
        QPixmap avatar(avatarPath);
        if (!avatar.isNull()) {
            avatarLabel->setPixmap(avatar.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            avatarLabel->setText("A");
            avatarLabel->setAlignment(Qt::AlignCenter);
        }

        // 右侧：时间 + 昵称 + 气泡
        QVBoxLayout *rightLayout = new QVBoxLayout;
        rightLayout->setSpacing(2);

        QLabel *timeLabel = new QLabel(QDateTime::currentDateTime().toString("HH:mm"));
        timeLabel->setStyleSheet("color: #999; font-size: 10px;");
        timeLabel->setMaximumWidth(120);

        QLabel *nameLabel = new QLabel(nickname);
        nameLabel->setStyleSheet("color: #555; font-size: 12px;");
        nameLabel->setMaximumWidth(120);

        QTextEdit *bubble = new QTextEdit;
        bubble->setPlainText(text);
        bubble->setReadOnly(true);
        bubble->setFrameStyle(QFrame::NoFrame);
        bubble->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        bubble->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        // 设置最大宽度以触发换行（模拟微信最大宽度）
        bubble->document()->setTextWidth(240);

        // ⚠️ 关键：先设置文本，再计算高度
        QSize size = bubble->document()->size().toSize();
        bubble->setFixedHeight(size.height() + 10);  // + padding

        // 微信绿色：#07C160
        QString bgColor = isSelf ? "#07C160" : "#f0f0f0";
        QString textColor = isSelf ? "white" : "black";

        bubble->setStyleSheet(QString(R"(
            QTextEdit {
                background-color: %1;
                color: %2;
                padding: 8px;
                border-radius: 8px;
                font-size: 14px;
                border: none;
            }
        )").arg(bgColor, textColor));

        rightLayout->addWidget(timeLabel);
        rightLayout->addWidget(nameLabel);
        rightLayout->addWidget(bubble);

        if (isSelf) {
            mainLayout->addStretch();
            mainLayout->addLayout(rightLayout);
            mainLayout->addWidget(avatarLabel);
        } else {
            mainLayout->addWidget(avatarLabel);
            mainLayout->addLayout(rightLayout);
            mainLayout->addStretch();
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.resize(380, 600);
    window.setWindowTitle("微信风格聊天界面 - 优化版");
    window.setStyleSheet("background: white;");

    auto *listWidget = new QListWidget(&window);
    listWidget->setStyleSheet(
        "QListWidget { background: white; border: none; }"
        "QListWidget::item { border: none; }"
    );
    listWidget->setVerticalScrollMode(QListWidget::ScrollPerPixel);

    auto *button = new QPushButton("发送新消息", &window);

    auto *layout = new QVBoxLayout(&window);
    layout->addWidget(listWidget);
    layout->addWidget(button);

    window.setLayout(layout);

    // 添加消息函数
    auto addMessage = [&](const QString &nick, const QString &text, const QString &avatar, bool isSelf) {
        auto *item = new QListWidgetItem(listWidget);
        item->setSizeHint(QSize(400, 80)); // 初始高度，会被 widget 覆盖

        auto *widget = new ChatMessageWidget(nick, text, avatar, isSelf);
        listWidget->setItemWidget(item, widget);

        // 滚动到底部
        QTimer::singleShot(0, listWidget, [listWidget]() {
            listWidget->verticalScrollBar()->setValue(listWidget->verticalScrollBar()->maximum());
        });
    };

    // 测试消息
    addMessage("小明", "你好啊，今天过得怎么样？", ":/avatar_other.png", false);
    addMessage("我", "我很好，谢谢！\n这是第二行\n第三行测试长文本是否自动换行并保持美观。", ":/avatar_self.png", true);
    addMessage("小明", "这是一段很长的文本，用来测试自动换行和布局稳定性，看看效果如何。", ":/avatar_other.png", false);
    addMessage("我", "收到！", ":/avatar_self.png", true);

    QObject::connect(button, &QPushButton::clicked, [&]() {
        static int n = 0;
        addMessage("我", QString("第 %1 条消息\n换行测试\n长文本自动换行正常显示").arg(++n), ":/avatar_self.png", true);
    });

    window.show();

    return app.exec();
}