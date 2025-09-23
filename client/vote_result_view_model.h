#pragma once
#include <QAbstractTableModel>

namespace tang {
namespace client {

// define a static icon object here!
class VoteResultViewModel : public QAbstractTableModel {
    Q_OBJECT
private:
    // the vote items
    QList<QString> vote_items;
    // vote counts
    QList<int> vote_counts;
    // column names
    QString vote_item_col_name;
    QString vote_count_col_name;
    QString vote_count_percent_col_name;
    int     total_count;

    void compute_total_count();

public:
    VoteResultViewModel(const QString& vote_item_col_name_          = "item",
                        const QString& vote_count_col_name_         = "count",
                        const QString& vote_count_percent_col_name_ = "percent",
                        QObject*       parent                       = nullptr);
    ~VoteResultViewModel();

    void set_vote_items(const QList<QString>& vote_items_);

    void set_vote_counts(const QList<int>& vote_counts_);

    // the override funcs
protected:
    int      rowCount(const QModelIndex& parent) const override;
    int      columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
};


}   // namespace client
}   // namespace tang