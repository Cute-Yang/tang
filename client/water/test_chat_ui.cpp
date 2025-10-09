#include <QApplication>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QPainter>
#include <QPushButton>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>


// 自定义委托：负责绘制每条消息
class ChatDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    ChatDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent) {}

    // 计算每条消息所需的高度
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        auto data = index.data(Qt::UserRole);
        if (!data.canConvert<QMap<QString, QString>>()) return QSize(100, 80);

        auto    msg    = data.value<QMap<QString, QString>>();
        QString text   = msg["text"];
        bool    isSelf = msg["isSelf"] == "true";

        // 创建临时 QTextEdit 计算高度
        QTextEdit temp;
        temp.setPlainText(text);
        temp.setFrameStyle(QFrame::NoFrame);
        temp.setStyleSheet("QTextEdit { background: transparent; }");
        temp.document()->setTextWidth(isSelf ? 240 : 240);   // 最大宽度

        QSize size   = temp.document()->size().toSize();
        int   height = size.height() + 60;   // + 昵称、时间、头像等

        return QSize(option.rect.width(), height + 16);   // + 外边距
    }

    // 绘制消息（我们不自己画，而是用 widget）
    void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const override {
        // 我们使用 setItemWidget，所以不需要手动 paint
    }
};

// 消息控件（和之前一样，但更轻量）
class ChatMessageWidget : public QWidget {
    Q_OBJECT

public:
    ChatMessageWidget(const QString& nickname, const QString& text, const QString& avatarPath,
                      bool isSelf, QWidget* parent = nullptr)
        : QWidget(parent)
        , isSelf(isSelf) {

        QHBoxLayout* mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(8, 8, 8, 8);
        mainLayout->setSpacing(8);

        QLabel* avatarLabel = new QLabel;
        avatarLabel->setFixedSize(36, 36);
        avatarLabel->setStyleSheet("QLabel { border-radius: 18px; border: 1px solid #ddd; }");
        QPixmap avatar(avatarPath);
        if (!avatar.isNull()) {
            avatarLabel->setPixmap(
                avatar.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            avatarLabel->setText("A");
            avatarLabel->setAlignment(Qt::AlignCenter);
        }

        QVBoxLayout* rightLayout = new QVBoxLayout;
        rightLayout->setSpacing(2);

        QLabel* timeLabel = new QLabel(QDateTime::currentDateTime().toString("HH:mm"));
        timeLabel->setStyleSheet("color: #999; font-size: 10px;");
        timeLabel->setMaximumWidth(120);

        QLabel* nameLabel = new QLabel(nickname);
        nameLabel->setStyleSheet("color: #555; font-size: 12px;");
        nameLabel->setMaximumWidth(120);

        QTextEdit* bubble = new QTextEdit;
        bubble->setPlainText(text);
        bubble->setReadOnly(true);
        bubble->setFrameStyle(QFrame::NoFrame);
        bubble->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        bubble->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        bubble->document()->setTextWidth(240);
        QSizeF docSize = bubble->document()->size();
        bubble->setFixedHeight(docSize.height() + 10);

        QString bgColor   = isSelf ? "#07C160" : "#f0f0f0";
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
        )")
                                  .arg(bgColor, textColor));

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

        this->adjustSize();
    }

private:
    bool isSelf;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.resize(380, 600);
    window.setWindowTitle("微信风格聊天 - 终极稳定版");
    window.setStyleSheet("background: white;");

    auto* listView = new QListView(&window);
    auto* model    = new QStandardItemModel(listView);

    listView->setModel(model);
    listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setStyleSheet("QListView { background: white; border: none; }");

    auto* delegate = new ChatDelegate(listView);
    listView->setItemDelegate(delegate);

    auto* button = new QPushButton("发送新消息", &window);

    auto* layout = new QVBoxLayout(&window);
    layout->addWidget(listView);
    layout->addWidget(button);

    window.setLayout(layout);

    // 添加消息
    auto addMessage =
        [&](const QString& nick, const QString& text, const QString& avatar, bool isSelf) {
            auto* item = new QStandardItem();
            item->setSizeHint(QSize(360, 80));   // 初始高度

            // 存储数据
            QMap<QString, QString> msgData;
            msgData["nickname"] = nick;
            msgData["text"]     = text;
            msgData["avatar"]   = avatar;
            msgData["isSelf"]   = isSelf ? "true" : "false";
            item->setData(QVariant::fromValue(msgData), Qt::UserRole);

            model->appendRow(item);

            // 创建 widget 并设置
            auto* widget = new ChatMessageWidget(nick, text, avatar, isSelf);
            listView->setIndexWidget(model->indexFromItem(item), widget);

            // ⚠️ 关键：延迟更新高度
            QTimer::singleShot(10, [=]() {
                widget->adjustSize();
                item->setSizeHint(widget->sizeHint() + QSize(0, 10));
                listView->doItemsLayout();   // 强制重新布局
                listView->scrollToBottom();
            });
        };

    // 测试消息
    addMessage("小明", "你好啊，今天过得怎么样？", ":/avatar_other.png", false);
    addMessage("我",
               "我很好，谢谢！\n这是第二行\n第三行测试长文本是否自动换行并保持美观。",
               ":/avatar_self.png",
               true);
    addMessage("小明",
               "这是一段很长的文本，用来测试自动换行和布局稳定性，看看效果如何。",
               ":/avatar_other.png",
               false);
    addMessage("我", "收到！", ":/avatar_self.png", true);

    QObject::connect(button, &QPushButton::clicked, [&]() {
        static int n = 0;
        addMessage(
            "我",
            QString("第 %1 条消息\n换行测试\n长文本自动换行\n第四行\n第五行\n第六行").arg(++n),
            ":/avatar_self.png",
            true);
    });

    window.show();

    return app.exec();
}

#include "test_chat_ui.moc"