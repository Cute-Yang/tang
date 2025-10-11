#include "vote_item_view_model.h"
#include <QIcon>

namespace tang {
namespace client {

VoteItemViewModel::VoteItemViewModel(const QString& vote_column_, QObject* parent)
    : QAbstractTableModel(parent)
    , vote_column(vote_column_)
    , vote_items() {}

VoteItemViewModel::~VoteItemViewModel() {}

int VoteItemViewModel::rowCount(const QModelIndex& parent) const {
    return vote_items.count();
}

int VoteItemViewModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant VoteItemViewModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    int col = index.column();
    if (role == Qt::DecorationRole) {
        QIcon prefer_icon(":icons/images/shanhu.svg");
        return prefer_icon;
    } else if (role == Qt::DisplayRole) {
        return vote_items[row];
    }
    return QVariant{};
}

QVariant VoteItemViewModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal and role == Qt::DisplayRole) {
        return vote_column;
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}


bool VoteItemViewModel::contains(const QString& item) const {
    return vote_items.contains(item);
}


bool VoteItemViewModel::add_vote_item(const QString& item) {
    if (vote_items.contains(item)) {
        return false;
    }
    // here the first and last is all closed interval!!!
    int current_size = vote_items.size();
    beginInsertRows(QModelIndex(), current_size, current_size);
    vote_items.push_back(item);
    endInsertRows();
    return true;
}

size_t VoteItemViewModel::add_vote_items(const QStringList& items) {
    std::vector<int> add_indexes;
    add_indexes.reserve(items.size());
    for (size_t i = 0; i < items.size(); ++i) {
        if (!vote_items.contains(items[i])) {
            add_indexes.push_back(i);
        }
    }
    size_t failed_size = items.size() - vote_items.size();
    if (add_indexes.size() == 0) {
        return failed_size;
    }
    int first = vote_items.size();
    int last  = first + add_indexes.size() - 1;
    beginInsertRows(QModelIndex(), first, last);
    for (auto index : add_indexes) {
        vote_items.push_back(items[index]);
    }
    endInsertRows();

    return failed_size;
}

QString VoteItemViewModel::get_vote_item(size_t index) const {
    assert(index < vote_items.size());
    return vote_items[index];
}

QStringList& VoteItemViewModel::get_vote_items() {
    return vote_items;
}

void VoteItemViewModel::clear() {
    vote_items.clear();
    this->layoutChanged();
}

void VoteItemViewModel::swap_item(int i, int j) {
    if (i == j || i < 0 || j < 0 || i >= vote_items.size() || j >= vote_items.size()) {
        return;
    }
    std::swap(vote_items[i], vote_items[j]);
}

void VoteItemViewModel::delete_item(int i) {
    if (i < 0 || i >= vote_items.size()) {
        return;
    }
    beginRemoveRows(QModelIndex(), i, i);
    vote_items.removeAt(i);
    endRemoveRows();
}

void VoteItemViewModel::set_vote_items(const QStringList& items) {
    vote_items.assign(items.begin(), items.end());
    layoutChanged();
}


}   // namespace client
}   // namespace tang