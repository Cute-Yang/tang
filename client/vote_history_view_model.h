#pragma once
#include "common/status.h"
#include <QAbstractTableModel>


namespace tang {
namespace client {

struct VoteHistory {
    uint32_t vote_id;
    // people
    QString creator;
    // time
    QString create_time;
    // topic
    QString                      vote_topic;
    QStringList                  vote_items;
    QStringList voters;
    tang::common::VoteChoiceType choice_type;
};

class VoteHistoryViewModel : public QAbstractTableModel {
    Q_OBJECT;

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;


    QVariant data(const QModelIndex& index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    QStringList              col_names;
    size_t                   batch_size;
    std::vector<VoteHistory> vote_history;

public:
    VoteHistoryViewModel(const QStringList& col_names, size_t batch_size_ = 10,
                         QObject* parent = nullptr);
    void append(VoteHistory&& item);
    void append(const VoteHistory& item);

    VoteHistory& at(size_t i);
};
}   // namespace client
}   // namespace tang