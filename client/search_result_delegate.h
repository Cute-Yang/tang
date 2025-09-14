#pragma once
#include <QStyledItemDelegate>


namespace tang {
namespace client {


class SearchResultDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    SearchResultDelegate(QObject* parent = nullptr);

    // 重写绘制函数
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
};
}   // namespace client
}   // namespace tang