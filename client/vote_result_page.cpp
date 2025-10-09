#include "vote_result_page.h"
#include "ElaMessageBar.h"
#include "ElaTheme.h"
#include "client_global_config.h"
#include "client_singleton.h"
#include "common/http_json_keys.h"
#include "util.h"
#include "vote_result_view_model.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>





using namespace tang::common;
namespace tang {
namespace client {
VoteResultPage::VoteResultPage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new VoteResultPageUi()) {

    ui->setup_ui(this);
    this->setTitleVisible(false);

    vote_count_model = new VoteItemCountViewModel();
    ui->vote_stat_view->setModel(vote_count_model);

    vote_result_history_model =
        new VoteResultHistoryViewModel({"vote id", "创建者", "创建时间", "主题", "类型", "查看"});
    ui->vote_history_view->setModel(vote_result_history_model);
    this->init_connects();
    this->refresh_finished_vote_impl();
}

VoteResultPage::~VoteResultPage() {
    delete ui;
}


void VoteResultPage::plot_impl(std::span<VoteItemCountInfo> vote_item_infos) {
    // colors for plot
    static QStringList const colors = {
        "#6480D6",
        "#A1DC85",
        "#FFAD25",
        "#FF7777",
        "#84D1EF",
        "#4CB383",
    };

    auto n = vote_item_infos.size();
    // add series
    ui->vote_img_series->setLabelsVisible(true);
    ui->vote_img_series->setLabelsPosition(QPieSlice::LabelInsideHorizontal);
    ui->vote_img_series->clear();

    uint32_t total_count = 0;
    for (size_t i = 0; i < n; ++i) {
        total_count += vote_item_infos[i].vote_item_count;
    }

    for (size_t i = 0; i < n; ++i) {
        if (vote_item_infos[i].vote_item_count == 0) {
            continue;
        }
        // if we clear the slices,when the slice will be deallocated?
        QPieSlice* slice = new QPieSlice(this);
        slice->setLabelVisible(true);
        auto c = vote_item_infos[i].vote_item_count;
        slice->setValue(c);
        slice->setLabel(QString("%1 (%2%)")
                            .arg(vote_item_infos[i].vote_item)
                            .arg(c * 100.0 / total_count, 0, 'f', 2));
        size_t color_index = i % colors.size();
        slice->setColor(colors[color_index]);
        QFont font;
        font.setFamily("SimSun");
        font.setBold(true);
        font.setPointSizeF(7);

        slice->setLabelFont(font);
        // avoid out of index!
        slice->setLabelColor(colors[color_index]);

        slice->setBorderColor(colors[color_index]);
        ui->vote_img_series->append(slice);
    }
}


void VoteResultPage::show_message(const QString& message, bool error) {
    if (error) {
        ElaMessageBar::warning(ElaMessageBarType::TopLeft,
                               "vote_result",
                               message,
                               ClientGlobalConfig::error_show_time,
                               this);
    } else {
        ElaMessageBar::success(ElaMessageBarType::TopLeft,
                               "vote_result",
                               message,
                               ClientGlobalConfig::message_show_time,
                               this);
    }
}

void VoteResultPage::init_connects() {
    connect(ui->vote_history_view,
            &ElaTableView::tableViewShow,
            this,
            &VoteResultPage::adjust_vote_result_histroy_view);
    connect(ui->refresh_page_button,
            &ElaToolButton::clicked,
            this,
            &VoteResultPage::refresh_finished_vote_impl);
    connect(ui->vote_history_view,
            &ElaTableView::doubleClicked,
            this,
            [this](const QModelIndex& index) {
                if (!index.isValid()) {
                    return;
                }
                this->display_result_history_impl(index.row());
            });
    connect(ui->adjust_history_view_button,
            &ElaToolButton::clicked,
            this,
            &VoteResultPage::adjust_vote_result_histroy_view);

    connect(eTheme, &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode theme_mode) {
        ui->vote_img_chart->setTheme(theme_mode == ElaThemeType::Light ? QChart::ChartThemeLight
                                                                       : QChart::ChartThemeDark);
    });
}

void VoteResultPage::adjust_vote_result_histroy_view() {
    constexpr std::array<float, 6> width_percents = {0.1f, 0.1f, 0.3f, 0.3f, 0.1f, 0.1f};
    auto                           width          = ui->vote_history_view->width();
    for (size_t i = 0; i < width_percents.size(); ++i) {
        auto w = static_cast<int>(width * width_percents[i]);
        ui->vote_history_view->setColumnWidth(i, w);
    }
}


void VoteResultPage::refresh_finished_vote_impl() {
    auto url(ClientSingleton::get_http_urls_instance().get_finished_vote_num_url());
    auto reply =
        ClientSingleton::get_network_manager_instance().post(QNetworkRequest(url), nullptr);

    auto callback = [this, reply]() {
        auto json_doc = get_json_document(reply);
        if (!json_doc) {
            return;
        }

        auto& json_data = json_doc.value();
        VALIDATE_JSON_RESP_IS_OK(json_data);

        auto count      = json_data["count"].toInt();
        auto batch_size = vote_result_history_model->get_batch_size();
        auto n          = (count + batch_size - 1) / batch_size;
        ui->current_history_page->setRange(0, n - 1);
        ui->total_history_page->setText(QString("%1(page) (total:%2)").arg(n).arg(count));
        this->get_chunk_finished_vote_data_impl(count > batch_size ? batch_size : count, 0);
    };
    connect(reply, &QNetworkReply::finished, this, callback);
}
void VoteResultPage::get_chunk_finished_vote_data_impl(int vote_num, int vote_offset) {
    if (vote_num < 0 || vote_offset < 0) {
        this->show_message("Invalid params...");
        return;
    }
    if (vote_num == 0) {
        vote_result_history_model->resize(0);
        vote_result_history_model->layoutChanged();
        return;
    }
    auto& url = ClientSingleton::get_http_urls_instance().get_chunk_finished_vote_data_url();

    QJsonObject json_data;
    json_data[ChunkVoteReqBaseKeys::vote_num_key]    = vote_num;
    json_data[ChunkVoteReqBaseKeys::vote_offset_key] = vote_offset;

    auto reply    = send_http_req_with_json_data(json_data, url);
    auto callback = [this, reply]() {
        auto json_doc = get_json_document(reply);
        if (!json_doc) {
            return;
        }

        auto& json_data = json_doc.value();
        VALIDATE_JSON_RESP_IS_OK(json_data);
        QJsonArray json_vote_datas = json_data["vote_datas"].toArray();
        auto       n               = json_vote_datas.size();
        vote_result_history_model->resize(n);
        for (size_t i = 0; i < n; ++i) {
            auto  json_vote_data       = json_vote_datas[i].toObject();
            auto& result_history       = vote_result_history_model->at(i);
            result_history.vote_id     = json_vote_data["vote_id"].toInt();
            result_history.vote_topic  = json_vote_data["vote_topic"].toString();
            result_history.creator     = json_vote_data["vote_creator"].toString();
            result_history.create_time = json_vote_data["vote_create_time"].toString();
            auto json_vote_items       = json_vote_data["vote_items"].toArray();

            auto  n_items         = json_vote_items.size();
            auto& vote_item_infos = result_history.vote_item_infos;
            vote_item_infos.resize(n_items);
            for (size_t j = 0; j < n_items; ++j) {
                auto json_vote_item                = json_vote_items[j].toObject();
                vote_item_infos[j].vote_item       = json_vote_item["vote_item"].toString();
                vote_item_infos[j].vote_item_count = json_vote_item["vote_item_count"].toInt();
                vote_item_infos[j].vote_item_status =
                    static_cast<VoteItemStatus>(json_vote_item["vote_item_status"].toInt());
            }
        }
        vote_result_history_model->layoutChanged();
    };

    connect(reply, &QNetworkReply::finished, this, callback);
}


void VoteResultPage::display_result_history_impl(size_t i) {
    auto& vote_result_history = vote_result_history_model->at(i);
    ui->vote_creator_name_text->setText(vote_result_history.creator);
    ui->vote_create_time_value->setText(vote_result_history.create_time);
    ui->vote_topic_value->setText(vote_result_history.vote_topic);
    ui->vote_choice_type_value->setText(
        vote_result_history.choice_type == VoteChoiceType::kSingleChoice ? QString("单选")
                                                                         : QString("多选"));

    QStringList hit_vote_items;
    auto&       vote_item_infos = vote_result_history.vote_item_infos;


    for (size_t i = 0; i < vote_item_infos.size(); ++i) {
        if (vote_item_infos[i].vote_item_status == VoteItemStatus::kSelected) {
            hit_vote_items.append(vote_item_infos[i].vote_item);
        }
    }
    ui->vote_result_value->setText(hit_vote_items.join(","));
    std::span vote_item_infos_view(vote_item_infos.data(), vote_item_infos.size());
    vote_count_model->set_vote_infos(vote_item_infos_view);

    // then plot!
    this->plot_impl(vote_item_infos_view);
}

}   // namespace client
}   // namespace tang