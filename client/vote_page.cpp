#include "vote_page.h"
#include "ElaMessageBar.h"
#include "vote_item_view_model.h"
#include <QStringListModel>


namespace tang {
namespace client {
VotePage::VotePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new VotePageUi) {
    ui->setup_ui(this);
    vote_data = new VoteItemViewModel("投票项", ui->vote_items);
    ui->vote_items->setModel(vote_data);
    QStringList items = {"钓鱼", "打球", "爬山", "LOL", "游泳", "骑自行车"};
    vote_data->add_vote_items(items);
    this->set_test_model();

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
}   // namespace client
}   // namespace tang