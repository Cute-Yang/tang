#include "participate_vote_page.h"

namespace tang {
namespace client {
// use unique_ptr to instead!
ParticipateVotePage::ParticipateVotePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new ParticipateVotePageUi()) {
    ui->setup_ui(this);
    this->setTitleVisible(false);

    QStringList     headers        = {"vote id", "创建者", "创建时间", "类型", "状态", "check"};
    QList<VoteData> vote_data_list = {
        // 数据 1
        {"张三",
         "2025-09-25 14:30:22",
         {"周末去哪里玩？", "郊外踏青", "电影院", "在家休息", "逛街购物"},
         1000,
         common::VoteStatus::kReady,
         common::VoteChoiceType::kSingleChoice},
        // 数据 2
        {"李四",
         "2025-09-26 09:15:45",
         {"午餐吃什么？", "火锅", "烧烤", "快餐", "食堂", "外卖"},
         1001,
         common::VoteStatus::kFinished,
         common::VoteChoiceType::kMultiChoice},
        // 数据 3
        {"王五",
         "2025-09-24 18:20:10",
         {"选择最佳员工", "张三", "李四", "王五", "赵六"},
         1002,
         common::VoteStatus::kFinished,
         common::VoteChoiceType::kSingleChoice},
        // 数据 4
        {"赵六",
         "2025-09-27 11:05:33",
         {"团建活动方案", "密室逃脱", "KTV", "聚餐", "短途旅行"},
         1003,
         common::VoteStatus::kReady,
         common::VoteChoiceType::kMultiChoice},
        // 数据 5
        {"钱七",
         "2025-09-23 16:40:18",
         {"办公室装修风格", "现代简约", "工业风", "温馨田园", "北欧风格"},
         1004,
         common::VoteStatus::kInvalid,
         common::VoteChoiceType::kSingleChoice},
        // 数据 6
        {"孙八",
         "2025-09-28 13:25:55",
         {"年会节目类型", "唱歌", "跳舞", "小品", "魔术", "乐器演奏"},
         1005,
         common::VoteStatus::kReady,
         common::VoteChoiceType::kMultiChoice},
        // 数据 7
        {"周九",
         "2025-09-22 10:10:01",
         {"新项目名称", "星辰大海", "智启未来", "创新引擎", "极光计划"},
         1006,
         common::VoteStatus::kFinished,
         common::VoteChoiceType::kSingleChoice},
        // 数据 8
        {"吴十",
         "2025-09-29 15:50:44",
         {"技术选型", "React", "Vue", "Angular", "Svelte", "原生开发"},
         1007,
         common::VoteStatus::kReady,
         common::VoteChoiceType::kMultiChoice},
        // 数据 9
        {"郑一",
         "2025-09-21 12:35:27",
         {"假期安排", "调休", "补假", "发补贴", "灵活工作", "团队旅游"},
         1008,
         common::VoteStatus::kInvalid,
         common::VoteChoiceType::kSingleChoice},
        // 数据 10
        {"王二",
         "2025-09-30 08:00:15",
         {"福利发放方式", "购物卡", "现金", "实物礼品", "带薪假期", "股票期权"},
         1009,
         common::VoteStatus::kReady,
         common::VoteChoiceType::kMultiChoice}};

    view_model = new ParticipateViewModel(8, headers, this);
    view_model->set_vote_datas(vote_data_list);
    ui->vote_todo_list->setModel(view_model);
    connect(ui->confirm_vote_button, &ElaToolButton::clicked, this, [this]() {
        ui->vote_status_value->setText("已投票");
        ui->combox_stacked_container->setCurrentIndex(1);
    });
}
ParticipateVotePage::~ParticipateVotePage() {
    delete ui;
}
}   // namespace client
}   // namespace tang