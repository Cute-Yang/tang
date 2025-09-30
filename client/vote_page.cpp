#include "vote_page.h"
#include "ElaMenu.h"
#include "ElaMessageBar.h"
#include "client_global_config.h"
#include "util.h"
#include "vote_item_view_model.h"
#include <QStringListModel>



namespace tang {
namespace client {
class VoteItemRightMenu {
public:
    ElaMenu* menu;
    QAction* delete_item_action;
    QAction* modify_item_action;
    QAction* move_up_action;
    QAction* move_down_action;

    VoteItemRightMenu(QWidget* parent = nullptr) {
        menu               = new ElaMenu(parent);
        delete_item_action = menu->addAction("删除");
        delete_item_action->setIcon(QIcon(":icons/images/delete.svg"));

        modify_item_action = menu->addAction("修改");
        modify_item_action->setIcon(QIcon(":icons/images/modify.svg"));

        move_up_action = menu->addAction("上移");
        move_up_action->setIcon(QIcon(":icons/images/move_up.svg"));

        move_down_action = menu->addAction("下移");
        move_down_action->setIcon(QIcon(":icons/images/move_down.svg"));
    }
};


VotePage::VotePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new VotePageUi)
    , vote_data_model(new VoteItemViewModel("投票项", this))
    , vote_item_right_menu(new VoteItemRightMenu(this))
    , show_widget(find_root_widget(this)) {
    ui->setup_ui(this);
    this->setTitleVisible(false);
    ui->vote_item_view->setModel(vote_data_model);

    QStringList vote_history_cols = {"id", "创建者", "创建时间", "主题", "类型", "check"};
    vote_history_model            = new VoteHistoryViewModel(vote_history_cols, 10, this);
    ui->vote_history_view->setModel(vote_history_model);

    // only for test!
    QStringList items = {"钓鱼", "打球", "爬山", "LOL", "游泳", "骑自行车"};
    vote_data_model->add_vote_items(items);

    this->initialize_connects();
}

VotePage::~VotePage() {
    delete ui;
    delete vote_data_model;
    delete vote_item_right_menu;
}


void VotePage::show_message(const QString& message, bool error) {
    if (error) {
        ElaMessageBar::warning(ElaMessageBarType::TopLeft,
                               "vote",
                               message,
                               ClientGlobalConfig::error_show_time,
                               show_widget);
    } else {
        ElaMessageBar::success(ElaMessageBarType::TopLeft,
                               "vote",
                               message,
                               ClientGlobalConfig::message_show_time,
                               show_widget);
    }
}

void VotePage::display_vote_item_right_menu(const QPoint& pos) {
    auto     index      = ui->vote_item_view->indexAt(pos);
    int      row        = index.row();
    auto     global_pos = ui->vote_item_view->mapToGlobal(pos);
    QAction* selected   = vote_item_right_menu->menu->exec(global_pos);
    if (selected == vote_item_right_menu->move_up_action) {
        emit start_swap_vote_item(row, row - 1);
    } else if (selected == vote_item_right_menu->move_down_action) {
        emit start_swap_vote_item(row, row + 1);
    } else if (selected == vote_item_right_menu->delete_item_action) {
        emit start_delete_vote_item(row);
    }
}


void VotePage::click_vote_items(const QModelIndex& index) {
    if (index.isValid()) {
        int           row       = index.row();
        int           col       = index.column();
        const QString vote_item = this->vote_data_model->get_vote_item(row);
        ElaMessageBar::information(ElaMessageBarType::TopLeft,
                                   "click vote item",
                                   QString("当前选中的投票项是%1").arg(vote_item),
                                   2700,
                                   this);
    }
}


void VotePage::adjust_vote_history_view() {
    auto width = ui->vote_history_view->width();
    // the min size of first column is 12
    int w0 = std::max(12, static_cast<int>(0.05 * width));
    int w1 = static_cast<int>(0.15 * width);
    int w2 = static_cast<int>(0.3 * width);
    int w3 = static_cast<int>(0.3 * width);
    // the min size of this column is 16
    int w4 = std::max(16, static_cast<int>(0.1 * width));
    int w5 = static_cast<int>(0.1 * width);

    ui->vote_history_view->setColumnWidth(0, w0);
    ui->vote_history_view->setColumnWidth(1, w1);
    ui->vote_history_view->setColumnWidth(2, w2);
    ui->vote_history_view->setColumnWidth(3, w3);
    ui->vote_history_view->setColumnWidth(4, w4);
    ui->vote_history_view->setColumnWidth(5, w5);
}

void VotePage::clear_vote_data() {
    // key,value is a better choice!
    ui->vote_creator_value->clear();
    ui->vote_creator_value->setText("西雅图");
    ui->vote_topic_line_edit->clear();
    ui->voters_combox->clearEditText();
    vote_data_model->clear();
}

void VotePage::initialize_connects() {
    connect(ui->vote_history_view,
            &ElaTableView::tableViewShow,
            this,
            &VotePage::adjust_vote_history_view);
    connect(ui->adjust_history_view_button,
            &ElaToolButton::clicked,
            this,
            &VotePage::adjust_vote_history_view);

    connect(ui->new_vote_button, &ElaToolButton::clicked, this, &VotePage::clear_vote_data);

    connect(ui->vote_item_view,
            &ElaTableView::customContextMenuRequested,
            this,
            &VotePage::display_vote_item_right_menu);

    connect(ui->add_vote_item_button, &ElaToolButton::clicked, this, [this]() {
        QString data = ui->add_vote_item_line_edit->text();
        if (data.isEmpty()) {
            this->show_message("不能添加空选项 (┬┬﹏┬┬)");
            return;
        } else if (this->vote_data_model->contains(data)) {
            this->show_message(QString("%1已存在,不能添加重复值＞﹏＜(●'◡'●)").arg(data));
            return;
        }
        this->show_message(QString("成功添加了投票项 %1 (●'◡'●)").arg(data), false);
        this->vote_data_model->add_vote_item(std::move(data));
    });

    // connect multi time...
    connect(this, &VotePage::start_swap_vote_item, this, [this](int i, int j) {
        // this->show_message(QString("swap %1 and %2").arg(i).arg(j));
        this->vote_data_model->swap_item(i, j);
    });

    connect(this, &VotePage::start_delete_vote_item, this, [this](int i) {
        this->vote_data_model->delete_item(i);
    });



    connect(ui->vote_item_view, &ElaTableView::clicked, this, &VotePage::click_vote_items);
}

}   // namespace client
}   // namespace tang