#include "test_plot.h"
#include <QChartView>
#include <QPieSeries>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    this->resize(QSize(800, 600));
    s = new QWidget(parent);
    initChart();
}

void MainWindow::initChart() {
    // 饼状图
    pie_series = new QPieSeries(this);
    connect(pie_series, SIGNAL(clicked(QPieSlice*)), this, SLOT(onPieSeriesClicked(QPieSlice*)));

    // 定义各扇形切片的颜色
    static const QStringList list_pie_color = {
        "#6480D6",
        "#A1DC85",
        "#FFAD25",
        "#FF7777",
        "#84D1EF",
        "#4CB383",
    };

    // 设置数据
    QList<qreal> list_data = {0.31, 1.2, 4.3, 9.4, 12.5, 3.6};

    pie_series->setLabelsVisible(true);
    // pie_series->setLabelsPosition(QPieSlice::LabelInsideHorizontal);
    pie_series->setLabelsPosition(QPieSlice::LabelInsideHorizontal);
    // 扇形
    for (int i = 0; i < list_pie_color.size(); i++) {
        QPieSlice* pie_slice = new QPieSlice(this);
        pie_slice->setLabelVisible(true);
        pie_slice->setValue(list_data[i]);
        pie_slice->setLabel(QString::number(list_data[i]) + "***************************");

        pie_slice->setColor(list_pie_color[i]);
        QFont font;
        font.setFamily("SimSun");
        font.setBold(true);
        font.setPointSizeF(10.5);
        pie_slice->setLabelFont(font);
        pie_slice->setLabelColor(list_pie_color[i]);
        pie_slice->setBorderColor(list_pie_color[i]);
        pie_series->append(pie_slice);
    }



    // 图表视图
    QChart* chart = new QChart;
    chart->setTitle("XXX统计饼图");
    // 设置暗黑主题
    chart->setTheme(QChart::ChartThemeLight);

    // 标题字体
    QFont font = qApp->font();
    font.setBold(true);
    font.setPointSize(16);
    chart->setTitleFont(font);

    // 加入饼图
    chart->addSeries(pie_series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    // 图例
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setBackgroundVisible(false);

    // 加入绘画视图
    QChartView* chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setChart(chart);

    // 加入布局
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    this->setCentralWidget(s);
    s->setLayout(layout);
}

// 点击饼图
void MainWindow::onPieSeriesClicked(QPieSlice* slice) {
    slice->setExploded(!slice->isExploded());
}