#include "vote_page_ui.h"
#include <ElaComboBox.h>
#include <ElaText.h>
#include <QHeaderView>
#include <QSplitter>
#include <QVBoxLayout>




namespace tang {
namespace client {
void VotePageUi::setup_ui(ElaScrollPage* page) {
    QWidget* central_widget         = new QWidget(page);
    auto     _central_widget_layout = new QVBoxLayout(central_widget);
    auto     color_central_widget   = new QWidget(page);
    _central_widget_layout->addWidget(color_central_widget);

    color_central_widget->setObjectName("vote_page_central_widget");
    QString central_widget_style = R"style(
        QWidget#vote_page_central_widget{
            background-color:#BEEDC7;
        }
        )style";
    // color_central_widget->setStyleSheet(central_widget_style);
    QVBoxLayout*  central_layout         = new QVBoxLayout(color_central_widget);
    constexpr int text_width             = 160;
    constexpr int combox_key_width       = text_width - 5;
    constexpr int container_fixed_height = 42;
    QWidget*      vote_person_container  = new QWidget(page);
    vote_person_container->setFixedHeight(container_fixed_height);
    central_layout->addWidget(vote_person_container);
    QHBoxLayout* vote_creator_layout = new QHBoxLayout(vote_person_container);

    vote_creator_text = new ElaText(page);
    auto font         = vote_creator_text->font();
    font.setFamily("SimSun");
    font.setPointSize(9);
    font.setBold(true);

    auto small_font = font;
    small_font.setPointSize(9);

    vote_creator_text->setText("发起人:");
    vote_creator_text->setMinimumWidth(text_width);
    vote_creator_text->setFont(font);
    vote_creator_layout->addWidget(vote_creator_text);

    vote_creator_name_text = new ElaText(page);
    vote_creator_name_text->setText("王昭君");
    vote_creator_name_text->setFont(small_font);
    vote_creator_layout->addWidget(vote_creator_name_text);
    vote_creator_layout->addStretch();

    QWidget* vote_topic_container = new QWidget(page);
    vote_topic_container->setFixedHeight(container_fixed_height);
    central_layout->addWidget(vote_topic_container);
    QHBoxLayout* vote_topic_layout = new QHBoxLayout(vote_topic_container);

    vote_topic_text = new ElaText(page);
    vote_topic_text->setMinimumWidth(text_width);
    vote_topic_text->setText("主题:");
    vote_topic_text->setFont(font);
    vote_topic_layout->addWidget(vote_topic_text);

    vote_topic_line_edit = new ElaLineEdit(page);
    vote_topic_line_edit->setPlaceholderText("投票主题~");
    vote_topic_line_edit->setMinimumWidth(500);
    vote_topic_line_edit->setMaximumWidth(900);
    vote_topic_line_edit->setFont(small_font);
    vote_topic_layout->addWidget(vote_topic_line_edit);
    vote_topic_layout->addStretch();


    QWidget* vote_choice_type_container = new QWidget(page);
    vote_choice_type_container->setFixedHeight(container_fixed_height);
    central_layout->addWidget(vote_choice_type_container);
    QHBoxLayout* vote_choice_type_layout = new QHBoxLayout(vote_choice_type_container);
    vote_choice_type                     = new ElaText(page);
    vote_choice_type->setFont(font);
    vote_choice_type->setText("投票类型:");
    vote_choice_type->setMinimumWidth(combox_key_width);
    vote_choice_type_layout->addWidget(vote_choice_type);

    vote_choice_type_combox = new ElaComboBox(page);
    vote_choice_type_combox->addItems({"单选", "多选"});
    vote_choice_type_combox->setMinimumWidth(48);
    vote_choice_type_combox->setFont(small_font);
    vote_choice_type_layout->addWidget(vote_choice_type_combox);
    vote_choice_type_layout->addStretch();


    QWidget* start_vote_container = new QWidget(page);
    start_vote_container->setFixedHeight(container_fixed_height);
    central_layout->addWidget(start_vote_container);
    QHBoxLayout* start_vote_layout = new QHBoxLayout(start_vote_container);


    voters_text = new ElaText(page);
    voters_text->setText("选择参与投票的人:");
    voters_text->setFont(font);
    voters_text->setMinimumWidth(combox_key_width);
    start_vote_layout->addWidget(voters_text);

    voters_combox = new ElaMultiSelectComboBox(page);
    voters_combox->setMinimumWidth(270);
    voters_combox->setFont(small_font);

    // test datas
    voters_combox->addItems({"小鱼",
                             "柯南",
                             "犬夜叉",
                             "戈薇",
                             "桔梗",
                             "小鱼",
                             "柯南",
                             "犬夜叉",
                             "戈薇",
                             "桔梗",
                             "小鱼",
                             "柯南",
                             "犬夜叉",
                             "戈薇",
                             "桔梗"});
    voters_combox->setCurrentSelection(3);
    start_vote_layout->addWidget(voters_combox);

    start_vote_layout->addSpacing(6);
    flush_voters_button = new ElaToolButton(page);
    flush_voters_button->setIcon(QIcon(":icons/images/flush.svg"));
    flush_voters_button->setToolTip("flush~");
    flush_voters_button->setText("刷新");
    flush_voters_button->setFont(font);
    flush_voters_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    flush_voters_button->setIconSize(QSize(21, 21));
    start_vote_layout->addWidget(flush_voters_button);

    start_vote_layout->addStretch();

    start_vote_button = new ElaToolButton(page);
    start_vote_button->setText("发起投票");
    start_vote_button->setFont(font);
    start_vote_button->setIcon(QIcon(":icons/images/vote.svg"));
    start_vote_button->setIconSize(QSize(21, 21));
    start_vote_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    start_vote_layout->addWidget(start_vote_button);


    QSplitter* spliter = new QSplitter(page);
    central_layout->addWidget(spliter);
    spliter->setOrientation(Qt::Horizontal);
    spliter->setOpaqueResize(true);
    vote_items = new ElaTableView(page);
    vote_items->setIconSize(QSize(21, 21));
    vote_items->setFont(small_font);
    // make the column width adapt with widget,important!
    vote_items->horizontalHeader()->setFont(small_font);
    vote_items->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    vote_items->setAlternatingRowColors(true);
    vote_items->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vote_items->setMinimumWidth(120);
    spliter->addWidget(vote_items);

    QWidget* vote_item_operator_container = new QWidget(page);
    spliter->addWidget(vote_item_operator_container);
    // vote_item_operator_container->setMinimumWidth(270);
    QVBoxLayout* vote_item_operator_layout = new QVBoxLayout(vote_item_operator_container);


    QWidget* add_vote_item_container = new QWidget(page);
    vote_item_operator_layout->addWidget(add_vote_item_container);
    QHBoxLayout* add_vote_item_layout = new QHBoxLayout(add_vote_item_container);

    add_vote_item_line_edit = new ElaLineEdit(page);
    add_vote_item_line_edit->setFont(small_font);
    add_vote_item_line_edit->setPlaceholderText("add...");
    add_vote_item_layout->addWidget(add_vote_item_line_edit);
    add_vote_item_layout->addSpacing(8);

    add_vote_item_button = new ElaToolButton(page);
    // add_vote_item_button->setToolTip("增加");
    add_vote_item_button->setText("添加");
    add_vote_item_button->setIcon(QIcon(":icons/images/add_item.svg"));
    add_vote_item_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    add_vote_item_button->setIconSize(QSize(16, 16));
    add_vote_item_layout->addWidget(add_vote_item_button);
    vote_item_operator_layout->addStretch();


    central_widget->setWindowTitle("~~~Vote~~~");
    page->addCentralWidget(central_widget);
}
}   // namespace client
}   // namespace tang