#pragma once
#include "ElaScrollPage.h"
#include "vote_history_view_model.h"
#include "vote_item_view_model.h"
#include "vote_page_ui.h"

namespace tang {
namespace client {

class VoteItemRightMenu;
class VotePage : public ElaScrollPage {
    Q_OBJECT
private:
    VotePageUi*           ui;
    VoteItemViewModel*    vote_data_model;
    VoteHistoryViewModel* vote_history_model;
    VoteItemRightMenu*    vote_item_right_menu;
    QWidget*              show_widget;

    void show_message(const QString& message, bool error = true);

    void adjust_vote_history_view();

    void initialize_connects();
    void clear_vote_data();

    void display_vote_item_right_menu(const QPoint& pos);

signals:
    void start_swap_vote_item(int i, int j);
    void start_delete_vote_item(int i);

public:
    VotePage(QWidget* parent = nullptr);
    ~VotePage();

    void set_test_model();

public slots:
    void click_vote_items(const QModelIndex& index);
};
}   // namespace client
}   // namespace tang