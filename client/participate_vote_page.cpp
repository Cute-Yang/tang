#include "participate_vote_page.h"
#include "ElaMessageBar.h"
#include "client_global_config.h"
#include "client_singleton.h"
#include "common/http_json_keys.h"
#include "util.h"
#include <QJsonArray>
#include <QJsonObject>


using namespace tang::common;

namespace tang {
namespace client {
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

void ParticipateVotePage::refresh_for_once() {
    this->refresh_participate_vote_history_impl();
    if (view_model->size() > 0) {
        this->display_vote_data(view_model->at(0));
    }
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
    connect(ui->confirm_vote_button,
            &ElaToolButton::clicked,
            this,
            &ParticipateVotePage::send_vote_choices_impl);

    connect(ui->refresh_todo_list_button,
            &ElaToolButton::clicked,
            this,
            &ParticipateVotePage::refresh_participate_vote_history_impl);

    connect(ui->current_todo_list_page, &ElaSpinBox::valueChanged, this, [this](int n) {
        auto batch_size = view_model->get_batch_size();
        this->get_chunk_participate_vote_data_impl(batch_size, batch_size * n);
    });
}

void ParticipateVotePage::send_vote_choices_impl() {
    if (vote_data_index >= view_model->size() || vote_data_index < 0) {
        this->show_message("越界啦(ノへ￣、)");
        return;
    }
    auto& vote_data = view_model->at(vote_data_index);
    if (vote_data.vote_process_status == VoteProcessStatus::kProcessed) {
        this->show_message("已经投过票啦(✿◠‿◠)", false);
        return;
    }

    auto&       current_user_info = ClientSingleton::get_cache_user_info_instance();
    QJsonObject json_data;
    json_data["voter_id"] = current_user_info.user_id;
    json_data["vote_id"]  = static_cast<int64_t>(vote_data.vote_id);

    QJsonArray choices;
    if (vote_data.vote_choice_type == VoteChoiceType::kSingleChoice) {
        choices.append(ui->vote_item_combox->currentIndex());
    } else {
        auto _choices = ui->vote_item_multi_combox->getCurrentSelectionIndex();
        for (auto c : _choices) {
            choices.append(c);
        }
    }
    if (choices.empty()) {
        this->show_message("请至少选择一个选项(✿◠‿◠)");
    }
    json_data["vote_choices"] = choices;
    auto reply                = send_http_req_with_json_data(
        json_data, ClientSingleton::get_http_urls_instance().get_send_vote_choices_url());


    auto send_callback =
        [this, reply, send_vote_index = vote_data_index, vote_topic = vote_data.vote_topic]() {
            auto json_doc = get_json_document(reply);
            if (!json_doc) {
                return;
            }
            auto& json_data = json_doc.value();
            VALIDATE_JSON_RESP_IS_OK(json_data);
            // just change it self
            // this maybe not safe
            // if send and refresh...
            // the data will change!
            view_model->at(send_vote_index).vote_process_status = VoteProcessStatus::kProcessed;
            this->show_message(QString("投票 '%1' 成功(✿◠‿◠)").arg(vote_topic), false);
            set_frozon(false);
        };
    connect(reply, &QNetworkReply::finished, this, send_callback);
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

    this->set_frozon(vote_data.vote_status == common::VoteStatus::kReady &&
                     vote_data.vote_process_status == common::VoteProcessStatus::kReady);
}

void ParticipateVotePage::adjust_vote_todo_list_view() {
    constexpr std::array<float, 8> percents = {0.08, 0.1, 0.2, 0.2, 0.1, 0.1, 0.1, 0.1};
    int                            width    = ui->vote_todo_list->width();
    for (size_t i = 0; i < percents.size(); ++i) {
        int w = static_cast<int>(width * percents[i]);
        ui->vote_todo_list->setColumnWidth(i, w);
    }
}


void ParticipateVotePage::refresh_participate_vote_history_impl() {
    auto&       current_user_info = ClientSingleton::get_cache_user_info_instance();
    QJsonObject json_data;
    json_data[GetParticipateVoteReqKeys::voter_id_key] = current_user_info.user_id;

    QJsonArray select_vote_status;
    auto       selected = ui->select_vote_status_combox->getCurrentSelectionIndex();
    if (selected.size() == 0) {
        this->show_message("请至少选择一个投票的状态哦(✿◠‿◠)");
        return;
    }
    for (size_t i = 0; i < selected.size(); ++i) {
        select_vote_status.append(selected[i]);
    }
    json_data[GetParticipateVoteReqKeys::vote_status_key] = select_vote_status;

    QJsonArray select_vote_process_status;
    selected = ui->select_vote_process_status_combox->getCurrentSelectionIndex();
    if (selected.size() == 0) {
        this->show_message("请至少选择一个处理的状态哦(✿◠‿◠)");
        return;
    }
    for (size_t i = 0; i < selected.size(); ++i) {
        select_vote_process_status.append(selected[i]);
    }
    json_data[GetParticipateVoteReqKeys::vote_process_status_key] = select_vote_process_status;
    QNetworkReply* reply                                          = send_http_req_with_json_data(
        json_data, ClientSingleton::get_http_urls_instance().get_participate_vote_num_url());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        auto document = get_json_document(reply);
        if (!document) {
            return;
        }
        auto json_data = document->object();
        VALIDATE_JSON_RESP_IS_OK(json_data);
        int vote_count = json_data["vote_count"].toInteger();
        if (vote_count < 0) {
            this->show_message("§(*￣▽￣*)§ 负数...");
            return;
        }
        size_t batch_size = view_model->get_batch_size();
        size_t n          = (vote_count + batch_size - 1) / batch_size;
        ui->totoal_todo_list_page->setText(
            QString(" / %1(pages) (total:%2)").arg(n).arg(vote_count));

        ui->current_todo_list_page->setRange(0, n - 1);
        this->get_chunk_participate_vote_data_impl(
            batch_size < vote_count ? batch_size : vote_count, 0);
    });
}

void ParticipateVotePage::get_chunk_participate_vote_data_impl(int vote_num, int vote_offset) {
    if (vote_num < 0 || vote_offset < 0) {
        this->show_message("invalid value...");
        return;
    }
    if (vote_num == 0) {
        view_model->resize(0);
        return;
    }
    auto&       current_user_info = ClientSingleton::get_cache_user_info_instance();
    QJsonObject json_data;
    json_data[GetParticipateVoteReqKeys::voter_id_key]    = current_user_info.user_id;
    json_data[GetParticipateVoteReqKeys::vote_num_key]    = vote_num;
    json_data[GetParticipateVoteReqKeys::vote_offset_key] = vote_offset;
    QJsonArray select_vote_status;
    auto       selected = ui->select_vote_status_combox->getCurrentSelectionIndex();
    if (selected.size() == 0) {
        this->show_message("请至少选择一个投票的状态哦(✿◠‿◠)");
        return;
    }
    for (size_t i = 0; i < selected.size(); ++i) {
        select_vote_status.append(selected[i]);
    }
    json_data[GetParticipateVoteReqKeys::vote_status_key] = select_vote_status;
    QJsonArray select_vote_process_status;
    selected = ui->select_vote_process_status_combox->getCurrentSelectionIndex();
    if (selected.size() == 0) {
        this->show_message("请至少选择一个处理的状态哦(✿◠‿◠)");
        return;
    }
    for (size_t i = 0; i < selected.size(); ++i) {
        select_vote_process_status.append(selected[i]);
    }
    json_data[GetParticipateVoteReqKeys::vote_process_status_key] = select_vote_process_status;
    QNetworkReply* reply                                          = send_http_req_with_json_data(
        json_data, ClientSingleton::get_http_urls_instance().get_participate_chunk_vote_data_url());
    this->show_message("查询中(✿◠‿◠)", false);

    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        auto document = get_json_document(reply);
        if (!document) {
            return;
        }
        auto json_data = document->object();
        VALIDATE_JSON_RESP_IS_OK(json_data);
        QJsonArray vote_datas = json_data["vote_datas"].toArray();
        size_t     n          = vote_datas.size();
        view_model->resize(n);
        for (size_t i = 0; i < n; ++i) {
            auto  json_vote_data  = vote_datas[i].toObject();
            auto& vote_data       = view_model->at(i);
            vote_data.vote_id     = json_vote_data["vote_id"].toInteger();
            vote_data.creator     = json_vote_data["vote_creator"].toString();
            vote_data.create_time = json_vote_data["vote_create_time"].toString();
            vote_data.vote_topic  = json_vote_data["vote_topic"].toString();
            vote_data.vote_status = static_cast<VoteStatus>(json_vote_data["vote_status"].toInt());
            vote_data.vote_choice_type =
                static_cast<VoteChoiceType>(json_vote_data["vote_type"].toInt());
            vote_data.vote_process_status =
                static_cast<VoteProcessStatus>(json_vote_data["vote_process_status"].toInt());
            auto  json_vote_items = json_vote_data["vote_items"].toArray();
            auto& vote_items      = vote_data.vote_items;
            vote_items.clear();
            vote_items.reserve(json_vote_items.size());
            for (size_t i = 0; i < json_vote_items.size(); ++i) {
                vote_items.push_back(json_vote_items[i].toString());
            }
        }
        view_model->layoutChanged();
    });
}
}   // namespace client
}   // namespace tang