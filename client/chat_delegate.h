#pragma once

#include <QDateTime>
#include <QPainter>
#include <QStyledItemDelegate>



// // 自定义角色
// const int MessageTypeRole = Qt::UserRole + 1;     // 消息类型：0=收到，1=发送
// const int MessageTextRole = Qt::UserRole + 2;     // 消息内容
// const int MessageTimeRole = Qt::UserRole + 3;     // 时间 (QDateTime)
// const int MessageAvatarRole = Qt::UserRole + 4;   // 头像 QPixmap

namespace tang {
namespace client {


class ChatDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ChatDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QPixmap m_defaultAvatar;   // 默认头像
};
}   // namespace client
}   // namespace tang
