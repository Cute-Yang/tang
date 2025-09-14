#pragma once
#include "ElaScrollPage.h"
#include "vote_page_ui.h"
#include "vote_item_view_model.h"

namespace tang {
namespace client {
class VotePage : public ElaScrollPage {
    Q_OBJECT
private:
    VotePageUi* ui;
    VoteItemViewModel* vote_data;

public:
    VotePage(QWidget* parent = nullptr);
    ~VotePage();

    void set_test_model();
};
}   // namespace client
}   // namespace tang