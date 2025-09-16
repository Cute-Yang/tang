#include "ElaScrollPage.h"
#include "participate_vote_page_ui.h"
#include <QWidget>

namespace tang {
namespace client {
class ParticipateVotePage : public ElaScrollPage {
public:
    enum class VoteStatus : uint8_t {
        // not vote
        ready = 0,
        // alreay vote
        finish = 1,
        // the vote is invalid!
        pending = 2
    };

private:
    ParticipateVotePageUi* ui;

public:
    ParticipateVotePage(QWidget* parent = nullptr);
    ~ParticipateVotePage();
};
}   // namespace client
}   // namespace tang