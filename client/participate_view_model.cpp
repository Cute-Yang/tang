#include "participate_view_model.h"
#include <QIcon>

namespace tang {
namespace client {

constexpr int vote_id_col          = 0;
constexpr int creator_col          = 1;
constexpr int create_time_col      = 2;
constexpr int vote_choice_type_col = 3;
constexpr int vote_status_col      = 4;
constexpr int check_col            = 5;

ParticipateViewModel::ParticipateViewModel(size_t batch_size_, const QStringList& headers_,
                                           QObject* parent)
    : QAbstractTableModel(parent)
    , batch_size(batch_size_)
    , vote_datas()
    , headers(headers_) {}


void ParticipateViewModel::set_vote_datas(const QList<VoteData>& vote_datas_) {
    vote_datas = vote_datas_;
}
int ParticipateViewModel::rowCount(const QModelIndex& parent) const {
    return vote_datas.size();
}

int ParticipateViewModel::columnCount(const QModelIndex& parent) const {
    return headers.size();
}

QVariant ParticipateViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return {};
    }
    int   row       = index.row();
    auto& vote_data = vote_datas[row];
    int   col       = index.column();
    if (role == Qt::DisplayRole) {
        if (col == vote_id_col) {
            return vote_data.vote_id;
        } else if (col == creator_col) {
            return vote_data.creator;
        } else if (col == create_time_col) {
            return vote_data.create_time;
        } else if (col == vote_choice_type_col) {
            return vote_data.vote_choice_type == common::VoteChoiceType::kSingleChoice
                       ? QString("单选")
                       : QString("多选");
        } else if (col == vote_status_col) {
            switch (vote_data.vote_status) {
            case common::VoteStatus::kReady: return QString("待投票"); break;
            case common::VoteStatus::kFinished: return QString("已完成"); break;
            default: return QString("已作废"); break;
            }
        } else if (col == check_col) {
            return QString("查看");
        }
    } else if (role == Qt::DecorationRole) {
        QString icon_file;
        if (col == vote_status_col) {
            if (vote_data.vote_status == common::VoteStatus::kReady) {
                icon_file = ":icons/images/happy.svg";
            } else if (vote_data.vote_status == common::VoteStatus::kFinished) {
                icon_file = ":icons/images/cheers.svg";
            } else {
                icon_file = ":icons/images/wuwu.svg";
            }
        } else if (col == check_col) {
            icon_file = ":icons/images/yezi.svg";
        } else if (col == create_time_col) {
            icon_file = ":icons/images/time.svg";

        } else if (col == vote_choice_type_col) {
            icon_file = ":icons/images/blue_vote.svg";

        } else if (col == creator_col) {
            icon_file = ":icons/images/mifeng.svg";
        }
        if (!icon_file.isEmpty()) {
            return QIcon(icon_file);
        }
    }
    return {};
}

QVariant ParticipateViewModel::headerData(int section, Qt::Orientation orientation,
                                          int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section < headers.size()) {
            return headers[section];
        }
    }
    return {};
}

}   // namespace client
}   // namespace tang