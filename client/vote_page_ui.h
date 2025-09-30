#pragma once
#include "ElaComboBox.h"
#include "ElaLineEdit.h"
#include "ElaMultiSelectComboBox.h"
#include "ElaScrollPage.h"
#include "ElaTableView.h"
#include "ElaText.h"
#include "ElaToolButton.h"



namespace tang {
namespace client {
class VotePageUi {
public:
    // vote_initator
    ElaText* vote_creator_key;
    ElaText* vote_creator_value;;

    // vote topic
    ElaText*     vote_topic_key;
    ElaLineEdit* vote_topic_line_edit;

    // single/multi choice?
    ElaText*     vote_choice_type_key;
    ElaComboBox* vote_choice_type_combox;

    ElaTableView* vote_item_view;

    // add item button
    ElaToolButton* add_vote_item_button;

    ElaLineEdit* add_vote_item_line_edit;

    // voters
    ElaText*                voters_key;
    ElaMultiSelectComboBox* voters_combox;
    ElaToolButton*          flush_voters_button;

    ElaToolButton* new_vote_button;
    ElaToolButton* start_vote_button;

    ElaToolButton* vote_history_text_button;

    ElaToolButton* adjust_history_view_button;
    ElaTableView* vote_history_view;
    void          setup_ui(ElaScrollPage* page);
};
}   // namespace client
}   // namespace tang