#pragma once
#include "common/status.h"
#include <QAbstractTableModel>
#include <span>

namespace tang {
namespace client {

// define a static icon object here!
struct VoteItemCountInfo {
    QString                vote_item;
    uint32_t               vote_item_count;
    common::VoteItemStatus vote_item_status;
};


struct VoteResultHistory {
    uint32_t vote_id;
    // people
    QString creator;
    // time
    QString create_time;
    // topic
    QString                      vote_topic;
    QList<VoteItemCountInfo>     vote_item_infos;
    tang::common::VoteChoiceType choice_type;
};

class VoteItemCountViewModel : public QAbstractTableModel {
    Q_OBJECT
private:
    std::span<VoteItemCountInfo> vote_item_infos;
    QStringList                  header_names;
    uint32_t                     total_count;
    void                         compute_total_count();

public:
    VoteItemCountViewModel(const QStringList& header_names_ = {"选项", "计数", "占比", "是否选中"},
                           QObject*           parent        = nullptr);
    ~VoteItemCountViewModel();

    void set_vote_infos(std::span<VoteItemCountInfo> vote_item_infos_);

    // the override funcs
protected:
    int      rowCount(const QModelIndex& parent) const override;
    int      columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
};


class VoteResultHistoryViewModel : public QAbstractTableModel {
    Q_OBJECT
private:
    std::vector<VoteResultHistory> vote_result_historys;
    QStringList                    header_names;

protected:
    int      rowCount(const QModelIndex& parent) const override;
    int      columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

public:
    VoteResultHistoryViewModel(const QStringList& header_names_, QObject* parent = nullptr);
    ~VoteResultHistoryViewModel();

    VoteResultHistory& at(size_t index);

    std::span<VoteItemCountInfo> get_vote_info(size_t index);
};



}   // namespace client
}   // namespace tang