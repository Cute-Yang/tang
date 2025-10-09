#pragma once
#include "ElaContentDialog.h"
#include "ElaWindow.h"
#include "chat_room.h"
#include "participate_vote_page.h"
#include "vote_page.h"
#include "vote_result_page.h"
#include "workspace_page.h"


namespace tang {
namespace client {
class Client;
class ClientMainWindow : public ElaWindow {
    Q_OBJECT
private:
    RemoteWorkspacePage* remote_workspace_page;
    VotePage*            vote_page;
    ChatRoom*            chat_room;
    ParticipateVotePage* participate_vote_page;
    VoteResultPage*      vote_result_page;
    ElaContentDialog*    close_dialog;

    QString setting_key{""};
    void    init_page();

public:
    friend class Client;
    ClientMainWindow(QWidget* parent = nullptr);
    ~ClientMainWindow() {}

    void init_user_display_info();

    // private:
};
}   // namespace client
}   // namespace tang