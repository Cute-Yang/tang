#include "vote_history_view_model.h"
#include <QIcon>

using namespace tang::common;
namespace tang {
namespace client {
VoteHistoryViewModel::VoteHistoryViewModel(const QStringList& col_names_, size_t batch_size_,
                                           QObject* parent)
    : QAbstractTableModel(parent)
    , col_names(col_names_)
    , batch_size(batch_size_)
    , vote_history() {
    vote_history.reserve(batch_size_);

    // set test datas!
    vote_history = {{1,
                     "张伟",
                     "2025-04-01 09:30:00",
                     "本周团建地点选择",
                     {"郊外烧烤", "游乐园", "密室逃脱"},
                     {"王静", "陈敏", "周婷"},
                     VoteChoiceType::kMultiChoice},
                    {2,
                     "李娜",
                     "2025-04-03 14:20:00",
                     "下季度学习主题投票",
                     {"C++高级编程", "Qt开发实战", "人工智能入门"},
                     {"王静", "黎晓曼", "陈敏"},
                     VoteChoiceType::kSingleChoice},
                    {3,
                     "王强",
                     "2025-04-05 10:15:00",
                     "办公室下午茶偏好",
                     {"奶茶", "咖啡", "果汁", "茶"},
                     {"王静", "叶凡", "陈敏"},
                     VoteChoiceType::kMultiChoice},
                    {4,
                     "陈静",
                     "2025-04-07 16:45:00",
                     "是否加班完成项目",
                     {"同意加班", "拒绝加班", "视情况而定"},
                     {"姚冉", "王静", "陈敏"},
                     VoteChoiceType::kSingleChoice},
                    {5,
                     "刘洋",
                     "2025-04-09 11:10:00",
                     "年会节目类型征集",
                     {"唱歌", "跳舞", "小品", "魔术"},
                     {"王强", "陈敏"},
                     VoteChoiceType::kMultiChoice},
                    {6,
                     "赵敏",
                     "2025-04-11 13:25:00",
                     "新办公椅样式选择",
                     {"人体工学椅", "简约风格椅", "电竞风格椅"},
                     {"孙玉", "刘静", "陈敏"},
                     VoteChoiceType::kSingleChoice},
                    {7,
                     "孙浩",
                     "2025-04-13 09:50:00",
                     "午餐订餐口味投票",
                     {"川菜", "粤菜", "东北菜", "轻食沙拉"},
                     {"王静", "周浩", "吴小曼", "陈敏"},
                     VoteChoiceType::kMultiChoice},
                    {8,
                     "周婷",
                     "2025-04-15 17:00:00",
                     "是否组织读书会",
                     {"支持", "反对", "可以尝试"},
                     {"王静", "陈三十", "陈敏"},
                     VoteChoiceType::kSingleChoice},
                    {9,
                     "吴磊",
                     "2025-04-17 10:40:00",
                     "团队口号征集",
                     {"团结一心", "勇攀高峰", "代码改变世界", "效率为王"},
                     {"王静", "李菲菲", "陈敏"},
                     VoteChoiceType::kMultiChoice},
                    {10,
                     "徐菲",
                     "2025-04-19 15:30:00",
                     "五一假期出行方式",
                     {"自驾游", "高铁出行", "飞机旅行", "本地休闲"},
                     {"王菲菲", "陈敏"},
                     VoteChoiceType::kMultiChoice}};
}

QVariant VoteHistoryViewModel::headerData(int section, Qt::Orientation orientation,
                                          int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return col_names[section];
        // } else if (role == Qt::DecorationRole) {
        //     static QIcon icon(":icons/images/yezi.svg");
        //     return icon;
        // }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}


int VoteHistoryViewModel::rowCount(const QModelIndex& parent) const {
    return vote_history.size();
}
int VoteHistoryViewModel::columnCount(const QModelIndex& parent) const {
    return col_names.size();
}


QVariant VoteHistoryViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return {};
    }
    int   row     = index.row();
    int   col     = index.column();
    auto& history = vote_history[vote_history.size() - 1 - row];
    if (role == Qt::DisplayRole) {
        if (col == 0) {
            return history.vote_id;
        } else if (col == 1) {
            return history.creator;
        } else if (col == 2) {
            return history.create_time;
        } else if (col == 3) {
            return history.vote_topic;
        } else if (col == 4) {
            return history.choice_type == VoteChoiceType::kSingleChoice ? QString("单选")
                                                                        : QString("多选");
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
            icon_file = ":icons/images/yellow_butterfly.svg";

        } else if (col == 4) {
            icon_file = ":icons/images/vote.svg";

        } else if (col == 5) {
            icon_file = ":icons/images/sea_leaf.svg";
        }
        if (!icon_file.isEmpty()) {
            return QIcon(icon_file);
        }
    }
    return {};
}

void VoteHistoryViewModel::append(VoteHistory&& item) {
    beginInsertRows(QModelIndex(), 0, 0);
    vote_history.push_back(std::move(item));
    endInsertRows();
}
void VoteHistoryViewModel::append(const VoteHistory& item) {
    beginInsertRows(QModelIndex(), 0, 0);
    vote_history.push_back(item);
    endInsertRows();
}

VoteHistory& VoteHistoryViewModel::at(size_t i) {
    return vote_history[i];
}



size_t VoteHistoryViewModel::get_batch_size() {
    return batch_size;
}
void VoteHistoryViewModel::set_batch_size(size_t batch_size_) {
    batch_size = batch_size_;
}
}   // namespace client

}   // namespace tang