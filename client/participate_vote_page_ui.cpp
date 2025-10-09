#include "participate_vote_page_ui.h"
#include "common/status.h"
#include "util.h"
#include <QFont>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QWidget>


using namespace tang::common;

namespace tang {
namespace client {
void ParticipateVotePageUi::setup_ui(ElaScrollPage* page) {
    // to support color!
    QWidget*     _central_widget = new QWidget(page);
    QVBoxLayout* _central_layout = new QVBoxLayout(_central_widget);

    QWidget* central_widget = new QWidget(page);
    _central_layout->addWidget(central_widget);

    QVBoxLayout* central_layout = new QVBoxLayout(central_widget);

    QFont font;
    font.setPointSize(10);
    font.setFamily("SimSun");
    font.setBold(true);

    constexpr int vote_key_width               = 120;
    constexpr int combox_width_delta           = 5;
    constexpr int vote_combox_key_width        = vote_key_width - combox_width_delta;
    constexpr int vote_value_width             = 200;
    constexpr int vote_horizontal_fixed_height = 30;

    static_assert(vote_key_width > 0 && vote_combox_key_width > 0 && vote_value_width > 0 &&
                      vote_horizontal_fixed_height > 0,
                  "negative value is not allowd!");

    QWidget* vote_id_container = new QWidget(page);
    vote_id_container->setFixedHeight(vote_horizontal_fixed_height);
    central_layout->addWidget(vote_id_container);
    QHBoxLayout* vote_id_layout = new QHBoxLayout(vote_id_container);
    vote_id_key                 = new ElaText(page);
    vote_id_key->setText("vote id:");
    vote_id_key->setFont(font);
    vote_id_key->setMinimumWidth(vote_key_width);
    vote_id_layout->addWidget(vote_id_key);

    vote_id_value = new ElaText(page);
    vote_id_value->setText("27");
    vote_id_value->setMinimumWidth(vote_value_width);
    vote_id_value->setFont(font);
    vote_id_layout->addWidget(vote_id_value);
    // make them at left!
    vote_id_layout->addStretch();


    QWidget* vote_creator_container = new QWidget(page);
    vote_creator_container->setFixedHeight(vote_horizontal_fixed_height);
    central_layout->addWidget(vote_creator_container);
    QHBoxLayout* vote_creator_layout = new QHBoxLayout(vote_creator_container);

    vote_creator_key = new ElaText(page);
    vote_creator_key->setFont(font);
    vote_creator_key->setMinimumWidth(vote_key_width);
    vote_creator_key->setText("发起人:");
    vote_creator_layout->addWidget(vote_creator_key);

    vote_creator_value = new ElaText(page);
    vote_creator_value->setFont(font);
    vote_creator_value->setMidLineWidth(vote_value_width);
    vote_creator_layout->addWidget(vote_creator_value);
    vote_creator_layout->addStretch();

    QWidget* vote_create_time_container = new QWidget(page);
    vote_create_time_container->setFixedHeight(vote_horizontal_fixed_height);
    central_layout->addWidget(vote_create_time_container);
    QHBoxLayout* vote_create_time_layout = new QHBoxLayout(vote_create_time_container);


    vote_create_time_key = new ElaText(page);
    vote_create_time_key->setFont(font);
    vote_create_time_key->setMinimumWidth(vote_key_width);
    vote_create_time_key->setText("创建时间:");
    vote_create_time_layout->addWidget(vote_create_time_key);

    vote_create_time_value = new ElaText(page);
    vote_create_time_value->setFont(font);
    vote_create_time_value->setMinimumWidth(vote_value_width);
    // vote_create_time_value->setText("2021-05-27 13:21:48");
    vote_create_time_layout->addWidget(vote_create_time_value);
    vote_create_time_layout->addStretch();

    QWidget* vote_type_container = new QWidget(page);
    vote_type_container->setFixedHeight(vote_horizontal_fixed_height);
    central_layout->addWidget(vote_type_container);
    QHBoxLayout* vote_type_layout = new QHBoxLayout(vote_type_container);
    vote_type_key                 = new ElaText(page);
    vote_type_key->setFont(font);
    vote_type_key->setMinimumWidth(vote_key_width);
    vote_type_key->setText("类型:");
    vote_type_layout->addWidget(vote_type_key);

    vote_type_value = new ElaText(page);
    vote_type_value->setFont(font);
    vote_type_value->setMinimumWidth(vote_key_width);
    // vote_type_value->setText("单选");
    vote_type_layout->addWidget(vote_type_value);
    vote_type_layout->addStretch();


    QWidget* vote_status_container = new QWidget(page);
    vote_status_container->setFixedHeight(vote_horizontal_fixed_height);
    central_layout->addWidget(vote_status_container);

    QHBoxLayout* vote_status_layout = new QHBoxLayout(vote_status_container);

    vote_status_key = new ElaText(page);
    vote_status_key->setText("状态:");
    vote_status_key->setFont(font);
    vote_status_key->setMinimumWidth(vote_key_width);
    vote_status_layout->addWidget(vote_status_key);

    vote_status_value = new ElaText(page);
    vote_status_value->setFont(font);
    vote_status_value->setMinimumWidth(vote_value_width);
    // 未投票/已投票/废弃
    vote_status_value->setText("未投票");
    vote_status_layout->addWidget(vote_status_value);
    vote_status_layout->addStretch();

    QWidget* vote_item_container = new QWidget(page);
    vote_item_container->setFixedHeight(48);
    central_layout->addWidget(vote_item_container);
    QHBoxLayout* vote_item_layout = new QHBoxLayout(vote_item_container);

    vote_item_key = new ElaText(page);
    vote_item_key->setText("投票选项:");
    vote_item_key->setFont(font);
    vote_item_key->setMinimumWidth(vote_combox_key_width);
    vote_item_layout->addWidget(vote_item_key);

    combox_stacked_container = new QStackedWidget(page);
    combox_stacked_container->setMaximumWidth(300);
    vote_item_layout->addWidget(combox_stacked_container);

    vote_item_combox = new ElaComboBox(page);
    vote_item_combox->setFont(font);
    vote_item_combox->setMinimumWidth(vote_value_width);
    combox_stacked_container->addWidget(vote_item_combox);

    vote_item_multi_combox = new ElaMultiSelectComboBox(page);
    vote_item_multi_combox->setFont(font);
    vote_item_multi_combox->setMinimumWidth(vote_value_width);
    vote_item_multi_combox->setMinimumWidth(400);
    combox_stacked_container->addWidget(vote_item_multi_combox);
    combox_stacked_container->setCurrentIndex(0);

    vote_item_layout->addSpacing(8);

    confirm_vote_button = new ElaToolButton(page);
    confirm_vote_button->setText("确认投票");
    confirm_vote_button->setFont(font);
    // if the status != 未投票,set it to false!
    confirm_vote_button->setEnabled(true);
    confirm_vote_button->setIcon(QIcon(":icons/images/leaf.svg"));
    confirm_vote_button->setIconSize(QSize(21, 21));
    confirm_vote_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    vote_item_layout->addWidget(confirm_vote_button);
    vote_item_layout->addStretch();

    adjust_content_view_button = new ElaToolButton(page);
    adjust_content_view_button->setText("调整视图");
    adjust_content_view_button->setFont(font);
    adjust_content_view_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    adjust_content_view_button->setIcon(QIcon(":icons/images/yezi.svg"));
    vote_item_layout->addWidget(adjust_content_view_button);

    central_layout->addSpacing(6);
    QWidget*     vote_todo_list_container = new QWidget(page);
    QVBoxLayout* vote_todo_list_layout    = new QVBoxLayout(vote_todo_list_container);
    central_layout->addWidget(vote_todo_list_container);

    vote_todo_list = new ElaTableView(page);
    vote_todo_list->horizontalHeader()->setFont(font);
    vote_todo_list->verticalHeader()->hide();
    vote_todo_list->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    vote_todo_list->setFont(font);
    vote_todo_list_layout->addWidget(vote_todo_list);

    // vote_todo_list_layout->addStretch();

    QWidget* page_info_container = new QWidget(page);
    vote_todo_list_layout->addWidget(page_info_container);
    QHBoxLayout* page_info_layout = new QHBoxLayout(page_info_container);
    refresh_todo_list_button      = new ElaToolButton(page);
    refresh_todo_list_button->setText("刷新");
    refresh_todo_list_button->setIcon(QIcon(":icons/images/color_refresh.svg"));
    refresh_todo_list_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    refresh_todo_list_button->setFont(font);
    page_info_layout->addWidget(refresh_todo_list_button);

    page_info_layout->addSpacing(5);

    vote_status_text = new ElaText(page);
    vote_status_text->setText("状态:");
    vote_status_text->setFont(font);
    page_info_layout->addWidget(vote_status_text);

    select_vote_status_combox = new ElaMultiSelectComboBox(page);
    select_vote_status_combox->setFixedHeight(27);
    select_vote_status_combox->setMaximumHeight(30);
    select_vote_status_combox->setFont(font);
    // the order must keep same with the xx
    size_t     n_status = static_cast<size_t>(VoteStatus::count);
    QList<int> ss;
    for (size_t i = 0; i < n_status; ++i) {
        ss.append(i);
        select_vote_status_combox->addItem(get_vote_status_display_str(static_cast<VoteStatus>(i)));
    }
    select_vote_status_combox->setCurrentSelection(ss);
    page_info_layout->addWidget(select_vote_status_combox);

    page_info_layout->addSpacing(8);

    vote_process_status_text = new ElaText(page);
    vote_process_status_text->setText("进度:");
    vote_process_status_text->setFont(font);
    page_info_layout->addWidget(vote_process_status_text);

    select_vote_process_status_combox = new ElaMultiSelectComboBox(page);
    select_vote_process_status_combox->setFixedHeight(27);
    select_vote_process_status_combox->setFont(font);

    ss.clear();
    for (size_t i = 0; i < static_cast<size_t>(VoteProcessStatus::count); ++i) {
        ss.append(i);
        select_vote_process_status_combox->addItem(
            get_vote_process_status_display_str(static_cast<VoteProcessStatus>(i)));
    }
    select_vote_process_status_combox->setCurrentSelection(ss);
    page_info_layout->addWidget(select_vote_process_status_combox);
    page_info_layout->addStretch();


    current_todo_list_page = new ElaSpinBox(page);
    current_todo_list_page->setFixedHeight(27);
    current_todo_list_page->setRange(0, 0);
    current_todo_list_page->setFont(font);
    page_info_layout->addWidget(current_todo_list_page);
    page_info_layout->addSpacing(2);

    totoal_todo_list_page = new ElaText(page);
    totoal_todo_list_page->setFont(font);
    totoal_todo_list_page->setMinimumWidth(160);
    totoal_todo_list_page->setText(" / 总数");
    page_info_layout->addWidget(totoal_todo_list_page);
    page_info_layout->addStretch();
    _central_widget->setWindowTitle("Start Vote");
    page->addCentralWidget(_central_widget);
}

}   // namespace client
}   // namespace tang