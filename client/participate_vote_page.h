#include "ElaScrollPage.h"
#include "participate_vote_page_ui.h"
#include <QWidget>
#include "participate_view_model.h"

namespace tang {
namespace client {
class ParticipateVotePage : public ElaScrollPage {
private:
    ParticipateVotePageUi* ui;
    ParticipateViewModel* view_model;

public:
    ParticipateVotePage(QWidget* parent = nullptr);
    ~ParticipateVotePage();
};
}   // namespace client
}   // namespace tang