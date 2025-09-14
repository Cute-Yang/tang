#include "vote_result_page.h"
#include "vote_result_view_model.h"

namespace tang {
namespace client {
VoteResultPage::VoteResultPage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new VoteResultPageUi())
    , vote_result_model(new VoteResultViewModel("投票项", "计数","percent", this)) {
    ui->setup_ui(this);
    ui->vote_stat_view->setModel(vote_result_model);
    this->set_test_data();
}

VoteResultPage::~VoteResultPage() {
    delete ui;
}


void VoteResultPage::set_test_data() {
    // colors for plot
    static const QStringList colors = {
        "#6480D6",
        "#A1DC85",
        "#FFAD25",
        "#FF7777",
        "#84D1EF",
        "#4CB383",
    };

    // set datas
    QList<int> plot_values = {3, 2, 4, 5, 3};

    // set plot labels
    QList<QString> plot_labels = {"香蕉", "苹果", "橘子", "樱桃", "菠萝"};

    this->vote_result_model->set_vote_items(plot_labels);
    this->vote_result_model->set_vote_counts(plot_values);
    // add series
    ui->vote_img_series->setLabelsVisible(true);
    ui->vote_img_series->setLabelsPosition(QPieSlice::LabelInsideHorizontal);

    // clear the previous data
    ui->vote_img_series->clear();

    // why the assert is passed?
    assert(plot_values.size() == plot_labels.size());
    qDebug() << "start add slice for plot!";
    for (size_t i = 0; i < plot_values.size(); ++i) {
        // if we clear the slices,when the slice will be deallocated?
        QPieSlice* slice = new QPieSlice(this);
        slice->setLabelVisible(true);
        slice->setValue(plot_values[i]);
        slice->setLabel(plot_labels[i]);

        size_t color_index = i % colors.size();
        qDebug() << "the color size is " << color_index;

        slice->setColor(colors[color_index]);

        QFont font;
        font.setFamily("SimSun");
        font.setBold(true);
        font.setPointSizeF(8);

        slice->setLabelFont(font);
        // avoid out of index!
        slice->setLabelColor(colors[color_index]);

        slice->setBorderColor(colors[color_index]);
        if (!ui->vote_img_series->append(slice)) {
            qDebug() << "fail to add slice with value:" << plot_values[i]
                     << " label:" << plot_labels[i];
        }
    }

    qDebug() << "add all the slices for plot!";
}

}   // namespace client
}   // namespace tang