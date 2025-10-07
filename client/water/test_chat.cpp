#include "chat_delegate.h"
#include "chat_view_model.h"
#include <QApplication>
#include <QListView>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QWidget>


using namespace tang::client;
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow  window;
    QWidget*     centralWidget = new QWidget;
    QVBoxLayout* layout        = new QVBoxLayout(centralWidget);

    QListView* listView = new QListView;
    // QStandardItemModel* model    = new QStandardItemModel;
    ChatViewModel* model    = new ChatViewModel(64);
    QStringList    messages = {"你好，今天过得怎么样？",
                               "我很好，谢谢！",
                               "这是一个带有<br><b>HTML</b> 格式的长消息示例。看看换行是否正常。",
                               "当然没问题！"};
    for (size_t i = 0; i < messages.size(); ++i) {
        ChatMessage msg = {messages[i], i %2 ? "layzdog":"黄蓉", "2025-09-11 13:28:49"};
        model->append_message(msg);
    }
    ChatDelegate* delegate = new ChatDelegate(listView);
    listView->setItemDelegate(delegate);
    listView->setModel(model);
    listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    layout->addWidget(listView);
    centralWidget->setLayout(layout);
    window.setCentralWidget(centralWidget);
    window.resize(400, 600);
    window.setWindowTitle("Qt6 气泡聊天框");



    // 添加测试消息

    auto addMessage = [&](const QString& text) {
        // QStandardItem* item = new QStandardItem;
        // item->setData(isSelf ? 1 : 0, MessageTypeRole);
        // item->setData(text, MessageTextRole);
        // item->setData(QDateTime::currentDateTime(), MessageTimeRole);
        // item->setData(avatar, MessageAvatarRole);
        // item->setSizeHint(
        //     delegate->sizeHint(QStyleOptionViewItem(), model->index(model->rowCount(), 0)));
        // model->appendRow(item);
    };

    // 示例消息
    QPixmap avatar1, avatar2;
    avatar1 = QPixmap(":icons/images/butterfly.svg");
    avatar1.scaled({36, 36}, Qt::KeepAspectRatio);
    // avatar1.fill(Qt::blue);

    avatar2 = QPixmap(":icons/images/yellow_butterfly.svg");
    avatar2.scaled({36, 36}, Qt::KeepAspectRatio);

    // avatar2.fill(Qt::red);


    window.show();

    return app.exec();
}