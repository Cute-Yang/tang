#include "vote_page.h"
#include "ElaMessageBar.h"
#include "vote_item_view_model.h"
#include <QStringListModel>


namespace tang {
namespace client {
VotePage::VotePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new VotePageUi)
    , vote_data(new VoteItemViewModel("投票项", this)) {
    ui->setup_ui(this);
    this->setTitleVisible(false);
    ui->vote_items->setModel(vote_data);

    QStringList vote_history_cols = {"id", "创建者", "创建时间", "主题", "类型", "check"};
    vote_history_model            = new VoteHistoryViewModel(vote_history_cols, 10, this);
    ui->vote_history_view->setModel(vote_history_model);

    QStringList items = {"钓鱼", "打球", "爬山", "LOL", "游泳", "骑自行车"};
    vote_data->add_vote_items(items);
    this->set_test_model();

    connect(ui->vote_history_view,
            &ElaTableView::tableViewShow,
            this,
            &VotePage::adjust_vote_history_view);

    // move the connect to func
    connect(ui->add_vote_item_button, &ElaToolButton::clicked, this, [this]() {
        auto text = ui->add_vote_item_line_edit->text();
        if (!vote_data->add_vote_item(text)) {
            ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                                   "vote",
                                   QString("'%1' 已经存在啦,不需要添加~~~").arg(text),
                                   1200,
                                   static_cast<QWidget*>(this->parent()));
        } else {
            ElaMessageBar::success(ElaMessageBarType::BottomRight,
                                   "vote",
                                   QString("'%1' 添加成功^_^").arg(text),
                                   1200,
                                   static_cast<QWidget*>(this->parent()));
        }
    });


    // connect(ui->vote_items,&ElaTableView::tableViewShow,this,[this](){
    //     ui->vote_items->setColumnWidth(0,ui->vote_items->width());
    // });

    connect(ui->vote_items, &ElaTableView::clicked, this, &VotePage::click_vote_items);
}

VotePage::~VotePage() {
    delete ui;
    delete vote_data;
}

void VotePage::set_test_model() {
    // QStringList items = {"钓鱼", "打球", "爬山", "LOL", "游泳", "骑自行车"};

    // ui->vote_items->setModel(m);
    // auto g = ui->vote_items->model();
}

void VotePage::click_vote_items(const QModelIndex& index) {
    if (index.isValid()) {
        int           row       = index.row();
        int           col       = index.column();
        const QString vote_item = this->vote_data->get_vote_item(row);
        ElaMessageBar::information(ElaMessageBarType::TopLeft,
                                   "click vote item",
                                   QString("当前选中的投票项是%1").arg(vote_item),
                                   2700,
                                   this);
    }
}


void VotePage::adjust_vote_history_view() {
    auto width = ui->vote_history_view->width();
    int  w0    = std::max(12,static_cast<int>(0.05 * width));
    int  w1    = static_cast<int>(0.15 * width);
    int  w2    = static_cast<int>(0.3 * width);
    int  w3    = static_cast<int>(0.3 * width);
    int  w4    = std::max(16,static_cast<int>(0.1 * width));
    int  w5    = static_cast<int>(0.1 * width);

    ui->vote_history_view->setColumnWidth(0, w0);
    ui->vote_history_view->setColumnWidth(1, w1);
    ui->vote_history_view->setColumnWidth(2, w2);
    ui->vote_history_view->setColumnWidth(3, w3);
    ui->vote_history_view->setColumnWidth(4, w4);
    ui->vote_history_view->setColumnWidth(5, w5);
}

}   // namespace client
}   // namespace tang