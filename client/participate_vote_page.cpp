#include "participate_vote_page.h"
#include "ElaMessageBar.h"
#include "client_global_config.h"
#include "util.h"
namespace tang {
namespace client {
// use unique_ptr to instead!
ParticipateVotePage::ParticipateVotePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new ParticipateVotePageUi())
    , show_widget(find_root_widget(this)) {
    ui->setup_ui(this);
    this->setTitleVisible(false);

    QStringList headers = {
        "vote id", "创建者", "创建时间", "主题", "类型", "状态", "check", "进度"};


    view_model = new ParticipateViewModel(8, headers, this);
    ui->vote_todo_list->setModel(view_model);

    this->initialize_connects();
}
ParticipateVotePage::~ParticipateVotePage() {
    delete ui;
}

void ParticipateVotePage::show_message(const QString& message, bool error) {
    if (error) {
        ElaMessageBar::warning(ElaMessageBarType::TopLeft,
                               "Participate",
                               message,
                               ClientGlobalConfig::error_show_time,
                               show_widget);
    } else {
        ElaMessageBar::success(ElaMessageBarType::TopLeft,
                               "Participate",
                               message,
                               ClientGlobalConfig::message_show_time,
                               show_widget);
    }
}

void ParticipateVotePage::initialize_connects() {
    connect(ui->vote_todo_list,
            &ElaTableView::tableViewShow,
            this,
            &ParticipateVotePage::adjust_vote_todo_list_view);
    connect(ui->adjust_content_view_button,
            &ElaToolButton::clicked,
            this,
            &ParticipateVotePage::adjust_vote_todo_list_view);
    connect(
        ui->vote_todo_list, &ElaTableView::doubleClicked, this, [this](const QModelIndex& index) {
            if (!index.isValid()) {
                return;
            }
            int row = index.row();
            if (row >= view_model->size()) {
                this->show_message("越界啦(ノへ￣、)");
                vote_data_index = -1;
                return;
            }
            vote_data_index = row;
            auto& vote_data = view_model->at(row);
            this->display_vote_data(vote_data);
        });
    connect(ui->confirm_vote_button, &ElaToolButton::clicked, this, [this]() {
        if (vote_data_index >= view_model->size() || vote_data_index < 0) {
            this->show_message("越界啦(ノへ￣、)");
            return;
        }
        auto& vote_data = view_model->at(vote_data_index);
        if (vote_data.processed) {
            this->show_message("已经投过票啦(✿◠‿◠)", false);
            return;
        }
        view_model->at(vote_data_index).processed = true;
        this->show_message(QString("投票 '%1' 成功(✿◠‿◠)").arg(vote_data.vote_topic), false);
    });
}

void ParticipateVotePage::set_frozon(bool enable) {
    ui->confirm_vote_button->setEnabled(enable);
}

void ParticipateVotePage::display_vote_data(const VoteData& vote_data) {
    ui->vote_id_value->setText(std::to_string(vote_data.vote_id).c_str());
    ui->vote_creator_value->setText(vote_data.creator);
    ui->vote_create_time_value->setText(vote_data.create_time);
    bool is_single = vote_data.vote_choice_type == common::VoteChoiceType::kSingleChoice;
    ui->vote_type_value->setText(is_single ? "单选" : "多选");
    if (is_single) {
        ui->vote_item_combox->clear();
        ui->vote_item_combox->addItems(vote_data.vote_items);
        ui->combox_stacked_container->setCurrentWidget(ui->vote_item_combox);
    } else {
        ui->vote_item_multi_combox->clear();
        ui->vote_item_multi_combox->addItems(vote_data.vote_items);
        ui->vote_item_multi_combox->setCurrentSelection(0);
        ui->combox_stacked_container->setCurrentWidget(ui->vote_item_multi_combox);
    }
    ui->vote_status_value->setText(get_vote_status_display_str(vote_data.vote_status));

    this->set_frozon(vote_data.vote_status == common::VoteStatus::kReady && !vote_data.processed);
}

void ParticipateVotePage::adjust_vote_todo_list_view() {
    constexpr std::array<float, 8> percents = {0.08, 0.1, 0.2, 0.2, 0.1, 0.1, 0.1, 0.1};
    int                            width    = ui->vote_todo_list->width();
    for (size_t i = 0; i < percents.size(); ++i) {
        int w = static_cast<int>(width * percents[i]);
        ui->vote_todo_list->setColumnWidth(i, w);
    }
}
}   // namespace client
}   // namespace tang