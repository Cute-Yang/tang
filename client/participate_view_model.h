#pragma once
#include "common/status.h"
#include <QAbstractTableModel>


namespace tang {
namespace client {
QString get_vote_status_display_str(common::VoteStatus status);
struct VoteData {
    QString                creator;
    QString                create_time;
    QString                vote_topic;
    QList<QString>         vote_items;
    uint32_t               vote_id;
    common::VoteStatus     vote_status;
    common::VoteChoiceType vote_choice_type;
    bool                   processed{false};
};

class ParticipateViewModel : public QAbstractTableModel {
    Q_OBJECT
private:
    size_t          batch_size;
    QList<VoteData> vote_datas;
    QStringList     headers;

public:
    ParticipateViewModel(size_t batch_size_, const QStringList& headers_, QObject* parent);
    void      set_vote_datas(const QList<VoteData>& vote_datas_);
    VoteData& at(size_t i);
    size_t    size() { return vote_datas.size(); }

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;


    QVariant data(const QModelIndex& index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
};
}   // namespace client
}   // namespace tang