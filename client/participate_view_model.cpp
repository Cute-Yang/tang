#include "participate_view_model.h"
#include <QIcon>

namespace tang {
namespace client {

constexpr int vote_id_col          = 0;
constexpr int creator_col          = 1;
constexpr int create_time_col      = 2;
constexpr int vote_topic_col       = 3;
constexpr int vote_choice_type_col = 4;
constexpr int vote_status_col      = 5;
constexpr int check_col            = 6;
constexpr int processed_col        = 7;

ParticipateViewModel::ParticipateViewModel(size_t batch_size_, const QStringList& headers_,
                                           QObject* parent)
    : QAbstractTableModel(parent)
    , batch_size(batch_size_)
    , vote_datas()
    , headers(headers_) {


    vote_datas = {// 数据 1
                  {"张三",
                   "2025-09-25 14:30:22",
                   "周末去哪里玩？",
                   {"郊外踏青", "电影院", "在家休息", "逛街购物"},
                   1000,
                   common::VoteStatus::kReady,
                   common::VoteChoiceType::kSingleChoice},
                  // 数据 2
                  {"李四",
                   "2025-09-26 09:15:45",
                   "午餐吃什么？",
                   {"火锅", "烧烤", "快餐", "食堂", "外卖"},
                   1001,
                   common::VoteStatus::kFinished,
                   common::VoteChoiceType::kMultiChoice},
                  // 数据 3
                  {"王五",
                   "2025-09-24 18:20:10",
                   "选择最佳员工",
                   {"张三", "李四", "王五", "赵六"},
                   1002,
                   common::VoteStatus::kFinished,
                   common::VoteChoiceType::kSingleChoice},
                  // 数据 4
                  {"赵六",
                   "2025-09-27 11:05:33",
                   "团建活动方案",
                   {"密室逃脱", "KTV", "聚餐", "短途旅行"},
                   1003,
                   common::VoteStatus::kReady,
                   common::VoteChoiceType::kMultiChoice},
                  // 数据 5
                  {"钱七",
                   "2025-09-23 16:40:18",
                   "办公室装修风格",
                   {"现代简约", "工业风", "温馨田园", "北欧风格"},
                   1004,
                   common::VoteStatus::kInvalid,
                   common::VoteChoiceType::kSingleChoice,
                   true},
                  // 数据 6
                  {"孙八",
                   "2025-09-28 13:25:55",
                   "年会节目类型",
                   {"唱歌", "跳舞", "小品", "魔术", "乐器演奏"},
                   1005,
                   common::VoteStatus::kReady,
                   common::VoteChoiceType::kMultiChoice},
                  // 数据 7
                  {"周九",
                   "2025-09-22 10:10:01",
                   "新项目名称",
                   {"星辰大海", "智启未来", "创新引擎", "极光计划"},
                   1006,
                   common::VoteStatus::kFinished,
                   common::VoteChoiceType::kSingleChoice},
                  // 数据 8
                  {"吴十",
                   "2025-09-29 15:50:44",
                   "技术选型",
                   {"React", "Vue", "Angular", "Svelte", "原生开发"},
                   1007,
                   common::VoteStatus::kReady,
                   common::VoteChoiceType::kMultiChoice},
                  // 数据 9
                  {"郑一",
                   "2025-09-21 12:35:27",
                   "假期安排",
                   {"调休", "补假", "发补贴", "灵活工作", "团队旅游"},
                   1008,
                   common::VoteStatus::kInvalid,
                   common::VoteChoiceType::kSingleChoice,
                   true},
                  // 数据 10
                  {"王二",
                   "2025-09-30 08:00:15",
                   "福利发放方式",
                   {"购物卡", "现金", "实物礼品", "带薪假期", "股票期权"},
                   1009,
                   common::VoteStatus::kReady,
                   common::VoteChoiceType::kMultiChoice}};
}


void ParticipateViewModel::set_vote_datas(const QList<VoteData>& vote_datas_) {
    vote_datas = vote_datas_;
}

VoteData& ParticipateViewModel::at(size_t i) {
    return vote_datas[i];
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

        } else if (col == vote_topic_col) {
            return vote_data.vote_topic;
        } else if (col == vote_choice_type_col) {
            return vote_data.vote_choice_type == common::VoteChoiceType::kSingleChoice
                       ? QString("单选")
                       : QString("多选");
        } else if (col == vote_status_col) {
            return get_vote_status_display_str(vote_data.vote_status);
        } else if (col == check_col) {
            return QString("查看");
        } else if (col == processed_col) {
            return vote_data.processed ? "已投票" : "未投票";
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

        } else if (col == vote_topic_col) {
            icon_file = ":icons/images/guihua.svg";
        } else if (col == vote_choice_type_col) {
            icon_file = ":icons/images/blue_vote.svg";

        } else if (col == creator_col) {
            icon_file = ":icons/images/mifeng.svg";
        } else if (col == processed_col) {
            icon_file =
                vote_data.processed ? ":icons/images/do_it.svg" : ":icons/images/not_do_it.svg";
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