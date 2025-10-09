#pragma once
#include "ElaScrollPage.h"
#include "vote_result_page_ui.h"
#include "vote_result_view_model.h"
namespace tang {
namespace client {
class VoteResultPage : public ElaScrollPage {
    Q_OBJECT

private:
    VoteResultPageUi* ui;
    // why call it model?I don't like it!!!
    VoteItemCountViewModel*     vote_count_model;
    VoteResultHistoryViewModel* vote_result_history_model;

    void show_message(const QString& message, bool error = true);
    void adjust_vote_result_histroy_view();
    void init_connects();
    void refresh_finished_vote_impl();
    void get_chunk_finished_vote_data_impl(int vote_num, int vote_offset);

    void display_result_history_impl(size_t i);

    void plot_impl(std::span<VoteItemCountInfo> vote_item_infos);

public:
    VoteResultPage(QWidget* parent = nullptr);

    ~VoteResultPage();
};
}   // namespace client
}   // namespace tang