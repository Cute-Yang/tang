#include "vote_history_view_model.h"

using namespace tang::common;
namespace tang {
namespace client {
VoteHistoryViewModel::VoteHistoryViewModel(const QStringList& col_names_, size_t batch_size_,
                                           QObject* parent)
    : QAbstractTableModel(parent)
    , col_names(col_names_)
    , vote_history() {
    vote_history.reserve(batch_size_);

    // set test datas!
    vote_history = {{1,
                     "张伟",
                     "2025-04-01 09:30:00",
                     "本周团建地点选择",
                     {"郊外烧烤", "游乐园", "密室逃脱"},
                     VoteChoiceType::kMultiChoice},
                    {2,
                     "李娜",
                     "2025-04-03 14:20:00",
                     "下季度学习主题投票",
                     {"C++高级编程", "Qt开发实战", "人工智能入门"},
                     VoteChoiceType::kSingleChoice},
                    {3,
                     "王强",
                     "2025-04-05 10:15:00",
                     "办公室下午茶偏好",
                     {"奶茶", "咖啡", "果汁", "茶"},
                     VoteChoiceType::kMultiChoice},
                    {4,
                     "陈静",
                     "2025-04-07 16:45:00",
                     "是否加班完成项目",
                     {"同意加班", "拒绝加班", "视情况而定"},
                     VoteChoiceType::kSingleChoice},
                    {5,
                     "刘洋",
                     "2025-04-09 11:10:00",
                     "年会节目类型征集",
                     {"唱歌", "跳舞", "小品", "魔术"},
                     VoteChoiceType::kMultiChoice},
                    {6,
                     "赵敏",
                     "2025-04-11 13:25:00",
                     "新办公椅样式选择",
                     {"人体工学椅", "简约风格椅", "电竞风格椅"},
                     VoteChoiceType::kSingleChoice},
                    {7,
                     "孙浩",
                     "2025-04-13 09:50:00",
                     "午餐订餐口味投票",
                     {"川菜", "粤菜", "东北菜", "轻食沙拉"},
                     VoteChoiceType::kMultiChoice},
                    {8,
                     "周婷",
                     "2025-04-15 17:00:00",
                     "是否组织读书会",
                     {"支持", "反对", "可以尝试"},
                     VoteChoiceType::kSingleChoice},
                    {9,
                     "吴磊",
                     "2025-04-17 10:40:00",
                     "团队口号征集",
                     {"团结一心", "勇攀高峰", "代码改变世界", "效率为王"},
                     VoteChoiceType::kMultiChoice},
                    {10,
                     "徐菲",
                     "2025-04-19 15:30:00",
                     "五一假期出行方式",
                     {"自驾游", "高铁出行", "飞机旅行", "本地休闲"},
                     VoteChoiceType::kMultiChoice}};
}

QVariant VoteHistoryViewModel::headerData(int section, Qt::Orientation orientation,
                                          int role) const {
    if (orientation == Qt::Horizontal and role == Qt::DisplayRole) {
        return col_names[section];
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
    auto& history = vote_history[row];
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
        }
    }
    return {};
}

}   // namespace client

}   // namespace tang