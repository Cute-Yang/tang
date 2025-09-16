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

    constexpr int key_text_width         = 60;
    constexpr int value_text_width       = 150;
    constexpr int container_fixed_height = 36;

    // ElaText* vote_detail_text = new ElaText(page);
    // auto font = vote_detail_text->font();
    // vote_detail_text->setText("投票详情");
    // vote_detail_text->setFont(font);
    // central_layout->addWidget(vote_detail_text);

    // horizontal spliter for vote detail and vote plot!
    QSplitter* vote_detail_spliter = new QSplitter(page);
    vote_detail_spliter->setOrientation(Qt::Horizontal);
    vote_detail_spliter->setOpaqueResize(true);
    central_layout->addWidget(vote_detail_spliter);

    QWidget* vote_detail_container = new QWidget(page);
    // limit the width of vote_detail_container!
    // vote_detail_container->setMinimumWidth(420);
    // vote_detail_container->setMaximumWidth(480);
    vote_detail_container->setFixedWidth(450);
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
    font.setPointSize(9);
    vote_creator_text->setText("发起人:");
    vote_creator_text->setFont(font);
    vote_creator_text->setMinimumWidth(key_text_width);
    vote_creator_layout->addWidget(vote_creator_text);

    vote_creator_name_text = new ElaText(page);
    vote_creator_name_text->setText("ゴウェイ");
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
    vote_create_time_value->setText("2025-08-27 19:27:31");
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
    vote_topic_value->setText("今天活动安排");
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
    vote_choice_type_value->setText("单选");
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
    vote_result_value->setText("爬山");
    vote_result_value->setFont(font);
    vote_result_layout->addWidget(vote_result_value);
    vote_result_layout->addStretch();

    // ElaText* vote_img_text = new ElaText(page);
    // vote_img_text->setText("投票统计");
    // vote_img_text->setFont(font);
    // central_layout->addWidget(vote_img_text);
    // QSplitter* spliter = new QSplitter(page);
    // central_layout->addWidget(spliter);

    vote_stat_view = new ElaTableView(page);
    vote_stat_view->setMinimumWidth(160);
    vote_stat_view->setMaximumWidth(480);
    vote_stat_view->verticalHeader()->setFont(font);
    vote_stat_view->setAlternatingRowColors(true);
    vote_stat_view->horizontalHeader()->setFont(font);
    vote_stat_view->setIconSize(QSize(21,21));
    vote_stat_view->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    vote_stat_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vote_stat_view->setFont(font);
    // spliter->addWidget(vote_stat_view);
    // this is a table view,contains vote options,vote values
    // central_layout->addWidget(vote_stat_view);
    // add it to the vote detail spliter!
    vote_detail_spliter->addWidget(vote_stat_view);


    QSplitter* vote_view_spliter = new QSplitter(page);
    vote_view_spliter->setOrientation(Qt::Horizontal);
    central_layout->addWidget(vote_view_spliter);


    vote_view_spliter->setOpaqueResize(true);
    QWidget* vote_img_container = new QWidget(page);
    // vote_img_container->setMinimumHeight(300);
    // vote_img_container->setMinimumWidth(420);
    // vote_img_container->setMaximumWidth(480);
    vote_img_container->setFixedWidth(450);
    // vote_detail_spliter->addWidget(vote_img_container);
    // central_layout->addWidget(vote_img_container);
    vote_view_spliter->addWidget(vote_img_container);

    QVBoxLayout* vote_img_layout = new QVBoxLayout(vote_img_container);
    vote_img_layout->setContentsMargins({0, 0, 0, 0});

    vote_img_series = new QPieSeries(page);
    // how to set the parent of chart?
    vote_img_chart = new QChart();
    vote_img_chart->addSeries(vote_img_series);
    vote_img_chart->setTitle("vote stat");
    vote_img_chart->setAnimationOptions(QChart::SeriesAnimations);
    vote_img_chart->legend()->setAlignment(Qt::AlignRight);
    vote_img_chart->legend()->setBackgroundVisible(false);
    // set the font of chat legend!
    vote_img_chart->legend()->setFont(font);
    vote_img_chart->setTitleFont(font);
    vote_img_chart->setTheme(QChart::ChartThemeLight);

    vote_img_view = new QChartView(page);
    // auto policy = vote_img_view->sizePolicy();
    // policy.setHorizontalPolicy(QSizePolicy::Expanding);
    // vote_img_view->setSizePolicy(policy);
    // I don't know the time to release the vote_img_chart!
    vote_img_view->setChart(vote_img_chart);
    vote_img_view->setRenderHint(QPainter::Antialiasing);
    // add
    vote_img_layout->addWidget(vote_img_view);


    // ElaText* vote_history_text = new ElaText(page);
    // vote_history_text->setText("投票纪录");
    // vote_history_text->setFont(font);
    // central_layout->addWidget(vote_history_text);
    vote_history_view = new ElaTableView(page);
    // vote_history_view->setMinimumWidth(300);
    vote_history_view->setMinimumHeight(200);
    // central_layout->addWidget(vote_history_view);
    vote_view_spliter->addWidget(vote_history_view);

    central_widget->setWindowTitle("~~~Vote result~~~");
    page->addCentralWidget(central_widget);
}
}   // namespace client
}   // namespace tang