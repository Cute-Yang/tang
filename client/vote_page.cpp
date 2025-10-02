#include "vote_page.h"
#include "ElaMenu.h"
#include "ElaMessageBar.h"
#include "client_global_config.h"
#include "client_singleton.h"
#include "common/response_keys.h"
#include "util.h"
#include "vote_item_view_model.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>
#include <chrono>



using namespace tang::common;


namespace tang {
namespace client {
class VoteItemRightMenu {
public:
    ElaMenu* menu;
    QAction* delete_item_action;
    QAction* modify_item_action;
    QAction* move_up_action;
    QAction* move_down_action;

    VoteItemRightMenu(QWidget* parent = nullptr) {
        menu               = new ElaMenu(parent);
        delete_item_action = menu->addAction("删除");
        delete_item_action->setIcon(QIcon(":icons/images/delete.svg"));

        modify_item_action = menu->addAction("修改");
        modify_item_action->setIcon(QIcon(":icons/images/modify.svg"));

        move_up_action = menu->addAction("上移");
        move_up_action->setIcon(QIcon(":icons/images/move_up.svg"));

        move_down_action = menu->addAction("下移");
        move_down_action->setIcon(QIcon(":icons/images/move_down.svg"));
    }
};


VotePage::VotePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new VotePageUi)
    , vote_data_model(new VoteItemViewModel("投票项", this))
    , vote_item_right_menu(new VoteItemRightMenu(this))
    , show_widget(find_root_widget(this)) {
    ui->setup_ui(this);
    this->setTitleVisible(false);
    ui->vote_item_view->setModel(vote_data_model);

    QStringList vote_history_cols = {"id", "创建者", "创建时间", "主题", "状态", "类型", "check"};
    vote_history_model            = new VoteHistoryViewModel(vote_history_cols, 10, this);
    ui->vote_history_view->setModel(vote_history_model);

    // only for test!
    QStringList items = {"钓鱼", "打球", "爬山", "LOL", "游泳", "骑自行车"};
    vote_data_model->add_vote_items(items);

    this->initialize_connects();
}

VotePage::~VotePage() {
    delete ui;
    delete vote_data_model;
    delete vote_item_right_menu;
}


void VotePage::show_message(const QString& message, bool error) {
    if (error) {
        ElaMessageBar::warning(ElaMessageBarType::TopLeft,
                               "vote",
                               message,
                               ClientGlobalConfig::error_show_time,
                               show_widget);
    } else {
        ElaMessageBar::success(ElaMessageBarType::TopLeft,
                               "vote",
                               message,
                               ClientGlobalConfig::message_show_time,
                               show_widget);
    }
}

void VotePage::display_vote_item_right_menu(const QPoint& pos) {
    auto     index      = ui->vote_item_view->indexAt(pos);
    int      row        = index.row();
    auto     global_pos = ui->vote_item_view->mapToGlobal(pos);
    QAction* selected   = vote_item_right_menu->menu->exec(global_pos);
    if (selected == vote_item_right_menu->move_up_action) {
        emit start_swap_vote_item(row, row - 1);
    } else if (selected == vote_item_right_menu->move_down_action) {
        emit start_swap_vote_item(row, row + 1);
    } else if (selected == vote_item_right_menu->delete_item_action) {
        emit start_delete_vote_item(row);
    }
}

void VotePage::adjust_vote_history_view() {
    auto                           width = ui->vote_history_view->width();
    constexpr std::array<float, 7> rr    = {0.05, 0.15, 0.25, 0.25, 0.1, 0.1, 0.1};
    for (size_t i = 0; i < rr.size(); ++i) {
        int w = static_cast<int>(width * rr[i]);
        ui->vote_history_view->setColumnWidth(i, w);
    }
}   // namespace client

void VotePage::clear_vote_data() {
    // key,value is a better choice!
    ui->vote_creator_value->clear();
    ui->vote_creator_value->setText("西雅图");
    ui->vote_topic_line_edit->clear();
    ui->voters_combox->clearEditText();
    vote_data_model->clear();
    this->set_frozon(true);
    // flush
    this->get_online_voters_impl();
}

void VotePage::initialize_connects() {
    connect(ui->vote_history_view,
            &ElaTableView::tableViewShow,
            this,
            &VotePage::adjust_vote_history_view);
    connect(ui->adjust_history_view_button,
            &ElaToolButton::clicked,
            this,
            &VotePage::adjust_vote_history_view);

    connect(ui->new_vote_button, &ElaToolButton::clicked, this, &VotePage::clear_vote_data);

    connect(ui->vote_item_view,
            &ElaTableView::customContextMenuRequested,
            this,
            &VotePage::display_vote_item_right_menu);

    connect(ui->add_vote_item_button, &ElaToolButton::clicked, this, [this]() {
        QString data = ui->add_vote_item_line_edit->text();
        if (data.isEmpty()) {
            this->show_message("不能添加空选项 (┬┬﹏┬┬)");
            return;
        } else if (this->vote_data_model->contains(data)) {
            this->show_message(QString("%1已存在,不能添加重复值＞﹏＜(●'◡'●)").arg(data));
            return;
        }
        this->show_message(QString("成功添加了投票项 %1 (●'◡'●)").arg(data), false);
        this->vote_data_model->add_vote_item(std::move(data));
        this->ui->add_vote_item_line_edit->clear();
    });

    // connect multi time...
    connect(this, &VotePage::start_swap_vote_item, this, [this](int i, int j) {
        // this->show_message(QString("swap %1 and %2").arg(i).arg(j));
        this->vote_data_model->swap_item(i, j);
    });

    connect(this, &VotePage::start_delete_vote_item, this, [this](int i) {
        this->vote_data_model->delete_item(i);
    });

    connect(
        ui->flush_voters_button, &ElaToolButton::clicked, this, &VotePage::get_online_voters_impl);

    connect(ui->start_vote_button, &ElaToolButton::clicked, this, &VotePage::create_vote_impl);

    connect(ui->vote_history_view,
            &ElaTableView::doubleClicked,
            this,
            [this](const QModelIndex& index) {
                if (!index.isValid()) {
                    return;
                }
                int col = index.column();
                // the check col...
                if (col == 6) {
                    // display!
                    this->display_vote_history_impl(vote_history_model->at(index.row()));
                }
            });
    connect(ui->refresh_vote_history_button,
            &ElaToolButton::clicked,
            this,
            &VotePage::refresh_vote_history_impl);

    connect(ui->current_vote_history_page, &ElaSpinBox::valueChanged, this, [this](int i) {
        auto batch_size = vote_history_model->get_batch_size();
        get_chunk_vote_data_impl(batch_size, batch_size * i);
    });
}

void VotePage::get_online_voters_impl() {
    QUrl            url(ClientSingleton::get_http_urls_instance().get_online_voters_url());
    QNetworkRequest request(url);
    auto            reply = ClientSingleton::get_network_manager_instance().get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        auto document = get_json_document(reply);
        if (!document) {
            this->show_message("网络错误...");
            return;
        }
        // show add a validator here!
        auto& json_data = document.value();
        int   status    = json_data[PublicResponseJsonKeys::status_key].toInt();
        if (status != static_cast<int>(StatusCode::kSuccess)) {
            this->show_message(json_data[PublicResponseJsonKeys::message_key].toString());
            return;
        }

        auto voter_infos = json_data["voter_infos"].toArray();
        this->online_voters.resize(voter_infos.size());
        QStringList voter_names(voter_infos.size());
        for (size_t i = 0; i < voter_infos.size(); ++i) {
            auto voter_info             = voter_infos[i].toObject();
            online_voters[i].voter_name = voter_info["voter_name"].toString();
            online_voters[i].voter_id   = voter_info["voter_id"].toInt();
            voter_names[i]              = online_voters[i].voter_name;
        }
        this->ui->voters_combox->clear();
        this->ui->voters_combox->addItems(voter_names);
        this->show_message("refresh成功(●'◡'●)", false);
    });
}

bool VotePage::prepare_vote_json_data(QJsonObject& json_data, VoteHistory& vote_history) {
    // prepare vote data!
    auto vote_topic = ui->vote_topic_line_edit->text();
    if (vote_topic.isEmpty()) {
        this->show_message("投票主题不能为空＞﹏＜");
        return false;
    }
    auto& vote_items = vote_data_model->get_vote_items();
    if (vote_items.empty()) {
        this->show_message("还没有添加投票项哦§(*￣▽￣*)§");
        return false;
    }
    if (vote_items.size() < 2) {
        this->show_message("投票项只有一个 ╯︿╰...");
        return false;
    }
    auto select_voter_indexes = ui->voters_combox->getCurrentSelectionIndex();
    if (select_voter_indexes.size() == 0) {
        this->show_message("请选择投票人§(*￣▽￣*)§");
        return false;
    }
    // so long,QAQ
    auto now = std::chrono::system_clock::now();
    auto us_timestamp =
        std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    auto& current_user_info = ClientSingleton::get_cache_user_info_instance();
    json_data["vote_topic"] = vote_topic;
    QJsonArray json_vote_items;
    for (size_t i = 0; i < vote_items.size(); ++i) {
        json_vote_items.append(vote_items[i]);
    }
    json_data["vote_items"]       = json_vote_items;
    json_data["vote_creator_id"]  = current_user_info.user_id;
    json_data["vote_creator"]     = current_user_info.user_name;
    json_data["vote_create_time"] = us_timestamp;

    QJsonArray json_voter_ids;
    for (size_t i = 0; i < select_voter_indexes.size(); ++i) {
        if (i >= online_voters.size()) {
            this->show_message("越界啦§(*￣▽￣*)§");
            return false;
        }
        // not supported unsigned!
        json_voter_ids.append(static_cast<int>(online_voters[i].voter_id));
    }
    json_data["voters"]           = json_voter_ids;
    json_data["vote_choice_type"] = static_cast<int>(
        ui->vote_choice_type_combox->currentIndex() == 0 ? VoteChoiceType::kSingleChoice
                                                         : VoteChoiceType::kMultiChoice);
    vote_history.creator     = current_user_info.user_name;
    vote_history.create_time = QString(format_time(now).c_str());
    vote_history.vote_topic  = vote_topic;
    vote_history.vote_items  = vote_items;
    this->show_message(
        QString("%1 成功发起了投票 %2 (✿◠‿◠)").arg(current_user_info.user_name, vote_topic), false);
    return true;
}

void VotePage::create_vote_impl() {
    QJsonObject json_data;
    VoteHistory vote_history;

    if (!this->prepare_vote_json_data(json_data, vote_history)) {
        return;
    }
    auto reply = send_http_req_with_json_data(
        json_data, ClientSingleton::get_http_urls_instance().get_create_vote_http_url());
    auto reply_callback = [this, reply, _vote_history = std::move(vote_history)]() mutable {
        auto document = get_json_document(reply);
        if (!document) {
            this->show_message("网络错误!＞︿＜");
            return;
        }

        auto     json_data = document.value();
        uint32_t status    = json_data[PublicResponseJsonKeys::status_key].toInteger();
        if (status != static_cast<uint32_t>(StatusCode::kSuccess)) {
            this->show_message(json_data[PublicResponseJsonKeys::message_key].toString());
            return;
        }
        uint32_t vote_id      = json_data["vote_id"].toInteger();
        _vote_history.vote_id = vote_id;
        // then push back to the table!
        vote_history_model->append(std::move(_vote_history));
    };
    connect(reply, &QNetworkReply::finished, this, std::move(reply_callback));
}


void VotePage::display_vote_history_impl(const VoteHistory& vote_history) {
    this->show_message(QString("查看vote %1 (✿◠‿◠)").arg(vote_history.vote_topic), false);
    ui->vote_creator_value->setText(vote_history.creator);
    ui->vote_topic_line_edit->setText(vote_history.vote_topic);
    ui->vote_choice_type_combox->setCurrentIndex(
        vote_history.choice_type == VoteChoiceType::kSingleChoice ? 0 : 1);
    ui->voters_combox->clear();
    ui->voters_combox->addItems(vote_history.voters);
    QList<int> ss(vote_history.voters.size());
    for (size_t i = 0; i < vote_history.voters.size(); ++i) {
        ss[i] = i;
    }
    ui->voters_combox->setCurrentSelection(ss);
    vote_data_model->set_vote_items(vote_history.vote_items);

    this->set_frozon(false);
}

void VotePage::set_frozon(bool enable) {
    // ui->vote_creator_value->setEnabled(true);
    ui->vote_topic_line_edit->setEnabled(enable);
    ui->voters_combox->setEnabled(enable);
    ui->vote_choice_type_combox->setEnabled(enable);
    ui->add_vote_item_button->setEnabled(enable);
    ui->start_vote_button->setEnabled(enable);
    ui->add_vote_item_line_edit->setEnabled(enable);
    ui->flush_voters_button->setEnabled(enable);
}


void VotePage::refresh_vote_history_impl() {
    auto&     current_user_info = ClientSingleton::get_cache_user_info_instance();
    QUrlQuery query;
    query.addQueryItem("voter_id", QString::number(current_user_info.user_id));
    QNetworkReply* reply = send_http_req_with_form_data(
        query, ClientSingleton::get_http_urls_instance().get_vote_num_url());
    // this->show_message("查询中(✿◠‿◠)", false);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        auto document = get_json_document(reply);
        if (!document) {
            return;
        }
        auto json_data = document->object();
        qDebug() << json_data;
        VALIDATE_JSON_RESP_IS_OK(json_data);
        int vote_count = json_data["vote_count"].toInteger();
        if (vote_count < 0) {
            this->show_message("§(*￣▽￣*)§ 负数...");
            return;
        }
        size_t batch_size = vote_history_model->get_batch_size();
        qDebug() << "the batch size is" << batch_size;
        size_t n = (vote_count + batch_size - 1) / batch_size;
        ui->total_vote_history_pages->setText(
            QString(" / %1(pages) (total:%2)").arg(n).arg(vote_count));

        ui->current_vote_history_page->setRange(0, n - 1);
        this->get_chunk_vote_data_impl(batch_size < vote_count ? batch_size : vote_count, 0);
    });
}

void VotePage::get_chunk_vote_data_impl(size_t vote_num, size_t vote_offset) {
    auto&     current_user_info = ClientSingleton::get_cache_user_info_instance();
    QUrlQuery query;
    query.addQueryItem("voter_id", QString::number(current_user_info.user_id));
    query.addQueryItem("vote_num", QString::number(vote_num));
    query.addQueryItem("vote_offset", QString::number(vote_offset));
    QNetworkReply* reply = send_http_req_with_form_data(
        query, ClientSingleton::get_http_urls_instance().get_chunk_vote_data_url());
    this->show_message("查询中(✿◠‿◠)", false);

    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        auto document = get_json_document(reply);
        if (!document) {
            return;
        }
        auto json_data = document->object();
        VALIDATE_JSON_RESP_IS_OK(json_data);

        QJsonArray vote_datas = json_data["vote_datas"].toArray();
        qDebug() << vote_datas;
        size_t n = vote_datas.size();
        vote_history_model->resize(n);
        for (size_t i = 0; i < n; ++i) {
            auto  vote_data           = vote_datas[i].toObject();
            auto& vote_hisotory       = vote_history_model->at(i);
            vote_hisotory.vote_id     = vote_data["vote_id"].toInteger();
            vote_hisotory.creator     = vote_data["vote_creator"].toString();
            vote_hisotory.create_time = vote_data["vote_create_time"].toString();
            vote_hisotory.vote_topic  = vote_data["vote_topic"].toString();
            vote_hisotory.choice_type = static_cast<VoteChoiceType>(vote_data["vote_type"].toInt());
            auto  json_vote_items     = vote_data["vote_items"].toArray();
            auto& vote_items          = vote_hisotory.vote_items;
            vote_items.clear();
            for (size_t i = 0; i < json_vote_items.size(); ++i) {
                vote_items.push_back(json_vote_items[i].toString());
            }
        }
        vote_history_model->layoutChanged();
    });
}
}   // namespace client
}   // namespace tang