#include "chat_delegate.h"
#include "chat_role.h"
#include "client_singleton.h"
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QDateTime>
#include <QFontMetrics>
#include <QPainterPath>
#include <QTextDocument>

namespace tang {
namespace client {

int compute_doc_width(const QFontMetricsF& metric, const QString& text) {
    auto  split_datas = text.split("<br>");
    qreal max_width   = 0.0f;
    for (auto& s : split_datas) {
        auto w    = metric.horizontalAdvance(s);
        max_width = max_width > w ? max_width : w;
    }
    return static_cast<int>(max_width + 0.5f);
}

ChatDelegate::ChatDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {
    m_defaultAvatar = QPixmap(36, 36);
    m_defaultAvatar.fill(Qt::blue);   // 简单蓝色占位图
}

QSize ChatDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    Q_UNUSED(option)
    QString       text     = index.data(ChatRole::MessageTextRole).toString();
    QString       nickname = index.data(ChatRole::NicknameTextRole).toString();
    QString       time     = index.data(ChatRole::MessageTimeRole).toString();
    QTextDocument doc;
    doc.setHtml(text);
    doc.setTextWidth(120);
    qreal head_height       = 36;
    qreal other_info_height = 10;
    qreal height            = doc.size().height() + head_height + other_info_height;
    return QSize(300, static_cast<int>(height));
}

void ChatDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                         const QModelIndex& index) const {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QString text     = index.data(ChatRole::MessageTextRole).toString();
    QString nickname = index.data(ChatRole::NicknameTextRole).toString();
    QString time_str = index.data(ChatRole::MessageTimeRole).toString();

    // our max text len is 100,just ok!
    QFontMetricsF metric(painter->font());
    int           text_width = compute_doc_width(metric, text) + 32;

    auto&   current_user_info = ClientSingleton::get_cache_user_info_instance();
    bool    is_self           = nickname == current_user_info.user_name;
    QPixmap avatar =
        is_self ? QPixmap(":icons/images/chat_self.svg") : QPixmap(":icons/images/chat_other.svg");
    constexpr int icon_size = 27;
    avatar                  = avatar.scaled({icon_size, icon_size});
    // 背景色和气泡颜色
    QColor bubbleColor      = is_self ? QColor("#DCF8C6") : QColor("#FFFFFF");
    QColor textColor        = is_self ? Qt::black : Qt::black;
    QColor other_info_color = QColor("#999999");
    QRect  rect             = option.rect;
    int    header_y         = rect.top() + 5;
    QPoint other_header_pos(rect.left() + 10, header_y);
    QPoint self_header_pos(rect.right() - 50, header_y);
    QPoint avatarPos = is_self ? self_header_pos : other_header_pos;
    // 绘制头像
    QPainterPath avatarPath;
    avatarPath.addRoundedRect(avatarPos.x(), avatarPos.y(), icon_size, icon_size, 0, 0);
    painter->setClipPath(avatarPath);
    painter->drawPixmap(avatarPos, avatar);
    painter->setClipping(false);

    int text_left;
    int text_right;
    if (is_self) {
        text_right = rect.right() - 60;
        text_left  = text_right - text_width;
    } else {
        text_left  = rect.left() + 60;
        text_right = text_left + text_width;
    }

    QTextDocument doc;
    doc.setHtml(QString("<font color='%1'>%2</font>").arg(textColor.name()).arg(text));
    doc.setTextWidth(text_width);
    QRectF textRect(text_left, rect.top(), text_width, doc.size().height());
    // 绘制气泡
    QPainterPath bubblePath;
    bubblePath.addRoundedRect(textRect, 10, 10);

    // 添加小三角（指向头像）
    QPointF tip[3];
    if (is_self) {
        tip[0] = QPointF(textRect.right() - 10, textRect.bottom() - 15);
        tip[1] = QPointF(textRect.right(), textRect.bottom() - 10);
        tip[2] = QPointF(textRect.right() - 10, textRect.bottom() - 5);
    } else {
        tip[0] = QPointF(textRect.left() + 10, textRect.bottom() - 15);
        tip[1] = QPointF(textRect.left(), textRect.bottom() - 10);
        tip[2] = QPointF(textRect.left() + 10, textRect.bottom() - 5);
    }
    bubblePath.addPolygon(QPolygonF(QVector<QPointF>::fromList(QVector<QPointF>(tip, tip + 3))));

    bubblePath.closeSubpath();

    painter->fillPath(bubblePath, bubbleColor);
    painter->setPen(QPen(QColor("#CCCCCC"), 1));
    painter->drawPath(bubblePath);

    // 绘制文本
    QAbstractTextDocumentLayout::PaintContext ctx;

    ctx.palette.setColor(QPalette::Text, textColor);
    painter->translate(textRect.topLeft() + QPointF(10, 0));
    doc.documentLayout()->draw(painter, ctx);
    painter->translate(-textRect.topLeft().x() - 10, -textRect.topLeft().y());

    auto font = painter->font();
    font.setPointSize(9);
    painter->setFont(font);
    QRect other_info_rect(
        is_self ? textRect.right() - 200 : textRect.left(), textRect.bottom() + 2, 200, 15);
    painter->setPen(other_info_color);
    QString other_info = QString("%1 (%2)").arg(nickname, time_str);
    painter->drawText(
        other_info_rect, Qt::AlignVCenter | (is_self ? Qt::AlignRight : Qt::AlignLeft), other_info);

    painter->restore();
}
}   // namespace client
}   // namespace tang