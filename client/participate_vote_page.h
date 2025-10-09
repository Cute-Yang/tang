#pragma once
#include "ElaScrollPage.h"
#include "participate_view_model.h"
#include "participate_vote_page_ui.h"
#include <QWidget>


namespace tang {
namespace client {
class ParticipateVotePage : public ElaScrollPage {
private:
    ParticipateVotePageUi* ui;
    ParticipateViewModel*  view_model;
    QWidget*               show_widget;
    int                    vote_data_index;
    void                   initialize_connects();
    void                   adjust_vote_todo_list_view();
    void                   display_vote_data(const VoteData& vote_data);
    void                   set_frozon(bool enable);

    void refresh_participate_vote_history_impl();

    void get_chunk_participate_vote_data_impl(int vote_num, int vote_offset);

    void show_message(const QString& message, bool error = true);

    void send_vote_choices_impl();

public:
    ParticipateVotePage(QWidget* parent = nullptr);
    ~ParticipateVotePage();
    void refresh_for_once();
};
}   // namespace client
}   // namespace tang