#pragma once
#include "ElaScrollPage.h"
#include "vote_page_ui.h"
#include "vote_item_view_model.h"
#include "vote_history_view_model.h"

namespace tang {
namespace client {
class VotePage : public ElaScrollPage {
    Q_OBJECT
private:
    VotePageUi* ui;
    VoteItemViewModel* vote_data;
    VoteHistoryViewModel* vote_history_model;
    
    void adjust_vote_history_view();
public:
    VotePage(QWidget* parent = nullptr);
    ~VotePage();

    void set_test_model();

public slots:
    void click_vote_items(const QModelIndex& index);
};
}   // namespace client
}   // namespace tang