#include "vote_result_view_model.h"
#include <QIcon>

namespace tang {
namespace client {

static QIcon vote_item_icon(":icons/images/leaf.svg");
VoteResultViewModel::VoteResultViewModel(const QString& vote_item_col_name_,
                                         const QString& vote_count_col_name_,
                                         const QString& vote_count_percent_col_name_,
                                         QObject*       parent)
    : QAbstractTableModel(parent)
    , vote_items()
    , vote_counts()
    , vote_item_col_name(vote_item_col_name_)
    , vote_count_col_name(vote_count_col_name_)
    , vote_count_percent_col_name(vote_count_percent_col_name_)
    , total_count(0) {}


// RAII
VoteResultViewModel::~VoteResultViewModel() {}

void VoteResultViewModel::compute_total_count() {
    total_count = 0;
    for (size_t i = 0; i < vote_counts.size(); ++i) {
        if (vote_counts[i] < 0) {
            qDebug() << "unexpected vote count value:" << vote_counts[i] << " for "
                     << vote_items[i];
            continue;
        }
        total_count += vote_counts[i];
    }
}

void VoteResultViewModel::set_vote_items(const QList<QString>& vote_items_) {
    // if new vote items is empty,print a message
    if (vote_items_.size() == 0) {
        qDebug() << "new vote items is empty!";
    }
    vote_items.resize(vote_items_.size());
    vote_items.assign(vote_items_.begin(), vote_items_.end());
}

void VoteResultViewModel::set_vote_counts(const QList<int>& vote_counts_) {
    if (vote_counts_.size() == 0) {
        qDebug() << "new vote counts is empty!";
    }
    vote_counts.resize(vote_counts_.size());
    vote_counts.assign(vote_counts_.begin(), vote_counts_.end());
    this->compute_total_count();
}

QVariant VoteResultViewModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            // do not show anything for the first column header,it is a icon!
            return vote_item_col_name;
        } else if (section == 1) {
            // show the vote item column
            return vote_count_col_name;
        } else if (section == 2) {
            // percent!
            return vote_count_percent_col_name;
        }
    }
    // using the default
    // for the vertical
    return QAbstractTableModel::headerData(section, orientation, role);
}

int VoteResultViewModel::rowCount(const QModelIndex& parent) const {
    assert(vote_items.size() == vote_counts.size());
    // the last item is vote_counts
    return vote_counts.size() + 1;
}

int VoteResultViewModel::columnCount(const QModelIndex& parent) const {
    return 3;
}

QVariant VoteResultViewModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    int col = index.column();
    if (role == Qt::DisplayRole) {
        if (row == vote_items.count()) {
            if(col == 0){
                return "total";
            }else if(col == 1){
                return total_count;
            }else if(col == 2){
                return "100%";
            }
        } else {
            if (col == 0) {
                // vote item
                return vote_items[row];
            } else if (col == 1) {
                // if specify the display role and decoration role
                //  will show the icon and text
                //  like icon text ^_^
                return vote_counts[row];
            } else if (col == 2) {
                double percent = static_cast<double>(vote_counts[row]) / total_count * 100;
                // save 2 precision
                return QString("%1%").arg(percent , 0, 'f', 2);
            }
        }
    } else if (role == Qt::DecorationRole && col == 0) {
        return vote_item_icon;
    }else if(role == Qt::TextAlignmentRole && col ==1 ){
        return Qt::AlignCenter;
    }
    return {};
}


}   // namespace client
}   // namespace tang