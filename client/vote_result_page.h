#pragma once
#include "ElaScrollPage.h"
#include "vote_result_page_ui.h"
#include "vote_result_view_model.h"
namespace tang {
namespace client {
class VoteResultPage : public ElaScrollPage{
Q_OBJECT

private:
    VoteResultPageUi* ui;
    //why call it model?I don't like it!!!
    VoteResultViewModel* vote_result_model;

    void set_test_data();
    
public:
    VoteResultPage(QWidget* parent = nullptr);

    ~VoteResultPage();
};
}
}