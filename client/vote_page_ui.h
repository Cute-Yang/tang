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
    ElaText* vote_creator_text;
    ElaText* vote_creator_name_text;

    // vote topic
    ElaText*     vote_topic_text;
    ElaLineEdit* vote_topic_line_edit;

    // single/multi choice?
    ElaText*     vote_choice_type;
    ElaComboBox* vote_choice_type_combox;

    // vote items
    // ElaListView* vote_items;
    ElaTableView* vote_items;

    // add item button
    ElaToolButton* add_vote_item_button;

    ElaLineEdit* add_vote_item_line_edit;

    // voters
    ElaText*                voters_text;
    ElaMultiSelectComboBox* voters_combox;
    ElaToolButton*          flush_voters_button;

    ElaToolButton* new_vote_button;
    ElaToolButton* start_vote_button;

    ElaToolButton* vote_history_text_button;
    ElaTableView* vote_history;
    void          setup_ui(ElaScrollPage* page);
};
}   // namespace client
}   // namespace tang