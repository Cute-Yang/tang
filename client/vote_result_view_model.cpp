#include "vote_result_view_model.h"
#include "util.h"
#include <QIcon>


namespace tang {
namespace client {
static QIcon vote_item_icon(":icons/images/leaf.svg");
VoteItemCountViewModel::VoteItemCountViewModel(const QStringList& header_names_, QObject* parent)
    : QAbstractTableModel(parent)
    , header_names(header_names_)
    , total_count(0) {}


// RAII
VoteItemCountViewModel::~VoteItemCountViewModel() {}

void VoteItemCountViewModel::compute_total_count() {
    total_count = 0;
    for (size_t i = 0; i < vote_item_infos.size(); ++i) {
        auto c = vote_item_infos[i].vote_item_count;
        total_count += c;
    }
}

void VoteItemCountViewModel::set_vote_infos(std::span<VoteItemCountInfo> vote_count_infos_) {
    vote_item_infos = vote_count_infos_;
    compute_total_count();
}

QVariant VoteItemCountViewModel::headerData(int section, Qt::Orientation orientation,
                                            int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_names[section];
    }
    // using the default
    // for the vertical
    return QAbstractTableModel::headerData(section, orientation, role);
}

int VoteItemCountViewModel::rowCount(const QModelIndex& parent) const {
    return vote_item_infos.size() + 1;
}

int VoteItemCountViewModel::columnCount(const QModelIndex& parent) const {
    return header_names.size();
}

QVariant VoteItemCountViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return false;
    }
    int  row = index.row();
    int  col = index.column();
    auto n   = vote_item_infos.size();
    if (role == Qt::DisplayRole) {
        if (row == vote_item_infos.size()) {
            if (col == 0) {
                return "total";
            } else if (col == 1) {
                return total_count;
            } else if (col == 2) {
                return "100%";
            }
        } else if (row < n) {
            auto& vote_item_info = vote_item_infos[row];
            if (col == 0) {
                // vote item
                return vote_item_info.vote_item;
            } else if (col == 1) {
                return vote_item_info.vote_item_count;
            } else if (col == 2) {
                double percent =
                    static_cast<double>(vote_item_info.vote_item_count) / total_count * 100;
                // save 2 precision
                return QString("%1%").arg(percent, 0, 'f', 2);
            } else if (col == 3) {
                return get_vote_item_status_display_str(vote_item_info.vote_item_status);
            }
        }
    } else if (role == Qt::DecorationRole && col == 0) {
        return vote_item_icon;
    } else if (role == Qt::TextAlignmentRole && col == 1) {
        return Qt::AlignCenter;
    }
    return {};
}

VoteResultHistoryViewModel::VoteResultHistoryViewModel(const QStringList& header_names_,
                                                       QObject*           parent)
    : QAbstractTableModel(parent)
    , header_names(header_names_) {

    vote_result_historys = {{3,
                             "周琳",
                             "2025-03-21 13:28:49",
                             "今天去哪爬山",
                             {
                                 {"马峦山", 1, common::VoteItemStatus::kNotSelected},
                                 {"梅沙尖", 2, common::VoteItemStatus::kSelected},
                                 {"梧桐山", 1, common::VoteItemStatus::kNotSelected},

                             },
                             common::VoteChoiceType::kSingleChoice

    }};
}

VoteResultHistoryViewModel::~VoteResultHistoryViewModel() {}
int VoteResultHistoryViewModel::rowCount(const QModelIndex& parent) const {
    return vote_result_historys.size();
}

int VoteResultHistoryViewModel::columnCount(const QModelIndex& parent) const {
    return header_names.size();
}

QVariant VoteResultHistoryViewModel::headerData(int section, Qt::Orientation orientation,
                                                int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header_names[section];
    }
    // using the default
    // for the vertical
    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant VoteResultHistoryViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return {};
    }
    int row = index.row();
    int col = index.column();

    auto& vote_history = vote_result_historys.at(row);
    if (role == Qt::DisplayRole) {
        if (col == 0) {
            return vote_history.vote_id;
        } else if (col == 1) {
            return vote_history.creator;
        } else if (col == 2) {
            return vote_history.create_time;
        } else if (col == 3) {
            return vote_history.vote_topic;
        } else if (col == 4) {
            return get_choice_type_display_str(vote_history.choice_type);
        } else if (col == 5) {
            return QString("查看");
        }
    } else if (role == Qt::DecorationRole) {
        QString icon_file;
        if (col == 1) {
            icon_file = ":icons/images/mifeng.svg";
        } else if (col == 2) {
            icon_file = ":icons/images/time.svg";
        } else if (col == 3) {
            icon_file = ":icons/images/guihua.svg";
        } else if (col == 4) {

            icon_file = ":icons/images/blue_vote.svg";

        } else if (col == 5) {
            icon_file = ":icons/images/yezi.svg";
        }
        if (!icon_file.isEmpty()) {
            return QIcon(icon_file);
        }
    }
    return {};
}

VoteResultHistory& VoteResultHistoryViewModel::at(size_t i) {
    return vote_result_historys[i];
}


std::span<VoteItemCountInfo> VoteResultHistoryViewModel::get_vote_info(size_t i) {
    auto&  vote_history = vote_result_historys[i];
    auto   p            = vote_history.vote_item_infos.data();
    size_t n            = vote_history.vote_item_infos.size();
    return {p, n};
}

}   // namespace client
}   // namespace tang