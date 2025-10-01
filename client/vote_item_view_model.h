#pragma once
#include <QAbstractTableModel>

namespace tang {
namespace client {
class VoteItemViewModel : public QAbstractTableModel {
    Q_OBJECT
private:
    QString     vote_column;
    QStringList vote_items;

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;


    QVariant data(const QModelIndex& index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

public:
    VoteItemViewModel(const QString& vote_column_ = "投票选项", QObject* parent = nullptr);
    ~VoteItemViewModel();

    bool add_vote_item(const QString& item);

    bool contains(const QString& item) const;

    size_t add_vote_items(const QStringList& items);

    void set_vote_items(const QStringList& items );

    // void delete_vote_item(size_t index);

    // void delete_vote_item(const QString& item);

    QString get_vote_item(size_t index) const;

    QStringList& get_vote_items();

    void swap_item(int i,int j);

    void delete_item(int i);

    void clear();
};

}   // namespace client
}   // namespace tang