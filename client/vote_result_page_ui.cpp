#include "vote_result_page_ui.h"
#include <QHeaderView>
#include <QSplitter>
#include <QVBoxLayout>


namespace tang {
namespace client {
void VoteResultPageUi::setup_ui(ElaScrollPage* page) {
    QWidget*     central_widget  = new QWidget(page);
    QVBoxLayout* _central_layout = new QVBoxLayout(central_widget);

    QWidget* color_central_widget = new QWidget(page);
    _central_layout->addWidget(color_central_widget);
    QVBoxLayout* central_layout = new QVBoxLayout(color_central_widget);

    constexpr int key_text_width         = 80;
    constexpr int value_text_width       = 150;
    constexpr int container_fixed_height = 36;

    // horizontal spliter for vote detail and vote plot!
    QSplitter* vote_detail_spliter = new QSplitter(page);
    vote_detail_spliter->setOrientation(Qt::Horizontal);
    vote_detail_spliter->setOpaqueResize(true);
    central_layout->addWidget(vote_detail_spliter);

    QWidget* vote_detail_container = new QWidget(page);
    // limit the width of vote_detail_container!
    // vote_detail_container->setMinimumWidth(420);
    // vote_detail_container->setMaximumWidth(480);
    vote_detail_container->setFixedWidth(400);
    // not add the vote detail container to the central layout!
    //  central_layout->addWidget(vote_detail_container);

    vote_detail_spliter->addWidget(vote_detail_container);
    QVBoxLayout* vote_detail_layout = new QVBoxLayout(vote_detail_container);

    // set font size!
    QWidget* vote_creator_container = new QWidget(page);
    vote_creator_container->setFixedHeight(container_fixed_height);
    vote_detail_layout->addWidget(vote_creator_container);
    QHBoxLayout* vote_creator_layout = new QHBoxLayout(vote_creator_container);

    vote_creator_text = new ElaText(page);
    auto font         = vote_creator_text->font();
    font.setFamily("SimSun");
    font.setBold(true);
    font.setPointSize(10);
    vote_creator_text->setText("发起人:");
    vote_creator_text->setFont(font);
    vote_creator_text->setMinimumWidth(key_text_width);
    vote_creator_layout->addWidget(vote_creator_text);

    vote_creator_name_text = new ElaText(page);
    // vote_creator_name_text->setText("ゴウェイ");
    vote_creator_name_text->setFont(font);
    vote_creator_layout->addWidget(vote_creator_name_text);
    vote_creator_layout->addStretch();

    QWidget* vote_create_time_container = new QWidget(page);
    vote_create_time_container->setFixedHeight(container_fixed_height);
    vote_detail_layout->addWidget(vote_create_time_container);
    QHBoxLayout* vote_create_time_layout = new QHBoxLayout(vote_create_time_container);

    vote_create_time = new ElaText(page);
    vote_create_time->setFont(font);
    vote_create_time->setMinimumWidth(key_text_width);
    vote_create_time->setText("发起时间:");
    vote_create_time_layout->addWidget(vote_create_time);

    vote_create_time_value = new ElaText(page);
    vote_create_time_value->setFont(font);
    vote_create_time_value->setMinimumWidth(value_text_width);
    // vote_create_time_value->setText("2025-08-27 19:27:31");
    vote_create_time_layout->addWidget(vote_create_time_value);
    vote_create_time_layout->addStretch();

    // topic
    QWidget* vote_topic_container = new QWidget(page);
    vote_topic_container->setFixedHeight(container_fixed_height);
    vote_detail_layout->addWidget(vote_topic_container);
    QHBoxLayout* vote_topic_layout = new QHBoxLayout(vote_topic_container);

    vote_topic_text = new ElaText(page);
    vote_topic_text->setText("主题:");
    vote_topic_text->setFont(font);
    vote_topic_text->setMinimumWidth(key_text_width);
    vote_topic_layout->addWidget(vote_topic_text);

    vote_topic_value = new ElaText(page);
    // vote_topic_value->setText("今天活动安排");
    vote_topic_value->setMinimumWidth(value_text_width);
    vote_topic_value->setFont(font);
    vote_topic_layout->addWidget(vote_topic_value);
    vote_topic_layout->addStretch();

    QWidget* vote_choice_type_container = new QWidget(page);
    vote_choice_type_container->setFixedHeight(container_fixed_height);
    vote_detail_layout->addWidget(vote_choice_type_container);
    QHBoxLayout* vote_choice_type_layout = new QHBoxLayout(vote_choice_type_container);

    vote_choice_type = new ElaText(page);
    vote_choice_type->setText("类型:");
    vote_choice_type->setFont(font);
    vote_choice_type->setMinimumWidth(key_text_width);
    vote_choice_type_layout->addWidget(vote_choice_type);

    vote_choice_type_value = new ElaText(page);
    vote_choice_type_value->setFont(font);
    // vote_choice_type_value->setText("单选");
    vote_choice_type_layout->addWidget(vote_choice_type_value);
    vote_choice_type_layout->addStretch();

    QWidget* vote_result_container = new QWidget(page);
    vote_result_container->setFixedHeight(container_fixed_height);
    vote_detail_layout->addWidget(vote_result_container);
    QHBoxLayout* vote_result_layout = new QHBoxLayout(vote_result_container);

    vote_result_text = new ElaText(page);
    vote_result_text->setText("投票结果:");
    vote_result_text->setMinimumWidth(key_text_width);
    vote_result_text->setFont(font);
    vote_result_layout->addWidget(vote_result_text);

    vote_result_value = new ElaText(page);
    vote_result_value->setFont(font);
    vote_result_layout->addWidget(vote_result_value);
    vote_result_layout->addStretch();

    QWidget* vote_count_container = new QWidget(page);
    vote_detail_spliter->addWidget(vote_count_container);
    QVBoxLayout* vote_count_layout = new QVBoxLayout(vote_count_container);

    vote_count_view_text = new ElaText(page);
    vote_count_view_text->setFont(font);
    vote_count_view_text->setText("投票统计");
    QHBoxLayout* t1 = new QHBoxLayout();
    t1->addStretch();
    t1->addWidget(vote_count_view_text);
    t1->addStretch();

    vote_count_layout->addLayout(t1);


    vote_stat_view = new ElaTableView(page);
    // vote_stat_view->setMinimumWidth(160);
    // vote_stat_view->setMaximumWidth(480);
    vote_stat_view->verticalHeader()->setFont(font);
    vote_stat_view->setAlternatingRowColors(true);
    vote_stat_view->horizontalHeader()->setFont(font);
    vote_stat_view->setIconSize(QSize(21, 21));
    vote_stat_view->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    vote_stat_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vote_stat_view->setFont(font);
    vote_count_layout->addWidget(vote_stat_view);


    QSplitter* vote_view_spliter = new QSplitter(page);
    // vote_view_spliter->setOpaqueResize(true);
    vote_view_spliter->setOrientation(Qt::Horizontal);
    central_layout->addWidget(vote_view_spliter);

    QWidget* vote_img_container = new QWidget(page);
    vote_img_container->setMinimumSize({400, 400});
    vote_view_spliter->addWidget(vote_img_container);
    QVBoxLayout* vote_img_layout = new QVBoxLayout(vote_img_container);
    vote_img_layout->setContentsMargins({0, 0, 0, 0});

    vote_img_series = new QPieSeries(page);
    vote_img_series->setLabelsPosition(QPieSlice::LabelOutside);
    // how to set the parent of chart?
    vote_img_chart = new QChart();
    vote_img_chart->addSeries(vote_img_series);
    vote_img_chart->setTitle("投票统计图");
    vote_img_chart->setAnimationOptions(QChart::SeriesAnimations);
    auto legend_font = font;
    legend_font.setPointSize(7);
    vote_img_chart->legend()->setAlignment(Qt::AlignRight);
    vote_img_chart->legend()->setBackgroundVisible(false);
    // set the font of chat legend!
    vote_img_chart->legend()->setFont(legend_font);

    auto title_font = font;
    title_font.setPointSize(12);
    vote_img_chart->setTitleFont(title_font);
    vote_img_chart->setTheme(QChart::ChartThemeLight);


    vote_img_view = new QChartView(page);
    vote_img_view->setChart(vote_img_chart);
    vote_img_view->setContentsMargins({0, 0, 0, 0});
    vote_img_view->setRenderHint(QPainter::Antialiasing);
    // add
    vote_img_layout->addWidget(vote_img_view);

    QWidget* vote_history_container = new QWidget(page);
    vote_view_spliter->addWidget(vote_history_container);


    QVBoxLayout* vote_history_layout = new QVBoxLayout(vote_history_container);
    vote_history_view_text           = new ElaText(page);
    vote_history_view_text->setFont(font);
    vote_history_view_text->setText("投票历史记录");
    QHBoxLayout* t2 = new QHBoxLayout();
    t2->addStretch();
    t2->addWidget(vote_history_view_text);
    t2->addStretch();
    vote_history_layout->addLayout(t2);

    // ElaText* vote_history_text = new ElaText(page);
    // vote_history_text->setText("投票纪录");
    // vote_history_text->setFont(font);
    // central_layout->addWidget(vote_history_text);

    vote_history_view = new ElaTableView(page);
    vote_history_view->verticalHeader()->setHidden(true);
    vote_history_view->horizontalHeader()->setFont(font);
    vote_history_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    // vote_history_view->setMinimumWidth(300);
    vote_history_view->setFont(font);
    vote_history_view->setMinimumHeight(200);
    vote_history_view->setMinimumWidth(300);
    vote_history_layout->addWidget(vote_history_view);


    QWidget* vote_page_container = new QWidget(page);
    vote_history_layout->addWidget(vote_page_container);
    QHBoxLayout* vote_page_layout = new QHBoxLayout(vote_page_container);

    refresh_page_button = new ElaToolButton(page);
    refresh_page_button->setText("刷新");
    refresh_page_button->setFont(font);
    refresh_page_button->setIcon(QIcon(":icons/images/color_refresh.svg"));
    refresh_page_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    vote_page_layout->addWidget(refresh_page_button);

    vote_page_layout->addStretch();

    current_history_page = new ElaSpinBox(page);
    current_history_page->setRange(0, 0);
    current_history_page->setFont(font);
    current_history_page->setMaximumHeight(27);
    vote_page_layout->addWidget(current_history_page);
    vote_page_layout->addSpacing(9);

    total_history_page = new ElaText(page);
    total_history_page->setText("总页数");
    total_history_page->setFont(font);

    vote_page_layout->addWidget(total_history_page);
    vote_page_layout->addStretch();

    adjust_history_view_button = new ElaToolButton(page);
    adjust_history_view_button->setFont(font);
    adjust_history_view_button->setText("调整视图");
    adjust_history_view_button->setIcon(QIcon(":icons/images/spider.svg"));
    adjust_history_view_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    vote_page_layout->addWidget(adjust_history_view_button);


    central_widget->setWindowTitle("~~~Vote result~~~");
    page->addCentralWidget(central_widget);
}
}   // namespace client
}   // namespace tang