#pragma once
#include "ElaLineEdit.h"
#include "ElaScrollPage.h"
#include "ElaTableView.h"
#include "ElaText.h"
#include <QChart>
#include <QChartView>
#include <QPieSeries>

namespace tang {
namespace client {
class VoteResultPageUi {
public:
    ElaText* vote_creator_text;
    ElaText* vote_creator_name_text;

    ElaText* vote_create_time;
    ElaText* vote_create_time_value;

    ElaText*     vote_topic_text;
    ElaText* vote_topic_value;

    ElaText* vote_choice_type;
    ElaText* vote_choice_type_value;

    ElaText*     vote_result_text;
    ElaText* vote_result_value;

    ElaTableView* vote_stat_view;

    // the series restore the data of it!
    // the series should notify the char to update the img!
    QPieSeries* vote_img_series;
    
    // maybe not need this?
    QChartView* vote_img_view;

    QChart* vote_img_chart;


    ElaTableView* vote_history_view;
    void setup_ui(ElaScrollPage* page = nullptr);
};


}   // namespace client
}   // namespace tang