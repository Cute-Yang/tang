#pragma once
#include "ElaText.h"
#include "ElaComboBox.h"
#include "ElaMultiSelectComboBox.h"
#include "ElaToolButton.h"
#include "ElaTableView.h"
#include <QStackedWidget>
#include "ElaScrollPage.h"


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
        
        ElaText* vote_item_key;
        QStackedWidget* combox_stacked_container;
        ElaComboBox* vote_item_combox;
        ElaMultiSelectComboBox* vote_item_multi_combox;

        ElaToolButton* confirm_vote_button;

        ElaTableView* vote_todo_list;

        void setup_ui(ElaScrollPage* page);

};
}
}