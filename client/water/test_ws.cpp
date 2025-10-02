#include <QCoreApplication>
#include <QDebug>
#include <QWebSocket>

// set a global websocket instance is all you need!
int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    QWebSocket socket;

    // 连接成功信号
    QObject::connect(&socket, &QWebSocket::connected, [&]() {
        qDebug() << "Connected!";
        socket.sendTextMessage("Hello from Qt!");
    });

    // 接收消息
    QObject::connect(&socket, &QWebSocket::textMessageReceived, [&](const QString& msg) {
        qDebug() << "Received:" << msg;

        // 收到消息后退出（可选）
        // QCoreApplication::quit();
    });



    // 连接到测试服务器（回显服务）
    socket.open(QUrl("ws://localhost:9000/api/v1/chat?user_id=32&user_name=戈薇"));

    return app.exec();
}