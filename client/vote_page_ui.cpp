#include "vote_page_ui.h"
#include "common/status.h"
#include "util.h"
#include <ElaComboBox.h>
#include <ElaText.h>
#include <QHeaderView>
#include <QSplitter>
#include <QVBoxLayout>



using namespace tang::common;
namespace tang {
namespace client {
void VotePageUi::setup_ui(ElaScrollPage* page) {
    QWidget* central_widget         = new QWidget(page);
    auto     _central_widget_layout = new QVBoxLayout(central_widget);
    auto     color_central_widget   = new QWidget(page);
    _central_widget_layout->addWidget(color_central_widget);
    QVBoxLayout*  central_layout         = new QVBoxLayout(color_central_widget);
    constexpr int text_width             = 160;
    constexpr int combox_key_width       = text_width - 5;
    constexpr int container_fixed_height = 36;
    QWidget*      vote_person_container  = new QWidget(page);
    vote_person_container->setFixedHeight(container_fixed_height);
    central_layout->addWidget(vote_person_container);
    QHBoxLayout* vote_creator_layout = new QHBoxLayout(vote_person_container);

    vote_creator_key = new ElaText(page);
    auto key_font    = vote_creator_key->font();
    key_font.setFamily("SimSun");
    key_font.setPointSize(10);
    key_font.setBold(true);

    auto value_font = key_font;
    value_font.setPointSize(10);

    vote_creator_key->setText("发起人:");
    vote_creator_key->setMinimumWidth(text_width);
    vote_creator_key->setFont(key_font);
    vote_creator_layout->addWidget(vote_creator_key);
    vote_creator_layout->addSpacing(5);

    vote_creator_value = new ElaText(page);
    vote_creator_value->setText("王昭君");
    vote_creator_value->setFont(value_font);
    vote_creator_layout->addWidget(vote_creator_value);
    vote_creator_layout->addStretch();

    QWidget* vote_topic_container = new QWidget(page);
    vote_topic_container->setFixedHeight(container_fixed_height);
    central_layout->addWidget(vote_topic_container);
    QHBoxLayout* vote_topic_layout = new QHBoxLayout(vote_topic_container);

    vote_topic_key = new ElaText(page);
    vote_topic_key->setMinimumWidth(text_width);
    vote_topic_key->setText("主题:");
    vote_topic_key->setFont(key_font);
    vote_topic_layout->addWidget(vote_topic_key);

    vote_topic_line_edit = new ElaLineEdit(page);
    vote_topic_line_edit->setFixedHeight(30);
    vote_topic_line_edit->setPlaceholderText("投票主题~");
    vote_topic_line_edit->setMinimumWidth(500);
    vote_topic_line_edit->setMaximumWidth(900);
    vote_topic_line_edit->setFont(value_font);
    vote_topic_layout->addWidget(vote_topic_line_edit);
    vote_topic_layout->addStretch();


    QWidget* vote_choice_type_container = new QWidget(page);
    vote_choice_type_container->setFixedHeight(container_fixed_height);
    central_layout->addWidget(vote_choice_type_container);
    QHBoxLayout* vote_choice_type_layout = new QHBoxLayout(vote_choice_type_container);
    vote_choice_type_key                 = new ElaText(page);
    vote_choice_type_key->setFont(key_font);
    vote_choice_type_key->setText("投票类型:");
    vote_choice_type_key->setMinimumWidth(combox_key_width);
    vote_choice_type_layout->addWidget(vote_choice_type_key);

    vote_choice_type_combox = new ElaComboBox(page);
    vote_choice_type_combox->setFixedHeight(30);
    for (size_t i = 0; i < static_cast<size_t>(VoteChoiceType::count); ++i) {
        vote_choice_type_combox->addItem(
            get_choice_type_display_str(static_cast<VoteChoiceType>(i)));
    }

    vote_choice_type_combox->setMinimumWidth(48);
    vote_choice_type_combox->setFont(value_font);
    vote_choice_type_layout->addWidget(vote_choice_type_combox);
    vote_choice_type_layout->addStretch();

    QWidget* start_vote_container = new QWidget(page);
    start_vote_container->setFixedHeight(container_fixed_height);
    central_layout->addWidget(start_vote_container);
    QHBoxLayout* start_vote_layout = new QHBoxLayout(start_vote_container);

    voters_key = new ElaText(page);
    voters_key->setText("选择参与投票的人:");
    voters_key->setFont(key_font);
    voters_key->setMinimumWidth(combox_key_width);
    start_vote_layout->addWidget(voters_key);

    voters_combox = new ElaMultiSelectComboBox(page);
    voters_combox->setFixedHeight(30);
    voters_combox->setMinimumWidth(270);
    voters_combox->setFont(value_font);

    // test datas
    // voters_combox->addItems({"小鱼", "柯南", "犬夜叉", "戈薇", "桔梗", "小鱼", "柯南"});
    // voters_combox->setCurrentSelection(3);
    start_vote_layout->addWidget(voters_combox);

    start_vote_layout->addSpacing(6);
    flush_voters_button = new ElaToolButton(page);
    flush_voters_button->setFixedHeight(30);
    flush_voters_button->setIcon(QIcon(":icons/images/color_refresh.svg"));
    flush_voters_button->setToolTip("refresh~");
    flush_voters_button->setText("刷新");
    flush_voters_button->setFont(key_font);
    flush_voters_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    flush_voters_button->setIconSize(QSize(21, 21));
    start_vote_layout->addWidget(flush_voters_button);

    start_vote_layout->addStretch();

    QSplitter* spliter = new QSplitter(page);
    central_layout->addWidget(spliter);
    spliter->setOrientation(Qt::Horizontal);
    spliter->setOpaqueResize(true);


    QWidget* vote_items_container = new QWidget(page);
    vote_items_container->setMaximumWidth(350);
    spliter->addWidget(vote_items_container);

    QVBoxLayout* vote_items_layout = new QVBoxLayout(vote_items_container);

    auto vote_button_container = new QWidget(page);
    vote_items_layout->addWidget(vote_button_container);
    auto vote_button_layout = new QHBoxLayout(vote_button_container);
    vote_button_layout->addStretch();

    new_vote_button = new ElaToolButton(page);
    new_vote_button->setText("新的投票");
    new_vote_button->setFont(key_font);
    new_vote_button->setIcon(QIcon(":icons/images/vote.svg"));
    new_vote_button->setIconSize(QSize(21, 21));
    new_vote_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    vote_button_layout->addWidget(new_vote_button);
    vote_button_layout->addSpacing(12);

    start_vote_button = new ElaToolButton(page);
    start_vote_button->setText("发起投票");
    start_vote_button->setFont(key_font);
    start_vote_button->setIcon(QIcon(":icons/images/vote.svg"));
    start_vote_button->setIconSize(QSize(21, 21));
    start_vote_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    vote_button_layout->addWidget(start_vote_button);
    vote_button_layout->addStretch();

    vote_item_view = new ElaTableView(page);
    vote_item_view->setIconSize(QSize(21, 21));
    vote_item_view->setFont(value_font);
    // makeem_viewe column width adapt with widget,important!
    vote_item_view->horizontalHeader()->setFont(key_font);
    vote_item_view->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    vote_item_view->setAlternatingRowColors(true);
    vote_item_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vote_item_view->setMinimumHeight(270);
    vote_item_view->setContextMenuPolicy(Qt::CustomContextMenu);

    vote_items_layout->addWidget(vote_item_view);
    vote_items_layout->addStretch();

    QWidget* add_vote_item_container = new QWidget(page);
    vote_items_layout->addWidget(add_vote_item_container);

    QHBoxLayout* add_vote_item_layout = new QHBoxLayout(add_vote_item_container);

    add_vote_item_line_edit = new ElaLineEdit(page);
    add_vote_item_line_edit->setFont(value_font);
    add_vote_item_line_edit->setPlaceholderText("add...");
    add_vote_item_layout->addWidget(add_vote_item_line_edit);
    add_vote_item_layout->addSpacing(8);

    add_vote_item_button = new ElaToolButton(page);
    add_vote_item_button->setText("添加");
    add_vote_item_button->setIcon(QIcon(":icons/images/add_item.svg"));
    add_vote_item_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    add_vote_item_button->setIconSize(QSize(16, 16));
    add_vote_item_layout->addWidget(add_vote_item_button);


    QWidget*     vote_history_widget = new QWidget(page);
    QVBoxLayout* vote_history_layout = new QVBoxLayout(vote_history_widget);
    spliter->addWidget(vote_history_widget);


    QWidget* vote_history_text_container = new QWidget(page);
    vote_history_layout->addWidget(vote_history_text_container);
    QHBoxLayout* vote_history_text_layout = new QHBoxLayout(vote_history_text_container);
    vote_history_text_layout->addStretch();
    vote_history_text_button = new ElaToolButton(page);
    vote_history_text_button->setIcon(QIcon(":icons/images/butterfly.svg"));
    vote_history_text_button->setFont(key_font);
    vote_history_text_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    vote_history_text_button->setText("投票创建记录");
    vote_history_text_layout->addWidget(vote_history_text_button);
    vote_history_text_layout->addStretch();

    adjust_history_view_button = new ElaToolButton(page);
    adjust_history_view_button->setText("调整视图");
    adjust_history_view_button->setFont(key_font);
    adjust_history_view_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    adjust_history_view_button->setIcon(QIcon(":icons/images/spider.svg"));
    vote_history_text_layout->addWidget(adjust_history_view_button);


    vote_history_view = new ElaTableView(page);
    vote_history_view->horizontalHeader()->setFont(key_font);
    vote_history_view->setFont(value_font);
    vote_history_view->verticalHeader()->hide();
    vote_history_layout->addWidget(vote_history_view);

    QWidget* vote_page_container = new QWidget(page);
    vote_history_layout->addWidget(vote_page_container);

    QHBoxLayout* vote_page_layout = new QHBoxLayout(vote_page_container);
    refresh_vote_history_button   = new ElaToolButton(page);
    refresh_vote_history_button->setIcon(QIcon(":icons/images/color_refresh.svg"));
    refresh_vote_history_button->setText("刷新");
    refresh_vote_history_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    vote_page_layout->addWidget(refresh_vote_history_button);
    vote_page_layout->addSpacing(8);

    // only_select_valid = new ElaToggleSwitch(page);
    // only_select_valid->setIsToggled(false);
    // only_select_valid->setToolTip("只展示投票中~~~");
    // vote_page_layout->addWidget(only_select_valid);
    vote_status_text = new ElaText(page);
    vote_status_text->setFixedWidth(42);
    vote_status_text->setText("状态:");
    vote_status_text->setFont(value_font);
    vote_page_layout->addWidget(vote_status_text);

    select_vote_status_combox = new ElaMultiSelectComboBox(page);
    select_vote_status_combox->setFixedHeight(27);
    select_vote_status_combox->setFont(value_font);
    // the order must keep same with the xx
    size_t n_status = static_cast<size_t>(VoteStatus::count);

    QList<int> ss;
    for (size_t i = 0; i < n_status; ++i) {
        select_vote_status_combox->addItem(get_vote_status_display_str(static_cast<VoteStatus>(i)));
        ss.append(i);
    }
    select_vote_status_combox->setCurrentSelection(ss);
    vote_page_layout->addWidget(select_vote_status_combox);
    vote_page_layout->addSpacing(16);

    vote_page_layout->addStretch();

    current_vote_history_page = new ElaSpinBox(page);
    current_vote_history_page->setFixedHeight(27);
    current_vote_history_page->setRange(0, 8);
    current_vote_history_page->setFont(value_font);
    vote_page_layout->addWidget(current_vote_history_page);
    vote_page_layout->addSpacing(10);
    total_vote_history_pages = new ElaText(page);
    total_vote_history_pages->setText(" / 8");
    total_vote_history_pages->setFont(value_font);
    total_vote_history_pages->setMinimumWidth(200);
    total_vote_history_pages->setEnabled(false);

    vote_page_layout->addWidget(total_vote_history_pages);
    vote_page_layout->addStretch();
    central_widget->setWindowTitle("~~~Vote~~~");
    page->addCentralWidget(central_widget);
}
}   // namespace client
}   // namespace tang