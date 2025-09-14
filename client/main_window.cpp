#include "main_window.h"
#include "vote_page.h"
#include "vote_result_page.h"
#include "workspace_page.h"

namespace tang {
namespace client {
ClientMainWindow::ClientMainWindow(QWidget* parent)
    : ElaWindow(parent) {
    this->init_page();
    this->resize(QSize(1200, 600));

    this->moveToCenter();
}

void ClientMainWindow::init_page() {
    RemoteWorkspacePage* remote_file_page = new RemoteWorkspacePage(this);
    addPageNode("工作空间", remote_file_page, ElaIconType::CabinetFiling);

    VotePage* vote_page = new VotePage(this);
    addPageNode("发起投票", vote_page, ElaIconType::FaceKiss);

    VoteResultPage* vote_result_page = new VoteResultPage(this);
    addPageNode("投票结果", vote_result_page, ElaIconType::Joystick);
}
}   // namespace client
}   // namespace tang