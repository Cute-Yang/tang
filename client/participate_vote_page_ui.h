#pragma once
#include "ElaComboBox.h"
#include "ElaMultiSelectComboBox.h"
#include "ElaScrollPage.h"
#include "ElaSpinBox.h"
#include "ElaTableView.h"
#include "ElaText.h"
#include "ElaToolButton.h"
#include <QStackedWidget>

namespace tang {
namespace client {

class ParticipateVotePageUi {
public:
    ElaText* vote_id_key;
    ElaText* vote_id_value;

    ElaText* vote_creator_key;
    ElaText* vote_creator_value;

    ElaText* vote_create_time_key;
    ElaText* vote_create_time_value;

    ElaText* vote_topic_key;
    ElaText* vote_topic_value;

    ElaText* vote_type_key;
    ElaText* vote_type_value;

    ElaText* vote_status_key;
    ElaText* vote_status_value;

    ElaText*                vote_item_key;
    QStackedWidget*         combox_stacked_container;
    ElaComboBox*            vote_item_combox;
    ElaMultiSelectComboBox* vote_item_multi_combox;

    ElaToolButton* confirm_vote_button;

    ElaTableView* vote_todo_list;

    ElaToolButton* adjust_content_view_button;

    ElaToolButton*          refresh_todo_list_button;
    ElaSpinBox*             current_todo_list_page;
    ElaText*                totoal_todo_list_page;

    ElaText* vote_status_text;
    ElaMultiSelectComboBox* select_vote_status_combox;

    ElaText* vote_process_status_text;
    ElaMultiSelectComboBox* select_vote_process_status_combox;

    void setup_ui(ElaScrollPage* page);
};
}   // namespace client
}   // namespace tang