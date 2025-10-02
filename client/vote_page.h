#pragma once
#include "ElaScrollPage.h"
#include "vote_history_view_model.h"
#include "vote_item_view_model.h"
#include "vote_page_ui.h"

namespace tang {
namespace client {


struct OnlineVoterInfo {
    QString  voter_name;
    uint32_t voter_id;
};


class VoteItemRightMenu;
class VotePage : public ElaScrollPage {
    Q_OBJECT
private:
    VotePageUi*                  ui;
    VoteItemViewModel*           vote_data_model;
    VoteHistoryViewModel*        vote_history_model;
    VoteItemRightMenu*           vote_item_right_menu;
    QWidget*                     show_widget;
    std::vector<OnlineVoterInfo> online_voters;
    void                         show_message(const QString& message, bool error = true);
    void                         adjust_vote_history_view();
    void                         initialize_connects();
    void                         clear_vote_data();
    void                         display_vote_item_right_menu(const QPoint& pos);
    void                         get_online_voters_impl();
    void                         create_vote_impl();
    void                         display_vote_history_impl(const VoteHistory& vote_history);

    bool prepare_vote_json_data(QJsonObject& json_data, VoteHistory& vote_history);

    void set_frozon(bool enable = false);

    void refresh_vote_history_impl();

    void get_chunk_vote_data_impl(size_t vote_num, size_t vote_offset);

signals:
    void start_swap_vote_item(int i, int j);
    void start_delete_vote_item(int i);

public:
    VotePage(QWidget* parent = nullptr);
    ~VotePage();

    void set_test_model();
};
}   // namespace client
}   // namespace tang